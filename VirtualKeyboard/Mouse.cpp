#include "stdafx.h"
#include "Mouse.h"

Mouse::Mouse(IMouse & mouse): mouse{ mouse }
{
	report.id = 2;
	auto width = GetSystemMetrics(SM_CXSCREEN);
	if (width == 0)
		printf("No mouse installed");
	auto height = GetSystemMetrics(SM_CYSCREEN);
	if (height == 0)
		printf("No mouse installed");
	printf("Resolution: %dx%d",width,height);
	screenWidth = width;
	screenHeight = height;
	widthScaleFactor = width / 127;
	heightScaleFactor = height / 127;
}

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
	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (x > screenWidth)
		x = screenWidth;
	if (y > screenHeight)
		y = screenHeight;
	prevX = currentX;
	prevY = currentY;
	currentX = x;
	currentY = y;
	report.X = (currentX-prevX);
	report.Y = (currentY-prevY);
	reportChanged(report);
}

void Mouse::updateWheel(int distance)
{
	report.Wheel = distance > 0 ? 20 : -20;
	reportChanged(report);
}
