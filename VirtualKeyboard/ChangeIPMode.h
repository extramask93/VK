#pragma once
#include "IRunMode.h"
#include <string>
#include "boost\asio.hpp"
#include "BlockingQueue.h"
#include "IMessage.h"
#include <memory>
#define STATIC_IP_REQ 0x45
struct IPMessage: IMessage
{
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual uint8_t getPacketId() override;
	virtual std::vector<uint8_t> getFields() override;
	static const uint8_t packetId = STATIC_IP_REQ;
	uint32_t ip32;
	uint32_t mask32;
	uint32_t gateWay32;
};

class ChangeIPMode :
	public IRunMode
{
public:
	ChangeIPMode(std::string ip, std::string mask, std::string gateWay);
	~ChangeIPMode();

	// Odziedziczono za poœrednictwem elementu IRunMode
	virtual void Run() override;
private:
	IPMessage message;
};

