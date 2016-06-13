// ZegoAvRoomKitApp.cpp : main source file for ZegoAvRoomKitApp.exe
//

#include "stdafx.h"

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atltypes.h>

#include "resource.h"

#include "ZegoAvRoomKitAppView.h"
#include "aboutdlg.h"
#include "GetInRoomDlg.h"
#include "MainFrm.h"


#include "ZegoAvRoomKit.h"

CAppModule _Module;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

    CRect recWin(0, 0, 1130, 900);
    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	if(wndMain.CreateEx(NULL, &recWin, style) == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);
    wndMain.CenterWindow();

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

    LoadLibrary(_T("RICHED20.DLL"));

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

    AVROOMAPI_InitializeZegoAVRoomKit();

	int nRet = Run(lpstrCmdLine, nCmdShow);

    AVROOMAPI_UninitializeZegoAVRoomKit();

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
