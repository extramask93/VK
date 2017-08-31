#pragma once
#include "IRunMode.h"
#include <string>
#include "boost\asio.hpp"
#include "BlockingQueue.h"
#include "boost/program_options.hpp"
#include "boost/algorithm/string.hpp"
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
	uint8_t mode;
	uint32_t ip32;
	uint16_t port;
	uint32_t mask32;
	uint32_t gateWay32;
};

class ChangeIPMode :
	public IRunMode
{
public:
	ChangeIPMode(BlockingQueue<std::shared_ptr<IMessage>> &bque_,uint8_t mode,std::string ip,uint16_t port, std::string mask, std::string gateWay);
	ChangeIPMode(BlockingQueue<std::shared_ptr<IMessage>> &bque_,boost::program_options::variables_map const &vm);
	void ParseAddress(uint8_t mode,std::string ip,uint16_t port, std::string mask, std::string gateWay);
	~ChangeIPMode();
	// Odziedziczono za poœrednictwem elementu IRunMode
	virtual void Run() override;
private:
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	IPMessage message;
};



