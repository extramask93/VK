#include "stdafx.h"
#include "Mouse.h"

boost::signals2::connection Mouse::connect(const signal_t::slot_type & subscribent)
{
	return reportChanged.connect(subscribent);
}

void Mouse::push(keyType key)
{
	report.buttons |= (1<<key);
	reportChanged(report);
}

void Mouse::release(keyType key)
{
	report.buttons &= ~(1<<key);
	reportChanged(report);

}

void Mouse::releaseAll()
{
	report.buttons = 0;
	reportChanged(report);
}

void Mouse::updatePosition(int x, int y)
{
	report.prevX = report.X;
	report.prevX = report.Y;
	report.currentX = x;
	report.currentY = y;
	report.X = report.prevX-report.currentX;
	report.Y = report.prevY - report.currentY;
	reportChanged(report);
}

void Mouse::updateWheel(int distance)
{
	report.Wheel = (distance>>16);
	reportChanged(report);
}
