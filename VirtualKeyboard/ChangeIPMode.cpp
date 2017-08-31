#include "stdafx.h"
#include "ChangeIPMode.h"

extern BlockingQueue<std::shared_ptr<IMessage>> bque;

ChangeIPMode::ChangeIPMode(BlockingQueue<std::shared_ptr<IMessage>> &bque_,uint8_t mode, std::string ip,uint16_t port, std::string mask, std::string gateWay):bque{bque_}
{
	ParseAddress(mode,ip,port,mask,gateWay);
}

void ChangeIPMode::ParseAddress(uint8_t mode, std::string ip,uint16_t port, std::string mask, std::string gateWay)
{
	message.mode = mode;
	message.ip32 = boost::asio::ip::address_v4::from_string(ip).to_ulong();
	message.port = port;
	message.mask32 = boost::asio::ip::address_v4::from_string(mask).to_ulong();
	message.gateWay32 = boost::asio::ip::address_v4::from_string(gateWay).to_ulong();
}

ChangeIPMode::ChangeIPMode(BlockingQueue<std::shared_ptr<IMessage>> &bque_, boost::program_options::variables_map const &vm):bque{bque_}
{
	std::vector<std::string> specs;
	boost::split(specs, vm["setip"].as<std::string>(), boost::is_any_of(","), boost::token_compress_off);
	ParseAddress(boost::lexical_cast<uint16_t>(specs[0]), specs[1], boost::lexical_cast<uint16_t>(specs[2]), specs[3], specs[4]);
}

ChangeIPMode::~ChangeIPMode()
{
}

void ChangeIPMode::Run()
{
	bque.push(std::make_shared<IPMessage>(message));
}

uint8_t IPMessage::getSize()
{
	return (sizeof(mode)+sizeof(ip32)+sizeof(port)+sizeof(mask32)+sizeof(gateWay32)+sizeof(packetId));
}

uint8_t IPMessage::getPacketId()
{
	return STATIC_IP_REQ;
}

std::vector<uint8_t> IPMessage::getFields()
{
	std::vector<unsigned char> fields;
	fields.push_back(getPacketId());
	fields.push_back(mode);
	fields.push_back(((ip32) & 0xFF));
	fields.push_back(((ip32 >>8) & 0xFF));
	fields.push_back(((ip32 >> 16) & 0xFF));
	fields.push_back(((ip32>>24) & 0xFF));
	fields.push_back(((port) & 0xFF));
	fields.push_back(((port>>8) & 0xFF));
	fields.push_back(((mask32 >> 24) & 0xFF));
	fields.push_back(((mask32 >> 16) & 0xFF));
	fields.push_back(((mask32 >> 8) & 0xFF));
	fields.push_back(((mask32) & 0xFF));
	fields.push_back(((gateWay32>> 24) & 0xFF));
	fields.push_back(((gateWay32>> 16) & 0xFF));
	fields.push_back(((gateWay32 >> 8) & 0xFF));
	fields.push_back(((gateWay32) & 0xFF));
	return fields;
}
