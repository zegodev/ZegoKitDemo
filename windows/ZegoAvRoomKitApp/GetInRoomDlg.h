// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CGetInRoomDlg : public CDialogImpl<CGetInRoomDlg>
{
public:
	enum { IDD = IDD_GETINROOM };

	BEGIN_MSG_MAP(CGetInRoomDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        COMMAND_ID_HANDLER(IDC_BUTTON_GETINROOM, GetInRoomCommandHandler)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

    CGetInRoomDlg()
    {
        m_uiRoomKey = 0;
    }

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
        m_combRoom.Attach(GetDlgItem(IDC_COMBO_ROOM));
        m_editId.Attach(GetDlgItem(IDC_EDIT_ID));
        m_editName.Attach(GetDlgItem(IDC_EDIT_NAME));
        m_btnGetInRoom.Attach(GetDlgItem(IDC_BUTTON_GETINROOM));

        m_combRoom.AddString(L"900000");
        m_combRoom.AddString(L"900001");
        m_combRoom.AddString(L"900002");
        m_combRoom.AddString(L"900003");
        m_combRoom.SetCurSel(0);

        __int64 now = time(0);

        TCHAR bufTime[64];
        _i64tot_s(now, bufTime, 64, 10);
        m_editId.SetWindowText(bufTime);

        m_editName.SetWindowText(L"laoning");

		CenterWindow(GetParent());
		return TRUE;
	}

    LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
    {
        EndDialog(0);
        return 0;
    }

    LRESULT GetInRoomCommandHandler(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
    {
        int nSel = m_combRoom.GetCurSel();
        if (nSel >= 0)
        {
            TCHAR szRoom[128];
            m_combRoom.GetLBText(nSel, szRoom);
            m_uiRoomKey = _ttoi(szRoom);

            m_editId.GetWindowText(m_strUserId);
            m_editName.GetWindowText(m_strUserName);
        }

        EndDialog(wID);
        return IDOK;
    }

    unsigned int GetRoomId()
    {
        return m_uiRoomKey;
    }

    CString GetUserId()
    {
        return m_strUserId;
    }

    CString GetUserName()
    {
        return m_strUserName;
    }

private:
    CComboBox       m_combRoom;
    CEdit           m_editId;
    CEdit           m_editName;
    CButton         m_btnGetInRoom;

    unsigned int    m_uiRoomKey;
    CString         m_strUserId;
    CString         m_strUserName;
};
