#pragma once
#include "Keyboard.h"
#include "IMouse.h"
#include <Windows.h>
#include <boost\signals2.hpp>
class Mouse:public IMouse
{
public:
	enum MouseButtons {LPM=0,PPM=2};
	typedef boost::signals2::signal<void(MouseReport)> signal_t;
	Mouse(IMouse &mouse);
	boost::signals2::connection connect(const signal_t::slot_type &subscribent);
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
	virtual void updatePosition(int x, int y) override;
	virtual void updateWheel(int distance) override;
private:
	int prevX;
	int prevY;
	int currentX;
	int currentY;
	int screenWidth;
	int screenHeight;
	int widthScaleFactor;
	int heightScaleFactor;
	MouseState state;
	MouseReport report;
	signal_t reportChanged;
	IMouse &mouse;
	// Odziedziczono za poœrednictwem elementu IMouse

};

