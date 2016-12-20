
#import "ChatRoomViewController.h"
#import <ZegoKit/AVRoom.h>
#import "CoolxSdkDemoHelp.h"
//#import "MyToast.h"
#import "ZegoKitManager.h"

@interface ChatRoomViewController ()
{
    NSString *strRoomKey;
    NSString *strUserID;
    NSString *strUserName;
    AVRoom* avRoom;
    bool bAux;
    bool bRecord;
    NSData *auxData;
    unsigned char *pPos;
    unsigned char *pData;
    FILE *fd;
}
@property (weak, nonatomic) IBOutlet UITextView *tvShowMsg;
@property (weak, nonatomic) IBOutlet UITextField *editSendMsg;
@property (weak, nonatomic) IBOutlet UIButton *btnSend;
@property (weak, nonatomic) IBOutlet UIButton *btnEnableSpeaker;
@property (weak, nonatomic) IBOutlet UIButton *btnEnableMic;
@property (weak, nonatomic) IBOutlet UIButton *btnPause;

@property (weak, nonatomic) IBOutlet UIButton *btnAux;
@property (weak, nonatomic) IBOutlet UIButton *btnRecord;
@end

@implementation ChatRoomViewController
- (IBAction)EnableSpeaker:(id)sender
{
    bool bEnableSpeaker = [avRoom GetSpeakerState];
    bEnableSpeaker = !bEnableSpeaker;
    [avRoom EnableSpeaker:bEnableSpeaker];
    if (bEnableSpeaker)
        [self.btnEnableSpeaker setTitle:@"关扬声器" forState:UIControlStateNormal];
    else
        [self.btnEnableSpeaker setTitle:@"开扬声器" forState:UIControlStateNormal];
}

- (IBAction)EnableMic:(id)sender
{
    bool bEnableMic = [avRoom GetMicState];
    bEnableMic = !bEnableMic;
    [avRoom EnableMic:bEnableMic];
    if (bEnableMic)
        [self.btnEnableMic setTitle:@"关Mic" forState:UIControlStateNormal];
    else
        [self.btnEnableMic setTitle:@"开Mic" forState:UIControlStateNormal];
}

- (IBAction)PauseAndResume:(id)sender
{
    AUDIO_STATE pause = [avRoom GetAudioState];
    if (pause == EN_AUDIO_PAUSE)
    {
        [avRoom ResumeAudio];
        [self.btnPause setTitle:@"暂停" forState:UIControlStateNormal];
    }
    else if (pause == EN_AUDIO_START)
    {
        [avRoom PauseAudio];
        [self.btnPause setTitle:@"恢复" forState:UIControlStateNormal];
    }
}

- (IBAction)GetRoomUsers:(id)sender
{
   NSArray* roomUsers = [avRoom GetRoomUsers];
    
    NSString *strMsg = [NSString new];
    
    for (NSObject *object in roomUsers)
    {
        NSString *strUser = [[NSString alloc] initWithFormat:@"%@(%@)\n", ((RoomUser*)object).strUserName, ((RoomUser*)object).strUserID];
        strMsg = [strMsg stringByAppendingString:strUser];
    }
    
    UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"成员列表" message:strMsg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    
    [alter show];
}

- (IBAction)SendMsg:(id)sender
{
    NSString* strMsg = self.editSendMsg.text;
    self.editSendMsg.text = @"";
    long long llSeq = [avRoom SendBroadcastTextMsg:strMsg];
    if (llSeq < 0)
    {
        [CoolxSdkDemoHelp ShowToast:@"发送失败"];
        return;
    }
    //上屏
    NSDate *now = [NSDate date];
    NSString *strShow = [NSString new];
    NSDateFormatter *dateFormatter = [NSDateFormatter new];
    [dateFormatter setDateFormat:@"HH:mm\n"];
    NSString *strDate = [dateFormatter stringFromDate:now];
    strShow = [strShow stringByAppendingString:strDate];
    strShow = [strShow stringByAppendingString:@"我:"];
    //strShow = [strShow stringByAppendingString:@":"];
    strShow = [strShow stringByAppendingString:strMsg];
    strShow = [strShow stringByAppendingString:@"\n"];
    [self ShowMSG:strShow];
    //[self.editSendMsg resignFirstResponder];
}

- (IBAction)aux:(id)sender {
    bAux = !bAux;
    [avRoom EnableAux:bAux];
    pPos = pData;
    if (bAux)
        [self.btnAux setTitle:@"关伴奏" forState:UIControlStateNormal];
    else
        [self.btnAux setTitle:@"开伴奏" forState:UIControlStateNormal];
}

- (IBAction)recorder:(id)sender {
    bRecord = !bRecord;
    [avRoom EnableRecorder:bRecord];
    if (bRecord){
        if (!fd){
            NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
            NSString *cachesDir = [paths objectAtIndex:0];
            NSFileManager *fileManager = [NSFileManager defaultManager];
            NSString *recodeFile = [cachesDir stringByAppendingPathComponent:@"zegoRecord"];
            [fileManager removeItemAtPath:recodeFile error:nil];
            [fileManager createFileAtPath:recodeFile contents:nil attributes:nil];
            
            char * pFile = (char*)[recodeFile UTF8String];
            fd = fopen(pFile,"ab+");
        }
        [self.btnRecord setTitle:@"关录音" forState:UIControlStateNormal];
    }
    else
    {
        if (fd) {
            fclose(fd);
            fd = NULL;
        }
        [self.btnRecord setTitle:@"开录音" forState:UIControlStateNormal];
    }
}

- (void) setMNotifEntity:(NotificationEntity *)mNotify
{
    _mNotifEntity = mNotify;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    //NSLog(@"mNotifEntity: %@", self);
    avRoom = [ZegoKitManager sharedInstance].avRoom;
    
    strRoomKey = [self.mNotifEntity strRoomKey];
    strUserID = [self.mNotifEntity strUserID];
    strUserName = [self.mNotifEntity strUserName];
    bAux = false;
    //NSLog(@"testlog");
    NSURL *url=[[NSBundle mainBundle]URLForResource:@"a.pcm" withExtension:nil];
    auxData = [NSData dataWithContentsOfURL:url options:0 error:nil];
    pData = (unsigned char *)[auxData bytes];
    pPos = pData;
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.navigationController.navigationBar.hidden = NO;
//    avRoom = [[AVRoom alloc] init];
    [avRoom SetCallback:self callbackQueue:dispatch_get_main_queue()];
    /*
    Byte signkey[] = {0x91,0x93,0xcc,0x66,0x2a,0x1c,0xe,0xc1,
        0x35,0xec,0x71,0xfb,0x7,0x19,0x4b,0x38,
        0x15,0xf1,0x43,0xf5,0x7c,0xd2,0xb5,0x9a,
        0xe3,0xdd,0xdb,0xe0,0xf1,0x74,0x36,0xd};
    
    NSData *nsDataAppSignature = [[NSData alloc] initWithBytes:signkey length:32];

    [avRoom SetLogLevel:AVROOM_LOG_LEVEL_DEBUG];
    [avRoom Init:1 AppSignature:nsDataAppSignature];
     */
    int nEnv = 0;
    nEnv = [self.mNotifEntity nEnv];
    if (nEnv == 1)
    {
        NSString *strIP = [self.mNotifEntity strTestIP];
        NSString *strPort = [self.mNotifEntity strTestPort];
        [avRoom SetTestServer:strIP Port:[strPort intValue] Enable:true];
    }
    else
    {
        [avRoom SetTestServer:@"" Port:0 Enable:false];
    }
    int nRoomKey = [strRoomKey intValue];
    RoomUser* roomUser = [RoomUser new];
    [roomUser setStrUserID:strUserID];
    [roomUser setStrUserName:strUserName];
    [avRoom GetInRoom:nRoomKey RoomUser:roomUser];
    
    [self.editSendMsg addTarget:self action:@selector(textFieldDidBeginEditing:) forControlEvents:UIControlEventEditingDidBegin];
    [self.editSendMsg addTarget:self action:@selector(textFieldDidEndEditing:) forControlEvents:UIControlEventEditingDidEnd];
    self.tvShowMsg.editable = NO;
    
    UITapGestureRecognizer *singleTap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(SingleTouch:)];
    [self.tvShowMsg addGestureRecognizer:singleTap];
    
    UIScreen *currentScreen = [UIScreen mainScreen];
    CGRect editSendMsgFrame = self.editSendMsg.frame;
    editSendMsgFrame.origin.y = currentScreen.applicationFrame.size.height - editSendMsgFrame.size.height - 20 + currentScreen.applicationFrame.origin.y;
    editSendMsgFrame.origin.x = currentScreen.applicationFrame.origin.x + 10;
    self.editSendMsg.frame = editSendMsgFrame;

    CGRect btnSendFrame = self.btnSend.frame;
    btnSendFrame.origin.y = currentScreen.applicationFrame.size.height - btnSendFrame.size.height - 20 + currentScreen.applicationFrame.origin.y;
    btnSendFrame.origin.x = currentScreen.applicationFrame.size.width - btnSendFrame.size.width - 20;
    self.btnSend.frame = btnSendFrame;
    
    CGRect tvShowMsgFrame = self.tvShowMsg.frame;
    tvShowMsgFrame.size.height = currentScreen.applicationFrame.size.height - editSendMsgFrame.size.height - 160;
    self.tvShowMsg.frame = tvShowMsgFrame;

}

-(void)SingleTouch:(UIGestureRecognizer *)gestureRecognizer
{
    [self.editSendMsg resignFirstResponder];
}

-(void)textFieldDidBeginEditing:(UITextField *)textField
{
    //开始编辑时，整体上移
    [self moveView:-240];
}

-(void)textFieldDidEndEditing:(UITextField *)textField
{
    //结束编辑时，整体下移
    [self moveView:240];
    //[self.editSendMsg resignFirstResponder];
}

- (BOOL)textViewShouldBeginEditing:(UITextView *)textView{
    return NO;
}

-(void)moveView:(float)move{
    NSTimeInterval animationDuration = 0.30f;
    CGRect frame = self.view.frame;
    //frame.origin.y +=move;//view的y轴上移
    frame.size.height += move;
    CGRect tvFrame = self.tvShowMsg.frame;
    tvFrame.size.height += move;
    self.tvShowMsg.frame = tvFrame;
    self.view.frame = frame;
    CGRect edFrame = self.editSendMsg.frame;
    edFrame.origin.y += move;
    self.editSendMsg.frame = edFrame;
    
    CGRect sendBtnFrame = self.btnSend.frame;
    sendBtnFrame.origin.y += move;
    self.btnSend.frame = sendBtnFrame;
    
    [UIView beginAnimations:@"ResizeView" context:nil];
    [UIView setAnimationDuration:animationDuration];
    //self.view.frame = frame;
    [UIView commitAnimations];//设置调整界面的动画效果
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    [avRoom SetCallback:nil callbackQueue:nil];
    [avRoom LeaveRoom];
    if (fd) {
        fclose(fd);
        fd = NULL;
    }
}

- (BOOL)textFieldShouldReturn:(UITextField *)theTextField {
    if (theTextField == self.editSendMsg)
    {
        [theTextField resignFirstResponder];
    }
    return YES;
}

- (IBAction)backgroundTap:(id)sender {
    [self.editSendMsg resignFirstResponder];
}

- (void) OnKickOut:(int) nReason Msg:(NSString*)strMsg
{
}

- (void) OnGetInResult:(int) nResult RoomKey:(int)nRoomKey
{
    NSString *strLoginTip;
    //int nRoomKeyBase = [CoolxSdkDemoHelp coolxSdkDemoRoomKeyBase];
    //int nRoomPos = nRoomKey % nRoomKeyBase + 1;
    if (nResult == 0)  //login ok
    {
        strLoginTip  = [NSString stringWithFormat:@"进入房间%d成功\n", nRoomKey];
    }
    else
    {
        strLoginTip  = [NSString stringWithFormat:@"进入房间%d失败\n", nRoomKey];
    }
    [self ShowMSG:strLoginTip];
    
}

- (void) OnRoomUsersUpdate:(NSArray*) arrNewUsers LeftUsers:(NSArray*)arrLeftUsers
{
    NSString *strMsg = [NSString new];
    
    for (NSObject *object in arrNewUsers)
    {
        NSString *strUser = [[NSString alloc] initWithFormat:@"%@(%@) 进入房间\n", ((RoomUser*)object).strUserName, ((RoomUser*)object).strUserID];
        strMsg = [strMsg stringByAppendingString:strUser];
    }
    
    for (NSObject *object in arrLeftUsers)
    {
        NSString *strUser = [[NSString alloc] initWithFormat:@"%@(%@) 离开房间\n", ((RoomUser*)object).strUserName, ((RoomUser*)object).strUserID];
        strMsg = [strMsg stringByAppendingString:strUser];
    }
    
    [self ShowMSG:strMsg];
}

- (void) OnReceiveBroadcastTextMsg:(AVRoomTextMsg*) textMsg
{
    NSString *strMsg = [NSString new];
    NSDate *date = [[NSDate alloc] initWithTimeIntervalSince1970:textMsg.sendTime];
    NSDateFormatter *dateFormatter = [NSDateFormatter new];
    [dateFormatter setDateFormat:@"HH:mm\n"];
    NSString *strDate = [dateFormatter stringFromDate:date];
    strMsg = [strMsg stringByAppendingString:strDate];
    strMsg = [strMsg stringByAppendingString:textMsg.oRoomUser.strUserName];
    strMsg = [strMsg stringByAppendingString:@":"];
    strMsg = [strMsg stringByAppendingString:textMsg.strMsg];
    strMsg = [strMsg stringByAppendingString:@"\n"];
    [self ShowMSG:strMsg];
}

- (void) OnSelfBeginTalking
{
    [CoolxSdkDemoHelp ShowToast:@"自己开始说话"];
}

//- (void) OnSelfKeepTalking
//{
//    [CoolxSdkDemoHelp ShowToast:@"自己正在说话"];
//}

- (void) OnSelfEndTalking
{
    [CoolxSdkDemoHelp ShowToast:@"自己结束说话"];
}

- (void) OnOthersBeginTalking:(RoomUser*) user
{
    NSString* strTip  = [NSString stringWithFormat:@"%@[%@]开始说话", user.strUserName, user.strUserID];
    [CoolxSdkDemoHelp ShowToast:strTip];
}

//- (void) OnOthersKeepTalking:(RoomUser*) user
//{
//    NSString* strTip  = [NSString stringWithFormat:@"%@[%@]正在说话", user.strUserName, user.strUserID];
//    [CoolxSdkDemoHelp ShowToast:strTip];
//}

- (void) OnOthersEndTalking:(RoomUser*) user
{
    NSString* strTip  = [NSString stringWithFormat:@"%@[%@]结束说话", user.strUserName, user.strUserID];
    [CoolxSdkDemoHelp ShowToast:strTip];
}

- (void) OnDisconnected:(int) nErrorCode
{
    NSString* strTip  = [NSString stringWithFormat:@"掉线了，可能是网络问题，处理好网络问题可以调用ReGetInRoom"];
    [CoolxSdkDemoHelp ShowToast:strTip];
}

- (void) OnAuxCallback:(unsigned char *)pDataOut DataLen:(int*)pDataLen SampleRate:(int*)pSampleRate NumChannels:(int*)pNumChannels BitDepth:(int*)pBitDepth
{
    if(auxData)
    {
        int nLen = (int)[auxData length];
        
        int nLeftLen = (int)(pData + nLen - pPos);
        if (nLeftLen < *pDataLen) {
            pPos = pData;
            *pDataLen = 0;
            return;
        }
        
        
        *pSampleRate = 44100;
        *pNumChannels = 2;
        *pBitDepth = 16;
            
        memcpy(pDataOut, pPos, *pDataLen);
		pPos = pPos + *pDataLen;
    }
}

- (void) OnRecorderCallback:(NSData*)data SampleRate:(int)sampleRate Channels:(int)channels BitDepth:(int)bitDepth
{
    NSLog(@"OnRecorderCallback Len = %d", (int)[data length]);
    if (fd)
    {
        Byte* pBytes = (Byte*)[data bytes];
        int nLen = (int)[data length];
        fwrite(pBytes, nLen, 1, fd);
    }
}

- (void) ShowMSG:(NSString*)strMsg
{
    if ([NSThread isMainThread])
    {
        NSString *strShowMsg = self.tvShowMsg.text;
        strShowMsg = [strShowMsg stringByAppendingString:strMsg];
        self.tvShowMsg.text = strShowMsg;
    }
    else
    {
        dispatch_async(dispatch_get_main_queue(), ^{
            NSString *strShowMsg = self.tvShowMsg.text;
            strShowMsg = [strShowMsg stringByAppendingString:strMsg];
            self.tvShowMsg.text = strShowMsg;
        });
    }
}
/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/
@end
