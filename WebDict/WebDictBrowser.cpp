#include "stdafx.h"

// Find browser window to activate
BOOL CALLBACK _EnumWindowsProc(HWND hwnd, LPARAM hFoundWnd)
{
	WINDOWINFO wi = {0};
	wi.cbSize = sizeof(WINDOWINFO);
	if (GetWindowInfo(hwnd, &wi) && WS_VISIBLE & wi.dwStyle)
	{
		_TCHAR sPath[MAX_PATH] = {'\0'};
		if (!GetWindowModuleFileName(hwnd, sPath, MAX_PATH))
			return TRUE;

		_tcsupr(sPath);

		if (NULL != _tcsstr(sPath, _TEXT("CHROME.EXE")) ||
			NULL != _tcsstr(sPath, _TEXT("WEBDICT.EXE"))) // Sometimes GetWindowModuleFileName says Chrome window is owned by WebDict 8-/
		{
			int len = GetWindowTextLength(hwnd);
			if (!len)
				return TRUE;

			_TCHAR *sTitle = new _TCHAR[len+1];

			if (!GetWindowText(hwnd, sTitle, len+1) ||
				!_tcsupr(sTitle) ||
				!_tcsstr(sTitle, _TEXT("CHROME")))
			{
				delete sTitle;
				return TRUE;
			}

			delete sTitle;

			if (*(HWND*)hFoundWnd && *(HWND*)hFoundWnd != hwnd)
			{
				// More than one suitable window found, give up
				*(HWND*)hFoundWnd = NULL;
				return FALSE;
			}

			*(HWND*)hFoundWnd = hwnd;
		}
	}

	return TRUE;
}

void WDActiveBrowser()
{
	return;
	// TODO: Check that Chrome is our default browser

	HWND hFoundWnd = NULL;
	if (EnumWindows(_EnumWindowsProc, (LPARAM) &hFoundWnd) && hFoundWnd)
	{
		// Workaround for SetForegroundWindow by SHShadow
		// ()
		if(!::IsWindow(hFoundWnd)) return;
 
		BYTE keyState[256] = {0};
		//to unlock SetForegroundWindow we need to imitate Alt pressing
		if(::GetKeyboardState((LPBYTE)&keyState))
		{
			if(!(keyState[VK_MENU] & 0x80))
			{
				::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
			}
		}
 
		::SetForegroundWindow(hFoundWnd);
 
		if(::GetKeyboardState((LPBYTE)&keyState))
		{
			if(!(keyState[VK_MENU] & 0x80))
			{
				::keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
		}
	}
}
