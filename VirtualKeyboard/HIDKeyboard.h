#pragma once
#include "IKeyboard.h"
#include "boost\signals2.hpp"
#include "IMessage.h"
#include <vector>

#define KEYBOARD_WRITE_REQ 0x30
struct Report;
using signal_t = boost::signals2::signal<void(const Report&)>;

struct Report: public IMessage 
{
	uint8_t id;
	uint8_t reserved;
	uint8_t modifiers;
	std::array<uint8_t, 6> keys;
	Report();
	// Odziedziczono za poœrednictwem elementu IMessage
	virtual uint8_t getSize() override;
	virtual std::vector<uint8_t> getFields() override;
	virtual uint8_t getPacketId() override;
};


class HIDKeyboard :
	public IKeyboard
{
public:
	HIDKeyboard();
	boost::signals2::connection connect(const signal_t::slot_type &subscriber);
	void disconnectAll();
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
private:
	signal_t reportChanged;
	Report report;
	bool isModifier(keyType key) const;
};

