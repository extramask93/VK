#pragma once
#include "IMessage.h"
class Message :
	public IMessage
{
public:
	Message(uint8_t size_, uint8_t id_) :id{ id_ }, size{ size_ } {}
	~Message();

	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual uint8_t getPacketId() override;
	virtual std::vector<uint8_t> getFields() override;
private:
	uint8_t id;
	uint8_t size;

};

