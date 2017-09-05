#pragma once
#include "IKeyboard.h"
#include "IMessage.h"

#define MOUSE_WRITE_REQ 0x40
//! Message containing mouse data
struct MouseReport :public IMessage
{
	uint8_t id;//!< Packet id
	uint8_t buttons;//!< Currently pushed buttons
	int8_t X;//!< X position increment
	int8_t Y;//!< Y Position increment
	int8_t Wheel;//!< Wheel change (positive goes up, negative goes down)
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual std::vector<uint8_t> getFields() override;
	virtual uint8_t getPacketId() override;
};

//! Iterface for mouse classess
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

