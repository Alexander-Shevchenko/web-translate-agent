// WebLingvo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <string>
#include "Shellapi.h"
#include "WebDict.h"
#include "WebDictBrowser.h"

static HHOOK keyHook;
static std::wstring clipStr; // ClipBoard string
static std::wstring dictURL;

LRESULT CALLBACK LowLevelKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam);
static BOOL _CheckClipBoard(DWORD hkTime);
static void _buildURL(std::wstring& dictURL, const std::wstring& translWord);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	// We create the keyboard hook
	keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);

    if (!lpCmdLine || *lpCmdLine == '\0')
        dictURL = _T("http://www.lingvo.ua/ru/Search/en-ru/%s");
    else
        dictURL = lpCmdLine;

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {

    } 
 
    return 0;
}

// Build URL basing on copied phrase and URL-template
static void _buildURL(std::wstring& dictURL, const std::wstring& translWord)
{
    size_t szReplPos = dictURL.find(_T("%s"), 0);
    if (szReplPos == std::string::npos)
        dictURL += translWord;
    else
        dictURL.replace(szReplPos, _tcslen(_T("%s")), translWord);
}

// CForTheKidsDlg global functions
LRESULT CALLBACK LowLevelKeyboardProc(INT nCode, WPARAM wParam, LPARAM lParam)
{
    try
    {
		// Process double Ctrl-C & Ctrl-Ins hotkeys
        KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
        if (HC_ACTION == nCode
			&&
				( VK_INSERT == pkbhs->vkCode ||
				'c' == pkbhs->vkCode || 'C' == pkbhs->vkCode || // Accept both English
				'�' == pkbhs->vkCode || '�' == pkbhs->vkCode)	// and Russian 'C' key
			&&
			WM_KEYUP == wParam
			&&
			GetAsyncKeyState (VK_CONTROL) >> ((sizeof(SHORT) * 8) - 1)
			&&
			_CheckClipBoard(pkbhs->time))
        {
             std::wstring urlBld(dictURL);
             _buildURL(urlBld, clipStr);

			// Hack imitating Alt pressing so new browser window becomes top (issue with Chrome)
			// http://www.codeproject.com/Tips/76427/How-to-bring-window-to-top-with-SetForegroundWindo
			BYTE keyState[256] = {0};
			if (GetKeyboardState((LPBYTE)&keyState) &&
				!(keyState[VK_MENU] & 0x80))
			{
				keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY, 0);
			}

			ShellExecute(NULL, _TEXT("open"), urlBld.data(), NULL, NULL, SW_SHOWNORMAL);

			// Hack, part 2 (see above)
			if (GetKeyboardState((LPBYTE)&keyState) &&
				!(keyState[VK_MENU] & 0x80))
			{
				keybd_event(VK_MENU, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
			}
        }

		CallNextHookEx (keyHook, nCode, wParam, lParam);
    }
    catch(...)
    {

    }
    return 0;
}

static BOOL _CheckClipBoard(DWORD hkTime)
{
	static DWORD prevTime;

	BOOL bRet = FALSE;

	if (OpenClipboard(NULL)) 
	{
		HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);
		LPCWSTR pchData = (LPCWSTR)GlobalLock(hClipboardData);

		if(pchData)
		{
			if(!clipStr.empty() && !_tcscmp(clipStr.data(),pchData))
			{
				bRet = (hkTime - prevTime <= 1000) ? TRUE : FALSE; // Double hotkey considered just within 1 second or less time
			}
			else
			{
				clipStr = pchData;
			}
			prevTime = hkTime;
		}

		GlobalUnlock(hClipboardData);
		CloseClipboard();
	}

	return bRet;
}
