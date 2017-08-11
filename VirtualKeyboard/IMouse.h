#pragma once
#include "IKeyboard.h"
#include "IMessage.h"
#define MOUSE_WRITE_REQ 0x40
struct MouseReport :public IMessage
{
	uint8_t id;
	uint8_t buttons;
	int prevX;
	int prevY;
	int currentX;
	int currentY;
	int8_t X;
	int8_t Y;
	int8_t Wheel;
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual std::vector<uint8_t> getFields() override;
	virtual uint8_t getPacketId() override;
private:
	static const uint8_t SIZE = 6;
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

