// ZegoAvRoomKitAppView.h : interface of the CZegoAvRoomKitAppView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ZegoAvRoomKit.h"
#include "AvRoomAppUtility.h"
#include <tuple>
#include <vector>
#include "GetInRoomDlg.h"

#define WM_ZEGOAVROOM_MESSAGE      WM_APP + 1
enum ZEGOAVROOM_MESSAGE_TYPE
{
    ZEGOAVROOM_MESSAGE_TYPE_Unknown,
    ZEGOAVROOM_MESSAGE_TYPE_OnGetInResult,
    ZEGOAVROOM_MESSAGE_TYPE_OnSendBroadcastTextMsgResult,
    ZEGOAVROOM_MESSAGE_TYPE_OnSendCustomBroadcastMsgResult,
    ZEGOAVROOM_MESSAGE_TYPE_OnRoomUsersUpdate,
    ZEGOAVROOM_MESSAGE_TYPE_OnRoomUserUpdateAll,
    ZEGOAVROOM_MESSAGE_TYPE_OnReceiveBroadcastTextMsg,
    ZEGOAVROOM_MESSAGE_TYPE_OnReceiveCustomBroadcastMsg,
    ZEGOAVROOM_MESSAGE_TYPE_OnSelfBeginTalking,
    ZEGOAVROOM_MESSAGE_TYPE_OnSelfEndTalking,
    ZEGOAVROOM_MESSAGE_TYPE_OnSelfKeepTalking,
    ZEGOAVROOM_MESSAGE_TYPE_OnOthersBeginTalking,
    ZEGOAVROOM_MESSAGE_TYPE_OnOthersEndTalking,
    ZEGOAVROOM_MESSAGE_TYPE_OnOthersKeepTalking,
    ZEGOAVROOM_MESSAGE_TYPE_OnDisconnected,
    ZEGOAVROOM_MESSAGE_TYPE_OnError,
    ZEGOAVROOM_MESSAGE_TYPE_OnKickOut,
    ZEGOAVROOM_MESSAGE_TYPE_OnAuxCallback,
    ZEGOAVROOM_MESSAGE_TYPE_OnRecorderCallback,
};


const unsigned char g_demo_app_signkey[] = { 0x91,0x93,0xcc,0x66,0x2a,0x1c,0xe,0xc1,
                                       0x35,0xec,0x71,0xfb,0x7,0x19,0x4b,0x38,
                                       0x15,0xf1,0x43,0xf5,0x7c,0xd2,0xb5,0x9a,
                                       0xe3,0xdd,0xdb,0xe0,0xf1,0x74,0x36,0xd };

const unsigned int g_demo_app_id = 1;


wchar_t  g_szBuffer[2*1024] = { 0 };


class CZegoAvRoomKitAppView : public CDialogImpl<CZegoAvRoomKitAppView>, public AVROOMAPI_IZegoAVRoomCallback
{
public:
	enum { IDD = IDD_ZEGOAVROOMKITAPP_FORM };

    CZegoAvRoomKitAppView()
    {
        m_uiAppId = 1;
        memcpy_s(m_arrSignature, 32, g_demo_app_signkey, 32);

        m_uiRoomKey = 0;
        m_nNetType = 0;
        m_bTryingGetInRoom = false;

        AVROOMAPI_SetCallback(this);
        AVROOMAPI_SetAppInfo(m_uiAppId, m_arrSignature, 32);
    }

    ~CZegoAvRoomKitAppView()
    {
        AVROOMAPI_SetCallback(NULL);
    }

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		return CWindow::IsDialogMessage(pMsg);
	}

	BEGIN_MSG_MAP(CZegoAvRoomKitAppView)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_ZEGOAVROOM_MESSAGE, OnZegoAvRoomMsgCome)
        COMMAND_ID_HANDLER(IDC_BUTTON_SEND, CommandHandler_SendMessage)
        COMMAND_ID_HANDLER(IDC_BUTTON_SOUND_INPUT, CommandHandler_Sound_Input)
        COMMAND_ID_HANDLER(IDC_BUTTON_SOUND_OUTPUT, CommandHandler_Sound_Output)
        COMMAND_ID_HANDLER(IDC_BUTTON_PAUSE, CommandHandler_Pause)
        COMMAND_ID_HANDLER(IDC_BUTTON_LEAVE, CommandHandler_Leave)
		COMMAND_ID_HANDLER(IDC_BUTTON_RELOGIN, CommandHandler_Relogin)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        m_editMessage.Attach(GetDlgItem(IDC_EDIT_MESSAGE));
        m_editSend.Attach(GetDlgItem(IDC_EDIT_INPUT));
        m_lbMember.Attach(GetDlgItem(IDC_LIST_MEMBER));
        m_btnSend.Attach(GetDlgItem(IDC_BUTTON_SEND));
        m_btnSoundInput.Attach(GetDlgItem(IDC_BUTTON_SOUND_INPUT));
        m_btnSoundOutput.Attach(GetDlgItem(IDC_BUTTON_SOUND_OUTPUT));
        m_btnPause.Attach(GetDlgItem(IDC_BUTTON_PAUSE));
		m_btnRelogin.Attach(GetDlgItem(IDC_BUTTON_RELOGIN));

        m_editSend.SetFocus();

        return TRUE;
    }

	LRESULT CommandHandler_Relogin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
	{
		AVROOMAPI_Relogin();
		return 0;
	}

    LRESULT CommandHandler_SendMessage(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        bHandled = TRUE;

        CString strMessage;
        m_editSend.GetWindowText(strMessage);

        m_editSend.SetFocus();

        if (m_uiRoomKey == 0 || m_bTryingGetInRoom)
        {
            MessageBox(_T("还没进入房间，不能发生消息"), _T("提示"));
            return 0;
        }

        if (!strMessage.IsEmpty())
        {
            std::string utf8Msg = AvRoomApp::Utility::WStringToUTF8(strMessage);
            long long result = AVROOMAPI_SendBroadcastTextMsg(utf8Msg.c_str());
            if (result > 0)
            {
                AddSelfMessage(utf8Msg.c_str());
            }

            m_editSend.SetSelAll();
            m_editSend.Clear();
        }

        return 0;
    }

    LRESULT CommandHandler_Sound_Output(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        bool bSpeaker = AVROOMAPI_GetSpeakerState();
        bSpeaker = !bSpeaker;
        AVROOMAPI_EnableSpeaker(bSpeaker);

        if (bSpeaker)
        {
            m_btnSoundOutput.SetWindowText(_T("关扬声器"));
        }
        else
        {
            m_btnSoundOutput.SetWindowText(_T("开扬声器"));
        }

        bHandled = TRUE;
        return 0;
    }

    LRESULT CommandHandler_Sound_Input(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        bool bMic = AVROOMAPI_GetMicState();
        bMic = !bMic;
        AVROOMAPI_EnableMic(bMic);

        if (bMic)
        {
            m_btnSoundInput.SetWindowText(_T("关Mic"));
        }
        else
        {
            m_btnSoundInput.SetWindowText(_T("开Mic"));
        }

        bHandled = TRUE;
        return 0;
    }

    LRESULT CommandHandler_Pause(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        int nAudioState = AVROOMAPI_GetAudioState();
        if (nAudioState == 0)
        {
            MessageBox(_T("语音未启动"), _T("提示"));
        }
        else if (nAudioState == 1)
        {
            AVROOMAPI_PauseAudio();
            m_btnPause.SetWindowText(_T("恢复"));
        }
        else if (nAudioState == 2)
        {
            AVROOMAPI_ResumeAudio();
            m_btnPause.SetWindowText(_T("暂停"));
        }

        bHandled = TRUE;
        return 0;
    }

    LRESULT CommandHandler_Leave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& bHandled)
    {
        AVROOMAPI_LeaveRoom();

        int nMemberCount = m_lbMember.GetCount();
        for (int i = 0; i < nMemberCount; ++i)
        {
            m_lbMember.DeleteString(0);
        }

        GetParent().SetWindowText(L"ZegoAvRoomKitApp");

        m_editMessage.SetReadOnly(FALSE);
        m_editMessage.SetSelAll();
        m_editMessage.Clear();
        m_editMessage.SetReadOnly();

        m_uiRoomKey = 0;

        bHandled = TRUE;


		::ShowWindow(GetParent().m_hWnd, SW_HIDE);

		CGetInRoomDlg dlgGetInRoom;
		int nResult = dlgGetInRoom.DoModal();
		if (IDC_BUTTON_GETINROOM == nResult)
		{
			::ShowWindow(GetParent().m_hWnd, SW_SHOW);
			GetInRoom(dlgGetInRoom.GetRoomId(), dlgGetInRoom.GetUserId(), dlgGetInRoom.GetUserName(), AVROOMAPI_NT_LINE);
			return 0;
		}

		GetParent().DestroyWindow();
        return 0;
    }

    bool GetInRoom(const unsigned int uiRoomKey, CString strUserID, CString strUserName, const int nNetType)
    {
        if (uiRoomKey == 0)
        {
            return false;
        }

        if (m_bTryingGetInRoom)
        {
            MessageBox(_T("正在进入房间，请等待。"), _T("提示"));
            return false;
        }

        if (uiRoomKey == m_uiRoomKey && strUserID == m_strUserID && strUserName == strUserName && nNetType == m_nNetType)
        {
            MessageBox(_T("你进入到这个房间，无需重复进入"), _T("提示"));
            return true;
        }

        m_uiRoomKey = uiRoomKey;
        m_strUserID = strUserID;
        m_strUserName = strUserName;
        m_nNetType = nNetType;

        m_bTryingGetInRoom = true;

        CString strTile;
        strTile.Format(L"ZegoAvRoomKitApp--正在进入房间%d", uiRoomKey);
        GetParent().SetWindowText(strTile);

        int nMemberCount = m_lbMember.GetCount();
        for (int i =0; i < nMemberCount; ++i)
        {
            m_lbMember.DeleteString(0);
        }

        return AVROOMAPI_GetInRoom(uiRoomKey, AvRoomApp::Utility::WStringToUTF8(strUserID).c_str(), AvRoomApp::Utility::WStringToUTF8(strUserName).c_str(), nNetType);
    }

    void SendMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
    {
        if (::IsWindow(m_hWnd) && m_uiRoomKey != 0)
        {
            CDialogImpl<CZegoAvRoomKitAppView>::SendMessage(message, wParam, lParam);
        }
    }

	void PosMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0)
	{
		if (::IsWindow(m_hWnd) && m_uiRoomKey != 0)
		{
			CDialogImpl<CZegoAvRoomKitAppView>::PostMessageW(message, wParam, lParam);
		}
	}

    virtual void OnGetInResult(int nResult, unsigned int uRoomKey)
    {
        auto* param = new std::tuple<int, unsigned int>(nResult, uRoomKey);
		PosMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnGetInResult, (LPARAM)param);
    }

    virtual void OnSendBroadcastTextMsgResult(int nResult, const char* psMsg, unsigned __int64 nMsgSeq)
    {
		std::string msg;
		if (psMsg)
		{
			msg = psMsg;
		}

        auto* param = new std::tuple<int, std::string, unsigned __int64>(nResult, msg, nMsgSeq);
        PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnSendBroadcastTextMsgResult, (LPARAM)param);
    }

    virtual void OnSendCustomBroadcastMsgResult(int nResult, const unsigned char* psMsg, unsigned int nMsgLen, unsigned __int64 nMsgSeq)
    {
		std::vector<unsigned char> vecMsg;
		if (nMsgLen > 0)
		{
			vecMsg.assign(psMsg, psMsg + nMsgLen);
		}
		auto* param = new std::tuple<int, std::vector<unsigned char>, unsigned __int64>(nResult, vecMsg, nMsgSeq);
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnSendCustomBroadcastMsgResult, (LPARAM)param);
    }

    virtual void OnRoomUsersUpdate(AVROOMAPI_UserInfo* pNewUsersList, unsigned int nNewUserCount, AVROOMAPI_UserInfo* pRemovedUsersList, unsigned int nRemovedUserCount)
    {
		std::vector<AVROOMAPI_UserInfo*> vecNew;
		std::vector<AVROOMAPI_UserInfo*> vecRemoved;

		for (int i = 0; i < nNewUserCount; ++i)
		{
			AVROOMAPI_UserInfo* p = new AVROOMAPI_UserInfo;
			memcpy(p, &pNewUsersList[i], sizeof(AVROOMAPI_UserInfo));
			vecNew.push_back(p);
		}

		for (int i = 0; i < nRemovedUserCount; ++i)
		{
			AVROOMAPI_UserInfo* p = new AVROOMAPI_UserInfo;
			memcpy(p, &pRemovedUsersList[i], sizeof(AVROOMAPI_UserInfo));
			vecRemoved.push_back(p);
		}

		auto* param = new std::tuple<std::vector<AVROOMAPI_UserInfo*>, std::vector<AVROOMAPI_UserInfo*> >(vecNew, vecRemoved);
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnRoomUsersUpdate, (LPARAM)param);
    }

    virtual void OnRoomUserUpdateAll(AVROOMAPI_UserInfo* pUsersList, unsigned int nUserCount)
    {
		std::vector<AVROOMAPI_UserInfo*>* vecAll = new std::vector<AVROOMAPI_UserInfo*>;
		for (int i = 0; i < nUserCount; ++i)
		{
			AVROOMAPI_UserInfo* p = new AVROOMAPI_UserInfo;
			memcpy(p, &pUsersList[i], sizeof(AVROOMAPI_UserInfo));
			(*vecAll).push_back(p);
		}

		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnRoomUserUpdateAll, (LPARAM)vecAll);
    }

    virtual void OnReceiveBroadcastTextMsg(AVROOMAPI_TextMsg& msg)
    {
		AVROOMAPI_TextMsg* pMsg = new AVROOMAPI_TextMsg;
		memcpy(pMsg, &msg, sizeof(AVROOMAPI_TextMsg));

		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnReceiveBroadcastTextMsg, (LPARAM)pMsg);
    }

    virtual void OnReceiveCustomBroadcastMsg(AVROOMAPI_CustomMsg& msg)
    {
		AVROOMAPI_CustomMsg* pMsg = new AVROOMAPI_CustomMsg;
		memcpy(pMsg, &msg, sizeof(AVROOMAPI_CustomMsg));
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnReceiveCustomBroadcastMsg, (LPARAM)pMsg);
    }

    virtual void OnSelfBeginTalking()
    {
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnSelfBeginTalking);
    }

    virtual void OnSelfEndTalking()
    {
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnSelfEndTalking);
    }

    virtual void OnSelfKeepTalking()
    {
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnSelfKeepTalking);
    }

    virtual void OnOthersBeginTalking(AVROOMAPI_UserInfo& user)
    {
		AVROOMAPI_UserInfo* pUser = new AVROOMAPI_UserInfo;
		memcpy(pUser, &user, sizeof(AVROOMAPI_UserInfo));
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnOthersBeginTalking, (LPARAM)pUser);
    }

    virtual void OnOthersEndTalking(AVROOMAPI_UserInfo& user)
    {
		AVROOMAPI_UserInfo* pUser = new AVROOMAPI_UserInfo;
		memcpy(pUser, &user, sizeof(AVROOMAPI_UserInfo));
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnOthersEndTalking, (LPARAM)pUser);
    }

    virtual void OnOthersKeepTalking(AVROOMAPI_UserInfo& user)
    {
		AVROOMAPI_UserInfo* pUser = new AVROOMAPI_UserInfo;
		memcpy(pUser, &user, sizeof(AVROOMAPI_UserInfo));
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnOthersKeepTalking, (LPARAM)pUser);
    }

    virtual void OnDisconnected(int nErrorCode)
    {
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnDisconnected, (LPARAM)nErrorCode);
    }

    virtual void OnError(int nErrorCode, const char* psDesc)
    {
		std::string msg;
		if (psDesc)
		{
			msg = psDesc;
		}
		auto* param = new std::tuple<unsigned int, std::string>(nErrorCode, msg);

		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnError, (LPARAM)param);
    }

    virtual void OnKickOut(unsigned int nReason, const char* psMsg)
    {
		std::string msg;
		if (psMsg)
		{
			msg = psMsg;
		}
        auto* param = new std::tuple<unsigned int, std::string>(nReason, msg);
		PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnKickOut, (LPARAM)param);
    }

    virtual void OnAuxCallback(unsigned char *pData, int *pDataLen, int *pSampleRate, int *pNumChannels, int *pBitDepth)
    {
        //auto param = std::make_tuple(pData, pDataLen, pSampleRate, pNumChannels, pBitDepth);
		//PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnAuxCallback, (LPARAM)param);
    }

    virtual void OnRecorderCallback(unsigned char * pData, unsigned int nDataLen, int nSsampleRate, int nChannels, int nBitDepth)
    {
        //auto param = std::make_tuple(pData, nDataLen, nSsampleRate, nChannels, nBitDepth);
		//PostMessage(WM_ZEGOAVROOM_MESSAGE, ZEGOAVROOM_MESSAGE_TYPE_OnRecorderCallback, (LPARAM)param);
    }

    LRESULT OnZegoAvRoomMsgCome(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        bHandled = TRUE;
        int nMsg_type = (int)wParam;

        switch (nMsg_type)
        {
        case ZEGOAVROOM_MESSAGE_TYPE_OnGetInResult:
        {
            m_bTryingGetInRoom = false;

            std::tuple<int, unsigned int>* pParam = (std::tuple<int, unsigned int>*)lParam;
            int nResult = std::get<0>(*pParam);
            unsigned int uRoomKey = std::get<1>(*pParam);

			int nMemberCount = m_lbMember.GetCount();
			for (int i = 0; i < nMemberCount; ++i)
			{
				m_lbMember.DeleteString(0);
			}


            CString strTile;

            if (nResult != 0)
            {
                m_uiRoomKey = 0;
                MessageBox(_T("进入房间失败。"), _T("提示"));
                strTile.Format(L"ZegoAvRoomKitApp--进入房间%d失败", uRoomKey);
            }
            else
            {
                strTile.Format(L"ZegoAvRoomKitApp--进入房间%d成功", uRoomKey);

                if (AVROOMAPI_GetSpeakerState())
                {
                    m_btnSoundOutput.SetWindowText(_T("关扬声器"));
                }
                else
                {
                    m_btnSoundOutput.SetWindowText(_T("开扬声器"));
                }

                if (AVROOMAPI_GetMicState())
                {
                    m_btnSoundInput.SetWindowText(_T("关Mic"));
                }
                else
                {
                    m_btnSoundInput.SetWindowText(_T("开Mic"));
                }

                int nAudioState = AVROOMAPI_GetAudioState();
                if (nAudioState == 0 || nAudioState == 1)
                {
                    m_btnPause.SetWindowText(_T("暂停"));
                }
                else
                {
                    m_btnPause.SetWindowText(_T("恢复"));
                }
            }

            GetParent().SetWindowText(strTile);

			//for test 500
			AVROOMAPI_EnableAux(true);
			AVROOMAPI_EnableRecorder(true);

			//AVROOMAPI_EnableAux(false);
			//AVROOMAPI_EnableRecorder(false);
			//AVROOMAPI_PauseAudio();

			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnSendBroadcastTextMsgResult:
        {
            auto* pParam = (std::tuple<int, std::string, unsigned __int64>*)lParam;
            int nResult = std::get<0>(*pParam); 
            std::string psMsg = std::get<1>(*pParam);
            unsigned __int64 nMsgSeq = std::get<2>(*pParam);

            if (nResult == 0)
            {
                AddSelfMessage(psMsg.c_str());
            }

			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnSendCustomBroadcastMsgResult:
        {
   //         auto* pParam = (std::tuple<int, std::vector<unsigned char>, unsigned __int64>*)lParam;
   //         int nResult = std::get<0>(*pParam);
			//std::vector<unsigned char> vecMsg = std::get<1>(*pParam);
   //         unsigned __int64 nMsgSeq = std::get<2>(*pParam);

			//delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnRoomUsersUpdate:
        {
            auto* pParam = (std::tuple<std::vector<AVROOMAPI_UserInfo*>, std::vector<AVROOMAPI_UserInfo*> >*)lParam;
			std::vector<AVROOMAPI_UserInfo*> vecNew = std::get<0>(*pParam);
			std::vector<AVROOMAPI_UserInfo*> vecRemove = std::get<1>(*pParam);


            for (unsigned int i = 0; i < vecNew.size(); ++i)
            {
                wsprintfW(g_szBuffer, L"%s-%s", AvRoomApp::Utility::UTF8ToWString(vecNew[i]->arrName).c_str(),
                    AvRoomApp::Utility::UTF8ToWString(vecNew[i]->arrId).c_str());

                m_lbMember.AddString(g_szBuffer);

				delete vecNew[i];
            }

            for (unsigned int i = 0; i < vecRemove.size(); ++i)
            {
                wsprintfW(g_szBuffer, L"%s-%s", AvRoomApp::Utility::UTF8ToWString(vecRemove[i]->arrName).c_str(),
                    AvRoomApp::Utility::UTF8ToWString(vecRemove[i]->arrId).c_str());

                int item = m_lbMember.FindString(-1, g_szBuffer);
                m_lbMember.DeleteString(item);

				delete vecRemove[i];
            }

			delete pParam;
        }
        break;


        case ZEGOAVROOM_MESSAGE_TYPE_OnRoomUserUpdateAll:
        {
            auto* pParam = (std::vector<AVROOMAPI_UserInfo*>*)lParam;

			for (unsigned int i = 0; i < (*pParam).size(); ++i)
			{
				wsprintfW(g_szBuffer, L"%s-%s", AvRoomApp::Utility::UTF8ToWString((*pParam)[i]->arrName).c_str(),
					AvRoomApp::Utility::UTF8ToWString((*pParam)[i]->arrId).c_str());

				m_lbMember.AddString(g_szBuffer);

				delete (*pParam)[i];
			}
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnReceiveBroadcastTextMsg:
        {
            auto* pParam = (AVROOMAPI_TextMsg*)lParam;
            AddOtherMessage(*pParam);
			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnReceiveCustomBroadcastMsg:
        {
            auto* pParam = (AVROOMAPI_CustomMsg*)lParam;
			delete pParam;
        }
        break;


        case ZEGOAVROOM_MESSAGE_TYPE_OnSelfBeginTalking:
        {
            CString strTile;
            strTile.Format(L"ZegoAvRoomKitApp--我开始说话");
            GetParent().SetWindowText(strTile);
        }
        break;


        case ZEGOAVROOM_MESSAGE_TYPE_OnSelfEndTalking:
        {
            CString strTile;
            strTile.Format(L"ZegoAvRoomKitApp--我结束说话");
            GetParent().SetWindowText(strTile);
        }
        break;


        case ZEGOAVROOM_MESSAGE_TYPE_OnSelfKeepTalking:
        {
            CString strTile;
            strTile.Format(L"ZegoAvRoomKitApp--我在说话");
            GetParent().SetWindowText(strTile);
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnOthersBeginTalking:
        {
            auto* pParam = (AVROOMAPI_UserInfo*)lParam;
            CString strTile;
			std::wstring strName = AvRoomApp::Utility::UTF8ToWString(pParam->arrName);
			std::wstring strId = AvRoomApp::Utility::UTF8ToWString(pParam->arrId);

            strTile.Format(L"ZegoAvRoomKitApp--%s--%s开始说话", strName.c_str(), strId.c_str());
            GetParent().SetWindowText(strTile);
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnOthersEndTalking:
        {
            auto* pParam = (AVROOMAPI_UserInfo*)lParam;
            CString strTile;
			std::wstring strName = AvRoomApp::Utility::UTF8ToWString(pParam->arrName);
			std::wstring strId = AvRoomApp::Utility::UTF8ToWString(pParam->arrId);

            strTile.Format(L"ZegoAvRoomKitApp--%s--%s结束说话", strName.c_str(), strId.c_str());
            GetParent().SetWindowText(strTile);

			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnOthersKeepTalking:
        {
            auto* pParam = (AVROOMAPI_UserInfo*)lParam;
            CString strTile;
			std::wstring strName = AvRoomApp::Utility::UTF8ToWString(pParam->arrName);
			std::wstring strId = AvRoomApp::Utility::UTF8ToWString(pParam->arrId);
            strTile.Format(L"ZegoAvRoomKitApp--%s--%s在说话", strName.c_str(), strId.c_str());
            GetParent().SetWindowText(strTile);

			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnDisconnected:
        {
            int nErrorCode = (int)lParam;

            CString strTile;
            strTile.Format(L"ZegoAvRoomKitApp--连接断开，错误码是%d", nErrorCode);
            GetParent().SetWindowText(strTile);
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnError:
        {
			auto* pParam = (std::tuple<unsigned int, std::string>*)lParam;
			unsigned int nErrorCode = std::get<0>(*pParam);
			std::string desc = std::get<1>(*pParam);

            CString strTile;
			std::wstring strDesc = AvRoomApp::Utility::UTF8ToWString(desc.c_str());

            strTile.Format(L"ZegoAvRoomKitApp--出错了，%s。错误码是%d, ", strDesc.c_str(), nErrorCode);
            GetParent().SetWindowText(strTile);

			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnKickOut:
        {
			auto* pParam = (std::tuple<unsigned int, std::string>*)lParam;
            unsigned int nReason = std::get<0>(*pParam);
            std::string msg = std::get<1>(*pParam);

            CString strTile;
			std::wstring strMsg = AvRoomApp::Utility::UTF8ToWString(msg.c_str());

            strTile.Format(L"ZegoAvRoomKitApp--被踢下线，%s。原因是%d, ", strMsg.c_str(), nReason);
            GetParent().SetWindowText(strTile);

			delete pParam;
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnAuxCallback:
        {
            auto* pParam = (std::tuple<unsigned char*, int*, int*, int*, int*>*)lParam;
            unsigned char *pData = std::get<0>(*pParam);
            int *pDataLen = std::get<1>(*pParam);
            int *pSampleRate = std::get<2>(*pParam);
            int *pNumChannels = std::get<3>(*pParam); 
            int *pBitDepth = std::get<4>(*pParam);
        }
        break;

        case ZEGOAVROOM_MESSAGE_TYPE_OnRecorderCallback:
        {
            auto* pParam = (std::tuple<unsigned char *, unsigned int, int, int, int>*)lParam;
            unsigned char * pData = std::get<0>(*pParam);
            unsigned int nDataLen = std::get<1>(*pParam); 
            int nSsampleRate = std::get<2>(*pParam);
            int nChannels = std::get<3>(*pParam); 
            int nBitDepth = std::get<4>(*pParam);
        }
        break;

        default:
            break;
        }

        return 0;
    }

protected:
    void AddSelfMessage(const char* psMsg)
    {
        std::wstring strMsg = AvRoomApp::Utility::UTF8ToWString(psMsg);
        strMsg += L"\r\n\r\n";

        time_t now = time(NULL);
        tm *pTime = localtime(&now);

        wsprintfW(g_szBuffer, L"%s-%s   %d:%d:%d\r\n", m_strUserName, m_strUserID, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);


        AddMessage(g_szBuffer, strMsg.c_str());
    }

    void AddOtherMessage(AVROOMAPI_TextMsg& aTextMsg)
    {
        std::wstring strMsg = AvRoomApp::Utility::UTF8ToWString(aTextMsg.arrMsg);
        strMsg += L"\r\n\r\n";

        time_t now = time(NULL);
        tm *pTime = localtime(&now);

        wsprintfW(g_szBuffer, L"%s-%s   %d:%d:%d\r\n", AvRoomApp::Utility::UTF8ToWString(aTextMsg.xSendUser.arrName).c_str(), AvRoomApp::Utility::UTF8ToWString(aTextMsg.xSendUser.arrId).c_str(), pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
    
        AddMessage(g_szBuffer, strMsg.c_str());
    }

    void AddMessage(const wchar_t* psSendAndTime, const wchar_t* psMessage)
    {
        int nOldLines = m_editMessage.GetLineCount();

        CHARFORMATW cf;
        cf.cbSize = sizeof(CHARFORMAT);
        cf.dwMask = CFM_COLOR | CFM_BOLD | CFM_SIZE;
        cf.dwEffects = (unsigned long)~(CFE_AUTOCOLOR | CFE_BOLD);
        cf.crTextColor = RGB(0, 128, 0);
        cf.yHeight = 12 * 15;
        wcscpy(cf.szFaceName, L"宋体");

        int nCurTextLen = m_editMessage.GetWindowTextLength();
        m_editMessage.SetSel(-1, -1);
        m_editMessage.SetSelectionCharFormat(cf);
        m_editMessage.ReplaceSel(psSendAndTime);
        

        cf.crTextColor = RGB(128, 128, 128);
        cf.yHeight = 12 * 15;

        nCurTextLen = m_editMessage.GetWindowTextLength();
        m_editMessage.SetSel(-1, -1);
        m_editMessage.SetSelectionCharFormat(cf);
        m_editMessage.ReplaceSel(psMessage);

        int nNewLines = m_editMessage.GetLineCount();
        int nScroll = nNewLines - nOldLines;
        m_editMessage.LineScroll(nScroll);
    }

private:
    CRichEditCtrl   m_editMessage;
    CListBox        m_lbMember;
    CEdit           m_editSend;
    CButton         m_btnSend;
    CButton         m_btnSoundInput;
    CButton         m_btnSoundOutput;
    CButton         m_btnPause;
	CButton			m_btnRelogin;

    bool            m_bTryingGetInRoom;
    unsigned int    m_uiAppId;
    unsigned char   m_arrSignature[32];
    unsigned int    m_uiRoomKey;
    CString         m_strUserID;
    CString         m_strUserName;
    int             m_nNetType;
};
