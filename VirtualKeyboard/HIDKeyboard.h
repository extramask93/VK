#pragma once
#include "IKeyboard.h"
#include "boost\signals2.hpp"
#include "IMessage.h"
#include <vector>
#define KEYBOARD_WRITE_REQ 0x30
struct Report:public IMessage 
{
	uint8_t id;
	uint8_t reserved;
	uint8_t modifiers;
	std::array<uint8_t, 6> keys;
	Report() {
		id = 1;
		reserved =modifiers= 0;
		for (auto &key : keys)
		{
			key = 0;
		}
	}
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual std::vector<uint8_t> getFields() override;
	virtual uint8_t getPacketId() override;
	static const uint8_t SIZE = 9;
};


class HIDKeyboard :
	public IKeyboard
{
public:
	typedef boost::signals2::signal<void(const Report&)> signal_t;
	boost::signals2::connection connect(const signal_t::slot_type &subscriber);
	HIDKeyboard();
	bool isModifier(keyType key);
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;

	
private:
	signal_t reportChanged;
	Report report;
};

