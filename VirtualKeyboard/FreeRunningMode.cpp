#include "stdafx.h"
#include "FreeRunningMode.h"


FreeRunningMode::FreeRunningMode()
{
	hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
	// Keep this app running until we're told to stop
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {//this while loop keeps the hook
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hhkLowLevelKybd);
}


FreeRunningMode::~FreeRunningMode()
{
}

LRESULT CALLBACK FreeRunningMode::LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			printf("%c ", p->vkCode);
		}
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
		}
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	}
	return 1;
}