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
		if (msg.message == WM_QUIT)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hhkLowLevelKybd);
	//UnhookWindowsHookEx(hhkLowLevelMouse);
	keyboard.releaseAll();
}
LRESULT CALLBACK LowLevelProcNoOp(int nCode, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
LRESULT CALLBACK LowLevelMouseProcDual(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

	if (wParam == WM_RBUTTONUP)
		mouse.release(Mouse::MouseButtons::PPM);
	if (wParam == WM_LBUTTONUP)
		mouse.release(Mouse::MouseButtons::LPM);
	if (wParam == WM_RBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::PPM);
	if (wParam == WM_LBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::LPM);
	if (wParam == WM_MOUSEWHEEL) {
		mouse.updateWheel(p->mouseData);
	}
	if (wParam == WM_MOUSEMOVE) {
		mouse.updatePosition(p->pt.x, p->pt.y);
	}
	return 0;
}
LRESULT CALLBACK LowLevelKeyboardProcDual(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			if (p->vkCode == VK_F12) {
				PostQuitMessage(0);
			}
			else
				keyboard.push(p->vkCode);
		}
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			if (p->vkCode == VK_F12) {
				PostQuitMessage(0);
			}
			else
				keyboard.release(p->vkCode);
		}
	}
	return 0;
}
LRESULT CALLBACK LowLevelMouseProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	MSLLHOOKSTRUCT* p = (MSLLHOOKSTRUCT*)lParam;

	if (wParam == WM_RBUTTONUP)
		mouse.release(Mouse::MouseButtons::PPM);
	if (wParam == WM_LBUTTONUP)
		mouse.release(Mouse::MouseButtons::LPM);
	if (wParam == WM_RBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::PPM);
	if(wParam == WM_LBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::LPM);
	if (wParam == WM_MOUSEWHEEL) {
		mouse.updateWheel(p->mouseData);
		return 0;
	}
	if (wParam == WM_MOUSEMOVE) {
		mouse.updatePosition(p->pt.x, p->pt.y);
		return 0;
	}

	return 1;
}
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			if (p->vkCode == VK_F12) {
				PostQuitMessage(0);
			}
			else
			keyboard.push(p->vkCode);
		}
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			if (p->vkCode == VK_F12) {
				PostQuitMessage(0);
			}
			else
			keyboard.release(p->vkCode);
		}
	}
	return 1;
}