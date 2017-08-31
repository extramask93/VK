#pragma once
#include "IRunMode.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "boost/program_options.hpp"
#include <Windows.h>
#include "Printer.h"
#include "BlockingQueue.h"

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelProcNoOp(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelKeyboardProcDual(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelMouseProcDual(int nCode, WPARAM wParam, LPARAM lParam);

class FreeRunningMode :
	public IRunMode
{
public:
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

