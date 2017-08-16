#include "stdafx.h"
#include "Message.h"



Message::~Message()
{
}

uint8_t Message::getSize()
{
	return uint8_t();
}

uint8_t Message::getPacketId()
{
	return uint8_t();
}

std::vector<uint8_t> Message::getFields()
{
	return std::vector<uint8_t>();
}
