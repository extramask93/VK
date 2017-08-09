#pragma once
#include "IRunMode.h"
#include <Windows.h>
class FreeRunningMode :
	public IRunMode
{
public:
	FreeRunningMode();
	virtual ~FreeRunningMode();
private:
	static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
	HHOOK hhkLowLevelKybd;
};

