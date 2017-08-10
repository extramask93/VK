#pragma once
#include "IKeyboard.h"
struct MouseReport
{
	uint8_t id;
	uint8_t buttons;
	int8_t X;
	int8_t Y;
	int8_t Wheel;
};
struct MouseState
{
	int32_t X;
	int32_t Y;
	int32_t prevX;
	int32_t prevY;
	int16_t Wheel;
	keyType lpm;
	keyType ppm;
};
class IMouse
{
public:
	virtual void push(keyType key)=0;
	virtual void release(keyType key) = 0;
	virtual void releaseAll() = 0;
	virtual void updatePosition(int x, int y) = 0;
	virtual void updateWheel(int distance) = 0;
	IMouse();
	~IMouse();
};

