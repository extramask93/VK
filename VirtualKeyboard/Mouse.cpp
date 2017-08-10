#include "stdafx.h"
#include "Mouse.h"

#define LPM 1
#define PPM	2
#define SPM	4
boost::signals2::connection Mouse::connect(signal_t::slot_type & subscribent)
{
	return reportChanged.connect(subscribent);
}

void Mouse::push(keyType key)
{
	key = key >> 16;
		report.buttons |= key;

		report.buttons |= key;
}

void Mouse::release(keyType key)
{
	key = key >> 16;
		report.buttons &= ~(key);
		report.buttons &= ~(key);

}

void Mouse::releaseAll()
{
	report.buttons = 0;
}

void Mouse::updatePosition(int x, int y)
{
	state.prevX = state.X;
	state.prevY = state.Y;
	state.X = x;
	state.Y = y;
	mouse.updatePosition(x, y);
}

void Mouse::updateWheel(int distance)
{
	report.Wheel = distance;
}
