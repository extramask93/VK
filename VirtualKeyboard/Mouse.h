#pragma once
#include "Keyboard.h"
#include "IMouse.h"
#include "ThreadPrinter.h"
#include <Windows.h>
#include <boost\signals2.hpp>
class Mouse: public IMouse
{
public:
	Mouse();
	enum MouseButtons { LPM_ = 0, PPM_ = 1 };
	void DisconnectAll();
	typedef boost::signals2::signal<void(MouseReport)> signal_t;
	typedef boost::signals2::signal<void(MouseReport,std::string)> signal2_t;
	boost::signals2::connection connect(const signal_t::slot_type &subscribent);
	boost::signals2::connection connect2(const signal2_t::slot_type &subscribent);
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
	virtual void updatePosition(int x, int y) override;
	virtual void move(int8_t x, int8_t y, int8_t wheel) override;
	virtual void updateWheel(int distance) override;
	MouseReport report;
private:
	int prevX;
	int prevY;
	int currentX;
	int currentY;
	int screenWidth;
	int screenHeight;
	int widthScaleFactor;
	int heightScaleFactor;
	signal_t reportChanged;
	signal2_t reportChanged2;
	// Odziedziczono za poœrednictwem elementu IMouse

};

