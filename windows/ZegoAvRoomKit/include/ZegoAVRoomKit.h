#ifndef __Zego_AvRoom_kit__
#define __Zego_AvRoom_kit__

#ifdef ZEGOAVROOMKIT_EXPORTS
#define ZEGOAVROOMKIT_API __declspec(dllexport)
#else
#define ZEGOAVROOMKIT_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

#define  ID_NAME_MAX_LEN  (64)
#define  MSG_MAX_LEN  (2*1024)

    typedef enum  {
        AVROOMAPI_NT_LINE = 1,
        AVROOMAPI_NT_WIFI = 2,
        AVROOMAPI_NT_2G = 3,
        AVROOMAPI_NT_3G = 4,
        AVROOMAPI_NT_4G = 5,
        AVROOMAPI_NT_UNKNOWN = 32
    }AVROOMAPI_NetType;

    typedef enum
    {
        AVROOMAPI_UPDATE_TYPE_ADD = 1,
        AVROOMAPI_UPDATE_TYPE_DEL,
        AVROOMAPI_UPDATE_TYPE_UPDATE,
    }AVROOMAPI_UpdateType;

    typedef struct
    {
        unsigned int		    nIndex;
        unsigned __int64        i64Id;
        char                    arrId[ID_NAME_MAX_LEN];
        char                    arrName[ID_NAME_MAX_LEN];
        unsigned int            nTermType;
        AVROOMAPI_UpdateType          eUpdateType;
    }AVROOMAPI_UserInfo;

    typedef struct
    {
        AVROOMAPI_UserInfo xSendUser;
        unsigned int nSendTime;
        char arrMsg[MSG_MAX_LEN];
    }AVROOMAPI_TextMsg;

    typedef struct
    {
        AVROOMAPI_UserInfo xSendUser;
        unsigned int nSendTime;
        char arrMsg[MSG_MAX_LEN];
        unsigned int nMsgLen;
    }AVROOMAPI_CustomMsg;


    class AVROOMAPI_IZegoAVRoomCallback
    {
    public:
        virtual ~AVROOMAPI_IZegoAVRoomCallback() {};
        virtual void OnGetInResult(int nResult, unsigned int uRoomKey) = 0;
        virtual void OnSendBroadcastTextMsgResult(int nResult, const char* psMsg, unsigned __int64 nMsgSeq) = 0;
        virtual void OnSendCustomBroadcastMsgResult(int nResult, const unsigned char* psMsg, unsigned int nMsgLen, unsigned __int64 nMsgSeq) = 0;

        virtual void OnRoomUsersUpdate(AVROOMAPI_UserInfo* pNewUsersList, unsigned int nNewUserCount, AVROOMAPI_UserInfo* pRemovedUsersList, unsigned int nRemovedUserCount) = 0;
        virtual void OnRoomUserUpdateAll(AVROOMAPI_UserInfo* pUsersList, unsigned int nUserCount) = 0;

        virtual void OnReceiveBroadcastTextMsg(AVROOMAPI_TextMsg& msg) = 0;
        virtual void OnReceiveCustomBroadcastMsg(AVROOMAPI_CustomMsg& msg) = 0;

        virtual void OnSelfBeginTalking() = 0;
        virtual void OnSelfEndTalking() = 0;
        virtual void OnSelfKeepTalking() = 0;
        virtual void OnOthersBeginTalking(AVROOMAPI_UserInfo& user) = 0;
        virtual void OnOthersEndTalking(AVROOMAPI_UserInfo& user) = 0;
        virtual void OnOthersKeepTalking(AVROOMAPI_UserInfo& user) = 0;

        virtual void OnDisconnected(int nErrorCode) = 0;
        virtual void OnError(int nErrorCode, const char* psDesc) = 0;
        virtual void OnKickOut(unsigned int nReason, const char* psMsg) = 0;

        virtual void OnAuxCallback(unsigned char *pData, int *pDataLen, int *pSampleRate, int *pNumChannels, int *pBitDepth) = 0;

        virtual void OnRecorderCallback(unsigned char * pData, unsigned int nDataLen, int nSsampleRate, int nChannels, int nBitDepth) = 0;
    };



    ZEGOAVROOMKIT_API bool AVROOMAPI_InitializeZegoAVRoomKit();
    ZEGOAVROOMKIT_API void AVROOMAPI_UninitializeZegoAVRoomKit();

    ZEGOAVROOMKIT_API void AVROOMAPI_SetTestServer(const char* psIp, int nPort, bool bEnable);
    ZEGOAVROOMKIT_API bool AVROOMAPI_SetAppInfo(const unsigned int uiAppID, const unsigned char* szAppSignature, const unsigned int nSignatureLen);

    ZEGOAVROOMKIT_API bool AVROOMAPI_GetInRoom(const unsigned int uiRoomKey, const char* psUserID, const char* psUserName, const int nNetType);
    ZEGOAVROOMKIT_API void AVROOMAPI_LeaveRoom();
    ZEGOAVROOMKIT_API void AVROOMAPI_GetRoomUsers(AVROOMAPI_UserInfo* pUserList,  unsigned int* npUserCount); //以pUserList=NULL调用获得个数
    ZEGOAVROOMKIT_API unsigned int AVROOMAPI_GetRoomKey();

    ZEGOAVROOMKIT_API void AVROOMAPI_EnableSpeaker(bool bEnable);
    ZEGOAVROOMKIT_API void AVROOMAPI_EnableMic(bool bEnable);
    
    ZEGOAVROOMKIT_API void AVROOMAPI_SetBluetoothOn(bool bEnable);
    
    ZEGOAVROOMKIT_API bool AVROOMAPI_GetSpeakerState();
    ZEGOAVROOMKIT_API bool AVROOMAPI_GetMicState();

    ZEGOAVROOMKIT_API __int64 AVROOMAPI_SendBroadcastTextMsg(const char* psMsg);
    ZEGOAVROOMKIT_API void AVROOMAPI_SetCallback(AVROOMAPI_IZegoAVRoomCallback* pCallback);
    ZEGOAVROOMKIT_API void AVROOMAPI_Relogin();

    ZEGOAVROOMKIT_API void AVROOMAPI_PauseAudio();
    ZEGOAVROOMKIT_API void AVROOMAPI_ResumeAudio();
    ZEGOAVROOMKIT_API int AVROOMAPI_GetAudioState(); // Audio状态  0:未启动,1:开启,2:暂停
    ZEGOAVROOMKIT_API void AVROOMAPI_SetHeadphoneOn(bool bEnable);
    ZEGOAVROOMKIT_API void AVROOMAPI_SetKeepInBackgroudTimeWhenDisconnect(int nSecond);
    ZEGOAVROOMKIT_API void AVROOMAPI_EnableAux(bool bEnable);
    ZEGOAVROOMKIT_API void AVROOMAPI_EnableRecorder(bool bEnable);
	ZEGOAVROOMKIT_API void SetAuxVolume(int volume);


#ifdef __cplusplus
}
#endif

#endif
