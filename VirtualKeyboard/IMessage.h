#pragma once
#include <cstdint>
#include <vector>
class IMessage
{
public:
	IMessage();
	virtual uint8_t getSize()=0;
	virtual uint8_t getPacketId() = 0;
	virtual std::vector<uint8_t> getFields() = 0;
	virtual ~IMessage();
};

