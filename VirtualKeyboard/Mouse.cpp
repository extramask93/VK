#include "stdafx.h"
#include "Mouse.h"
#include <cmath>
#define TRESHOLD 20
Mouse::Mouse()
{
	report.id = 2;
	auto width = GetSystemMetrics(SM_CXSCREEN);
	if (width == 0)
		PrintThread{}<<("No mouse installed\n");
	auto height = GetSystemMetrics(SM_CYSCREEN);
	if (height == 0)
		PrintThread{} << ("No mouse installed\n");
	screenWidth = width;
	screenHeight = height;
	widthScaleFactor = width / 127;
	heightScaleFactor = height / 127;
}

void Mouse::DisconnectAll()
{
	reportChanged.disconnect_all_slots();
}

boost::signals2::connection Mouse::connect(const signal_t::slot_type & subscribent)
{
	return reportChanged.connect(subscribent);
}

boost::signals2::connection Mouse::connect2(const signal2_t::slot_type& subscribent)
{
	return reportChanged2.connect(subscribent);
}

void Mouse::push(keyType key)
{
	report.buttons |= (1<<key);
	reportChanged(report);
	reportChanged2(report,"mousePush");
}

void Mouse::release(keyType key)
{
	report.buttons &= ~(1<<key);
	reportChanged(report);
	reportChanged2(report, "mouseRelease" );

}

void Mouse::releaseAll()
{
	report.buttons = 0;
	reportChanged(report);
	reportChanged2(report,"mouseReleaseAll");
}

void Mouse::updatePosition(int x, int y)
{
	/*static bool wasProgrammaticallyChanged = false;
	if (wasProgrammaticallyChanged)
	{
		wasProgrammaticallyChanged = false;
		return;
	}
	if (x > screenWidth|| x<0 || y<0 || y>screenHeight) {
		wasProgrammaticallyChanged = true;
		SetCursorPos(screenWidth / 2, screenHeight / 2);
	}//xcnt--;*/

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
	report.X = ceil((currentX-prevX));
	report.Y = ceil((currentY-prevY));
	if (report.X == 0 && report.Y == 0)
		return;
	reportChanged(report);
	reportChanged2(report,"mouseMove");
}

void Mouse::move(int8_t x, int8_t y, int8_t wheel)
{
	report.X = x;
	report.Y = y;
	report.Wheel = wheel;
	reportChanged(report);
	reportChanged2(report,"mouseMove");
}

void Mouse::updateWheel(int distance)
{
	report.Wheel = distance > 0 ? 20 : -20;
	reportChanged(report);
	reportChanged2(report,"mouseMove"); 
}
