#include "stdafx.h"
#include "FreeRunningMode.h"


FreeRunningMode::FreeRunningMode()
{

}

void FreeRunningMode::Run()
{
	printf("Keyboard hook is running");
	HHOOK hhkLowLevelKybd;
	HHOOK hhkLowLevelMouse;
	hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, 0, 0);
	//hhkLowLevelMouse = SetWindowsHookEx(WH_MOUSE_LL, LowLevelMouseProc, 0, 0);
	// Keep this app running until we're told to stop
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {//this while loop keeps the hook
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hhkLowLevelKybd);
	//UnhookWindowsHookEx(hhkLowLevelKybd);
	keyboard.releaseAll();
}


/*LRESULT CALLBACK LowLevelMouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

	if (wParam == WM_RBUTTONUP || wParam == WM_LBUTTONUP)
		mouse.push(p->mouseData);
	if (wParam == WM_RBUTTONDOWN || wParam == WM_LBUTTONDOWN)
		mouse.release(p->mouseData);
	if (wParam == WM_MOUSEWHEEL)
		mouse.updateWheel(p->mouseData>>16);
	if (wParam == WM_MOUSEMOVE)
		mouse.updatePosition(p->pt.x,p->pt.y);

	return 1;
}*/
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			keyboard.push(p->vkCode);
		}
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			keyboard.release(p->vkCode);
		}
	}
	return 1;
}