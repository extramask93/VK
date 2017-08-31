#pragma once
#include <Windows.h>
class IRunMode
{
public:
	virtual ~IRunMode() = default;
	typedef LRESULT(__stdcall * KeyboardHookProc)(int , WPARAM , LPARAM );
	typedef LRESULT(__stdcall * MouseHookProc)(int , WPARAM , LPARAM );
	KeyboardHookProc keyhookproc;
	MouseHookProc mousehookproc;
	virtual void Run()=0;
	virtual bool RequiresConnection(){return true;}
};

