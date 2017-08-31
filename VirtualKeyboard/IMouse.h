#pragma once
#include "IKeyboard.h"
#include "IMessage.h"

#define MOUSE_WRITE_REQ 0x40

struct MouseReport :public IMessage
{
	uint8_t id;
	uint8_t buttons;
	int8_t X;
	int8_t Y;
	int8_t Wheel;
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual std::vector<uint8_t> getFields() override;
	virtual uint8_t getPacketId() override;
};


class IMouse
{
public:
	virtual void push(keyType key)=0;
	virtual void release(keyType key) = 0;
	virtual void releaseAll() = 0;
	virtual void move(int8_t x, int8_t y, int8_t wheel)=0;
	virtual void updatePosition(int x, int y) = 0;
	virtual void updateWheel(int distance) = 0;
	IMouse();
	virtual ~IMouse();
};

