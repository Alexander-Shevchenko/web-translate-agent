// WebLingvo.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WebLingvo.h"
#include "HotkeyHandler.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{

    RegisterHotKey(
        NULL,
        1,
        MOD_CONTROL | MOD_NOREPEAT,
		VK_INSERT );  //0x42 is 'b'
 
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
			if (OpenClipboard(NULL)) 
			{
				// Retrieve the Clipboard data (specifying that 

				// we want ANSI text (via the CF_TEXT value).

				HANDLE hClipboardData = GetClipboardData(CF_UNICODETEXT);

				// Call GlobalLock so that to retrieve a pointer

				// to the data associated with the handle returned

				// from GetClipboardData.

				LPCWSTR pchData = (LPCWSTR)GlobalLock(hClipboardData);

				//MessageBox(NULL,pchData,NULL, MB_OK);

				GlobalUnlock(hClipboardData);

				// Finally, when finished I simply close the Clipboard

				// which has the effect of unlocking it so that other

				// applications can examine or modify its contents.

				CloseClipboard();
			}
		}
    } 
 
    return 0;
}
