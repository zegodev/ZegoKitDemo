
#import <Foundation/Foundation.h>
#import <ZegoKit/RoomUser.h>
#import <ZegoKit/AVRoomTextMsg.h>
#import <ZegoKit/AVRoomCustomMsg.h>

@protocol AVRoomCallback<NSObject>

- (void) OnGetInResult:(int) nResult RoomKey:(int)nRoomKey;
- (void) OnDisconnected:(int)nErrorCode;
- (void) OnKickOut:(int) nReason Msg:(NSString*)strMsg;

@optional
- (void) OnSendBroadcastTextMsgResult:(int) nResult Msg:(NSString*)strMsg MsgSeq:(long)lMsgSeq;
//- (void) OnSendCustomBroadcastMsgResult:(int) nResult Msg:(Byte[])szMsg MsgSeq:(long)lMsgSeq;
- (void) OnRoomUsersUpdate:(NSArray*) arrNewUsers LeftUsers:(NSArray*)arrLeftUsers;
- (void) OnRoomUserUpdateAll:(NSArray*) arrUsers;
- (void) OnReceiveBroadcastTextMsg:(AVRoomTextMsg*) textMsg;
//- (void) OnReceiveCustomBroadcastMsg:(AVRoomCustomMsg*) customMsg;
//- (void) OnError:(int) OnError Desc:(NSString*)strDesc;
- (void) OnSelfBeginTalking;
- (void) OnSelfKeepTalking;
- (void) OnSelfEndTalking;
- (void) OnOthersBeginTalking:(RoomUser*) user;
- (void) OnOthersKeepTalking:(RoomUser*) user;
- (void) OnOthersEndTalking:(RoomUser*) user;
- (void) OnAuxCallback:(unsigned char *)pData DataLen:(int*)pDataLen SampleRate:(int*)pSampleRate NumChannels:(int*)pNumChannels BitDepth:(int*)pBitDepth;
- (void) OnRecorderCallback:(NSData*)data SampleRate:(int)sampleRate Channels:(int)channels BitDepth:(int)bitDepth;

@end
