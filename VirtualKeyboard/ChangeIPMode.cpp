#include "stdafx.h"
#include "ChangeIPMode.h"


extern BlockingQueue<std::shared_ptr<IMessage>> bque;

ChangeIPMode::ChangeIPMode(uint8_t mode, std::string ip,uint16_t port, std::string mask, std::string gateWay)
{
	ParseAddress(mode,ip,port,mask,gateWay);
}

void ChangeIPMode::ParseAddress(uint8_t mode, std::string ip,uint16_t port, std::string mask, std::string gateWay)
{
	try {
		message.mode = mode;
		message.ip32 = boost::asio::ip::address_v4::from_string(ip).to_ulong();
		message.port = port;
		message.mask32 = boost::asio::ip::address_v4::from_string(mask).to_ulong();
		message.gateWay32 = boost::asio::ip::address_v4::from_string(gateWay).to_ulong();
	}
	catch (...)
	{
		exit(1);
	}
}

ChangeIPMode::ChangeIPMode()
{
	message.mode = 1;
	message.ip32 = 0;
	message.port = 21;
	message.mask32 = 0;
	message.gateWay32 = 0;
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
