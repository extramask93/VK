#include "stdafx.h"
#include "Mouse.h"


boost::signals2::connection Mouse::connect(signal_t::slot_type & subscribent)
{
	return reportChanged.connect(subscribent);
}

void Mouse::push(keyType key)
{
	key = key >> 16;
	if (key = 0x0002)
	{
		state.ppm = 1;
	}
	else if (key = 0x001)
	{
		state.lpm = 1;
	}
}

void Mouse::release(keyType key)
{
	key = key >> 16;
	if (key = 0x0002) {
		state.ppm = 0;
	}
	else if (key = 0x001) {
		state.lpm = 0;
	}

}

void Mouse::releaseAll()
{
	state.lpm = 0;
	state.ppm = 0;
	mouse.releaseAll();
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
	state.Wheel = distance;
	mouse.updateWheel(distance);
}
