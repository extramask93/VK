#include "stdafx.h"
#include "IMouse.h"


IMouse::IMouse()
{
}


IMouse::~IMouse()
{
}

uint8_t MouseReport::getSize()
{
	return SIZE;
}

std::vector<uint8_t> MouseReport::getFields()
{
	uint8_t x = X;
	uint8_t y = Y;
	uint8_t wheel = Wheel;
	return std::vector<uint8_t>{ getPacketId(),id,buttons,x,y,wheel };
}

uint8_t MouseReport::getPacketId()
{
	return MOUSE_WRITE_REQ;
}
