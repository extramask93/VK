#pragma once
#include <cstdint>
#include <vector>
//! Iterface used by different types of messages wishing to be sent by TCP Thread
class IMessage
{
public:
	IMessage();
	//! Returns size of the message in bytes. 
	virtual uint8_t getSize()=0;
	//! returns packet id according to the protocol specification
	virtual uint8_t getPacketId() = 0;
	//! returns byte-wide fields of the message. Includes id.
	virtual std::vector<uint8_t> getFields() = 0;
	virtual ~IMessage();
};

