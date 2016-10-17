
#import <Foundation/Foundation.h>
#import <ZegoKit/AVRoomCallback.h>
#import <ZegoKit/RoomUser.h>
#import <ZegoKit/AVRoomTextMsg.h>
#import <ZegoKit/AVRoomCustomMsg.h>
#import <ZegoKit/AVRoomCallback.h>

@interface AVRoom : NSObject<AVRoomCallback>
{
}

typedef enum
{
    AVROOM_LOG_LEVEL_GRIEVOUS = 0,
    AVROOM_LOG_LEVEL_ERROR,
    AVROOM_LOG_LEVEL_WARNING,
    AVROOM_LOG_LEVEL_GENERIC,
    AVROOM_LOG_LEVEL_DEBUG
}AVROOM_LOG;

typedef enum
{
    EN_AUDIO_STOP = 0,
    EN_AUDIO_START,
    EN_AUDIO_PAUSE
}AUDIO_STATE;

/// \brief 初始化avRoom
/// \param nAppID Zego派发的数字ID，各个开发者的唯一标识
/// \param szAppSignature Zego派发的签名,用来校验对应appID的合法性
/// \return true:调用成功；false:调用失败
- (void) Init:(int)nAppID AppSignature:(NSData*)szAppSignature;

/// \brief 设置回调，和回调queue
- (void) SetCallback:(id<AVRoomCallback>)callback callbackQueue:(dispatch_queue_t)callbackQueue;

/// \brief 进入语音房间
/// \param nRoomKey 房间Key
/// \param oRoomUser 自己的账号信息
/// \return true:调用成功，等待AVRoomCallback的OnGetInResult的通知；false:调用失败
- (bool) GetInRoom:(int)nRoomKey RoomUser:(RoomUser*)oRoomUser;

/// \brief 退出语音房间
/// \return true：退出成功；false:退出失败
- (void) LeaveRoom;

/// \brief 获取房间成员列表
/// \return 成员列表
- (NSArray*) GetRoomUsers;

/// \brief 获取房间Key
/// \return Key
- (int) GetRoomKey;

/// \brief 切换扬声器和听筒
/// \param bOpenSpeaker true打开扬声器，false切换到听筒
- (void) EnableSpeaker:(bool)bOpenSpeaker;

/// \brief 获取是使用扬声器还是听筒
/// \return true:扬声器；false:听筒
- (bool) GetSpeakerState;

/// \brief 开关麦克风
/// \param bMute true打开，false关闭
- (void) EnableMic:(bool)bMute;

/// \brief 获取麦克风状态
/// \return true:开；false:关
- (bool) GetMicState;

/// \brief 在房间内广播一条文本消息
/// \param strMsg 文本内容
/// \return 成功则返回本条消息的seq，方便后续用来搜索，失败返回-1
- (long long) SendBroadcastTextMsg:(NSString*)strMsg;
//- (long long) SendCustomBroadcastMsg:(AVRoomCustomMsg*)szMsg;

/// \brief 掉线时，可以调用ReGetInRoom用来进入原来的房间
- (void) ReGetInRoom;

/// \brief 暂停语音功能
- (void) PauseAudio;

/// \brief 恢复语音功能
- (void) ResumeAudio;

/// \brief 获取语音的状态
/// \return AUDIO_STATE
- (AUDIO_STATE) GetAudioState;

/// \brief 设置ZegoLog的级别
/// \param nLevel 级别
- (void) SetLogLevel:(AVROOM_LOG) nLevel;

/// \brief 开关混音
/// \param bEnable true打开，false关闭
- (void) EnableAux:(bool)bEnable;

/// \brief 开关接收回调语音数据，打开后OnRecorderCallback会收到语音数据，主要让开发者用于录音
/// \param bEnable true打开，false关闭
- (void) EnableRecorder:(bool)bEnable;

- (void) SetAuxVolume:(int) volume;

- (void) SetPlayVolume:(int) volume;

- (void) EnableMute:(bool)bEnable;

/// 开发者无需关心这几个接口，测试使用
- (void) SetKeepInBackgroudTimeWhenDisconnect:(int) nSecond;
- (void) SetTestServer:(NSString*)strIP Port:(int)nPort Enable:(bool)bEnable;

@end
