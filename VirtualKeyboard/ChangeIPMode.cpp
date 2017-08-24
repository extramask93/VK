#include "stdafx.h"
#include "ChangeIPMode.h"


extern BlockingQueue<std::shared_ptr<IMessage>> bque;

ChangeIPMode::ChangeIPMode(std::string ip, std::string mask, std::string gateWay)
{
	try {
		message.ip32 = boost::asio::ip::address_v4::from_string(ip).to_ulong();
		message.mask32 = boost::asio::ip::address_v4::from_string(mask).to_ulong();
		message.gateWay32 = boost::asio::ip::address_v4::from_string(gateWay).to_ulong();
	}
	catch (...)
	{
		exit(1);
	}
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
	return (sizeof(ip32)+sizeof(mask32)+sizeof(gateWay32)+sizeof(packetId));
}

uint8_t IPMessage::getPacketId()
{
	return packetId;
}

std::vector<uint8_t> IPMessage::getFields()
{
	std::vector<unsigned char> fields;
	fields.push_back(ip32);
	fields.push_back(mask32);
	fields.push_back(gateWay32);
	return fields;
}
