
#import "ViewController.h"
#import "ChatRoomViewController.h"
#import "SettingViewController.h"
#import "CoolxSdkDemoHelp.h"


typedef long long uint64;
static int g_nCustomRoomKeyPos = 4;

@interface ViewController ()
{
    NSArray *naRoom;
    int mNEnv;
    NSString *mStrTestIP;
    NSString *mStrTestPort;
}
@property (weak, nonatomic) IBOutlet UITextField *customRoomKey;
//@property (weak, nonatomic) IBOutlet UITextField *roomkey;
@property (weak, nonatomic) IBOutlet UITextField *userID;
@property (weak, nonatomic) IBOutlet UITextField *userName;
- (IBAction)onGetInRoom:(id)sender;
- (IBAction)aboutzego:(id)sender;
@property (weak, nonatomic) IBOutlet UIPickerView *pickerroom;
@property (weak, nonatomic) IBOutlet UIButton *btnAboutZego;
@property (weak, nonatomic) IBOutlet UIButton *btnSetting;

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
    NSDate *now = [NSDate date];
    NSTimeInterval userKey = [now timeIntervalSince1970];
    NSString *strUserID = [[NSString alloc] initWithFormat:@"%.0f", userKey];
    self.userID.text = strUserID;
    
    naRoom = [NSArray arrayWithObjects:@"房间1:900000",@"房间2:900001",@"房间3:900002",@"房间4:900003",@"自定义房间", nil];
    _customRoomKey.hidden = true;
    _customRoomKey.keyboardType = UIKeyboardTypeNumberPad;
    _pickerroom.delegate = self;
    _pickerroom.dataSource = self;
    
    mNEnv = 0;
    mStrTestIP = @"";
    mStrTestPort = @"";
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:animated];
    self.navigationController.navigationBar.hidden = YES;
    
    UIScreen *currentScreen = [UIScreen mainScreen];
    CGRect btnAboutZegoFrame = self.btnAboutZego.frame;
    btnAboutZegoFrame.origin.y = currentScreen.applicationFrame.size.height - btnAboutZegoFrame.size.height - 20 + currentScreen.applicationFrame.origin.y;
    btnAboutZegoFrame.origin.x = currentScreen.applicationFrame.size.width - btnAboutZegoFrame.size.width - 20;
    self.btnAboutZego.frame = btnAboutZegoFrame;
    
    CGRect btnSettingFrame = self.btnSetting.frame;
    btnSettingFrame.origin.y = btnAboutZegoFrame.origin.y;
    btnSettingFrame.origin.x = 20;
    self.btnSetting.frame = btnSettingFrame;
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView{
    return 1;
}

-(NSInteger) pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
    return [naRoom count];
}

-(NSString*) pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
    return [naRoom objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component{
    if (row == g_nCustomRoomKeyPos)
        _customRoomKey.hidden = false;
    else
        _customRoomKey.hidden = true;
}

- (IBAction)onGetInRoom:(id)sender {
}

- (IBAction)aboutzego:(id)sender {
    NSString *strMsg = @"深圳市即构科技有限公司\r\n官网: http://www.zego.im\r\n版本:V1.2.9";
    UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"关于我们" message:strMsg delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alter show];
}

- (IBAction)backgroundTap:(id)sender {
    //[self.roomkey resignFirstResponder];
    [self.userID resignFirstResponder];
    [self.userName resignFirstResponder];
    [self.customRoomKey resignFirstResponder];
}

- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender
{
    NSLog(@"identifier:%@", segue.identifier);
    if ([segue.identifier isEqualToString:@"ChatRoomViewController"])
    {
        NotificationEntity *mNotificationEntity = [[NotificationEntity alloc] init];
        
        //NSString *strRoomKey = self.roomkey.text;
        NSString *strUserID = self.userID.text;
        NSString *strUserName = self.userName.text;
        int nPos = (int)[_pickerroom selectedRowInComponent:0];
        int nRoomkey = [CoolxSdkDemoHelp coolxSdkDemoRoomKeyBase] + nPos;
        if (nPos == g_nCustomRoomKeyPos) { //customRoomKey
            NSString* strCustomRoomKey = self.customRoomKey.text;
            if (strCustomRoomKey.length == 0)
            {
                [CoolxSdkDemoHelp ShowToast:@"请输入房间号"];
                return;
            }
            nRoomkey = [strCustomRoomKey intValue];
            if (nRoomkey == 0)
            {
                [CoolxSdkDemoHelp ShowToast:@"房间号不能为0"];
                return;
            }
        }
        
        NSString *strRoomKey  = [NSString stringWithFormat:@"%d\n", nRoomkey];
        
        
        [mNotificationEntity setStrRoomKey:strRoomKey];
        [mNotificationEntity setStrUserName:strUserName];
        [mNotificationEntity setStrUserID:strUserID];
        [mNotificationEntity setNEnv:mNEnv];
        [mNotificationEntity setStrTestIP:mStrTestIP];
        [mNotificationEntity setStrTestPort:mStrTestPort];
        
        
        ChatRoomViewController* chatRoomView = segue.destinationViewController;
        [chatRoomView setMNotifEntity:mNotificationEntity];
    }
    else if([segue.identifier isEqualToString:@"SettingViewController"])
    {
        SettingViewController* settingView = segue.destinationViewController;
        [settingView InitParam:mNEnv IP:mStrTestIP Port:mStrTestPort];
        
        if ([settingView respondsToSelector:@selector(setMainViewController:)]) {
            [settingView setValue:self forKey:@"mainViewController"];
        }
    }
}

- (void)SetEnv:(int)nEnv IP:(NSString*)strIP Port:(NSString*)strPort
{
    mNEnv = nEnv;
    if (nEnv == 1)
    {
        mStrTestIP = strIP;
        mStrTestPort = strPort;
    }
}
@end