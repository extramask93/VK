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
//!Message containing network settings to be put on the remote device
struct IPMessage: IMessage
{
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual uint8_t getPacketId() override;
	virtual std::vector<uint8_t> getFields() override;
	static const uint8_t packetId = STATIC_IP_REQ;
	uint8_t mode; //!Specifies mode for obtaining ip: 0-static 1-dhcp
	uint32_t ip32;
	uint16_t port;
	uint32_t mask32;
	uint32_t gateWay32;
};
//! Encapsulates all the logic needed to change remote device settings
class ChangeIPMode :
	public IRunMode
{
public:
	/*! @param[in] que Message queue used to send by TCP Thread
		@param[in] vm Map containing supplied command  options*/
	ChangeIPMode(BlockingQueue<std::shared_ptr<IMessage>> &bque_,boost::program_options::variables_map const &vm);
	/*!
		@param[in] mode 0 or 1 value indicating either static mode or dhcp mode
		@param[in] port Port on which remote device should listen
		@param[in] mask Textual representation(dot format) of  Subnet mask
		@param[in] gateWay Textual representation(dot format) of a default gateway)*/
	void ParseAddress(uint8_t mode,std::string ip,uint16_t port, std::string mask, std::string gateWay);//!<Parses addressess to integer represetations
	~ChangeIPMode();
	// Odziedziczono za poœrednictwem elementu IRunMode
	virtual void Run() override;
private:
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	IPMessage message;
};



