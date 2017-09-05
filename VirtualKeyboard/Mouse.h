#pragma once
#include "Keyboard.h"
#include "IMouse.h"
#include <Windows.h>
#include <boost\signals2.hpp>
#include "ThreadPrinter.h"
//! Class which keeps track of mouse state
class Mouse: public IMouse
{
public:
	Mouse();
	enum MouseButtons { LPM_ = 0, PPM_ = 1 };
	void DisconnectAll();//!< Disconnects all subscribents
	typedef boost::signals2::signal<void(MouseReport)> signal_t;
	typedef boost::signals2::signal<void(MouseReport,std::string)> signal2_t;
	boost::signals2::connection connect(const signal_t::slot_type &subscribent);//!< Informs about mouse state change
	boost::signals2::connection connect2(const signal2_t::slot_type &subscribent);//!< Informs about mouse state change
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
	virtual void updatePosition(int x, int y) override;//!< Updates position with scaling
	virtual void move(int8_t x, int8_t y, int8_t wheel) override;//!< Updates position without scaling
	virtual void updateWheel(int distance) override;//!< Updates wheel
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

