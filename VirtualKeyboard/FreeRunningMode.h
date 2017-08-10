#pragma once
#include "IRunMode.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <Windows.h>
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
extern Keyboard keyboard;
class FreeRunningMode :
	public IRunMode
{
public:
	FreeRunningMode();
	virtual void Run() override;
};

