#include "stdafx.h"
#include "FreeRunningMode.h"


Mouse mouse{};
HIDKeyboard hidkbd;
KeyboardTranslator tr;
Keyboard keyboard{ tr,hidkbd };

void FreeRunningMode::Run()
{
	if (isMouseInUse())
	{
		ResetMousePosition();
	}
	HHOOK hhkLowLevelKybd = SetWindowsHookEx(WH_KEYBOARD_LL, keyhookproc, 0, 0);
	HHOOK hhkLowLevelMouse = SetWindowsHookEx(WH_MOUSE_LL, mousehookproc, 0, 0);
	// Keep this app running until we're told to stop
	MSG msg;
	while (!GetMessage(&msg, NULL, NULL, NULL)) {//this while loop keeps the hook
		if (msg.message == WM_QUIT)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	UnhookWindowsHookEx(hhkLowLevelKybd);
	UnhookWindowsHookEx(hhkLowLevelMouse);
	keyboard.releaseAll();
}

bool FreeRunningMode::isMouseInUse() const
{
	return (mousehookproc != &LowLevelProcNoOp);
}

void FreeRunningMode::ResetMousePosition() const
{
	for (int i = 0; i < 50; i++)
	{
		mouse.move(-127,-127,0);
	}
	SetCursorPos(0, 0);
}

LRESULT CALLBACK LowLevelProcNoOp(int nCode, WPARAM wParam, LPARAM lParam)
{
	auto state=GetAsyncKeyState(VK_F9);
	auto alt = GetAsyncKeyState(VK_MENU);
	if(state&&alt)
		PostQuitMessage(0);
	return 0;
}
LRESULT CALLBACK LowLevelMouseProcDual(int nCode, WPARAM wParam, LPARAM lParam)
{
	MSLLHOOKSTRUCT* p = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

	if (wParam == WM_RBUTTONUP)
		mouse.release(Mouse::MouseButtons::PPM_);
	if (wParam == WM_LBUTTONUP)
		mouse.release(Mouse::MouseButtons::LPM_);
	if (wParam == WM_RBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::PPM_);
	if (wParam == WM_LBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::LPM_);
	if (wParam == WM_MOUSEWHEEL) {
		mouse.updateWheel(p->mouseData);
	}
	if (wParam == WM_MOUSEMOVE) {
		mouse.updatePosition(p->pt.x, p->pt.y);
	}
	return 0;
}

BOOL ShouldTurnOff(int code, int last)
{	
	if (code == VK_F9 && (last==VK_LMENU||last==VK_RMENU))
		return true;
	else
		return false;
}

FreeRunningMode::FreeRunningMode(BlockingQueue<std::shared_ptr<IMessage>> &que, boost::program_options::variables_map const& vm) :bque{que}, printer{ que }
{
	mouse.SetScale(vm["scale"].as<double>());
	auto con1 = hidkbd.connect(std::bind(&Printer::printer, &printer, std::placeholders::_1));
	auto con2 = mouse.connect(std::bind(&Printer::mousePrinter, &printer, std::placeholders::_1));
	keyhookproc = LowLevelProcNoOp;
	mousehookproc = LowLevelProcNoOp;
	if (vm.count("verbose"))
	{
		con1.disconnect();
		con2.disconnect();
		hidkbd.connect(std::bind(&Printer::verbosePrinter, &printer, std::placeholders::_1));
		mouse.connect(std::bind(&Printer::verboseMousePrinter, &printer, std::placeholders::_1));
	}
	auto mode = vm["mode"].as<std::string>();
	auto device = vm["device"].as<std::string>();
	if (mode.find('d')!=std::string::npos)
	{
		if (device.find('m') != std::string::npos)
			mousehookproc = &LowLevelMouseProcDual;
		if (device.find('k') != std::string::npos)
			keyhookproc = &LowLevelKeyboardProcDual;
	}
	else if (mode.find('s') != std::string::npos) {
		if (device.find('m') != std::string::npos)
		{
			mousehookproc = &LowLevelMouseProc;
		}
		if (device.find('k') != std::string::npos)
		{
			keyhookproc = &LowLevelKeyboardProc;
		}
	}
}

FreeRunningMode::FreeRunningMode(const FreeRunningMode& r): bque{r.bque},printer{r.printer}
{
	keyhookproc = r.keyhookproc;
	mousehookproc = r.mousehookproc;
}

FreeRunningMode& FreeRunningMode::operator=(FreeRunningMode const & r)
{
	bque=r.bque;
	keyhookproc = r.keyhookproc;
	mousehookproc = r.mousehookproc;
	return *this;
}

LRESULT CALLBACK LowLevelKeyboardProcDual(int nCode, WPARAM wParam, LPARAM lParam)
{
	BOOL fEatKeystroke = FALSE;
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
	static int last;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			if (ShouldTurnOff(p->vkCode,last)) {
				PostQuitMessage(0);
			}
			else
				keyboard.push(p->vkCode);
			last = p->vkCode;
		}
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			if (ShouldTurnOff(p->vkCode,last)) {
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
	MSLLHOOKSTRUCT* p = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

	if (wParam == WM_RBUTTONUP)
		mouse.release(Mouse::MouseButtons::PPM_);
	if (wParam == WM_LBUTTONUP)
		mouse.release(Mouse::MouseButtons::LPM_);
	if (wParam == WM_RBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::PPM_);
	if(wParam == WM_LBUTTONDOWN)
		mouse.push(Mouse::MouseButtons::LPM_);
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
	PKBDLLHOOKSTRUCT p = reinterpret_cast<PKBDLLHOOKSTRUCT>(lParam);
	static int last;
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
		{
			if (ShouldTurnOff(p->vkCode,last)) {
				PostQuitMessage(0);
			}
			else
			keyboard.push(p->vkCode);
			last = p->vkCode;
		}
		if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
		{
			if (ShouldTurnOff(p->vkCode,last)) {
				PostQuitMessage(0);
			}
			else
			keyboard.release(p->vkCode);
		}
	}
	return 1;
}