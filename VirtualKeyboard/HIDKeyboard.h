#pragma once
#include "IKeyboard.h"
#include "boost\signals2.hpp"
#include <vector>
struct Report
{
	uint8_t id;
	uint8_t reserved;
	uint8_t modifiers;
	std::array<uint8_t, 6> keys;
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

