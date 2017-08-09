#pragma once
#include "IRunMode.h"
#include "Keyboard.h"
//#include "Mouse.h"
#include <Windows.h>
#include "Mouse.h"
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
Keyboard keyboard;
Mouse mouse;
class FreeRunningMode :
	public IRunMode
{
public:
	FreeRunningMode();
};

