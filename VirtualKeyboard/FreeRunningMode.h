#pragma once
#include "IRunMode.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "boost/program_options.hpp"
#include <Windows.h>
#include "Printer.h"
#include "BlockingQueue.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);//!< Hooks keyboard and blocks keystrokes from propagating further
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam); //!< Hooks mouse and blocks events from propagating further
LRESULT CALLBACK LowLevelProcNoOp(int nCode, WPARAM wParam, LPARAM lParam);//!< Function used as a dummy, only tranfers control to the oprating system
LRESULT CALLBACK LowLevelKeyboardProcDual(int nCode, WPARAM wParam, LPARAM lParam); // !< Hooks keyboard and tranfers control to the oprating system
LRESULT CALLBACK LowLevelMouseProcDual(int nCode, WPARAM wParam, LPARAM lParam);//!< Hooks mouse and tranfers control to the oprating system
//! Implements mode in which events are transfered in real time to remote device
class FreeRunningMode :
	public IRunMode
{
public:
	/*! @param[in] que Message queue used to send by TCP Thread
		@param[in] vm Map containing supplied command  options*/
	FreeRunningMode(BlockingQueue<std::shared_ptr<IMessage>> &que,boost::program_options::variables_map const &map);
	FreeRunningMode(const FreeRunningMode &r);
	FreeRunningMode& operator=(FreeRunningMode const &r);
	virtual void Run() override;
private:
	bool isMouseInUse() const;
	void ResetMousePosition() const;
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	Printer printer;
};

