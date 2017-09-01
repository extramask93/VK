#pragma once
#include <Windows.h>
//! Interface used to represet different modes of operation
class IRunMode
{
public:
	virtual ~IRunMode() = default;
	typedef LRESULT(__stdcall * KeyboardHookProc)(int , WPARAM , LPARAM );
	typedef LRESULT(__stdcall * MouseHookProc)(int , WPARAM , LPARAM );
	KeyboardHookProc keyhookproc;//!< Pointer to the kyboard hook function
	MouseHookProc mousehookproc;//!<Pointer to the mouse hook function
	virtual void Run()=0;
	virtual bool RequiresConnection(){return true;}//!< Specifies whether the TCP Thread should be run
};

