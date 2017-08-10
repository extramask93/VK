#pragma once
#include "IKeyboard.h"
#include <vector>
class HIDKeyboard :
	public IKeyboard
{
public:
	struct Report
	{
		uint8_t id;
		uint8_t reserved;
		uint8_t modifiers;
		std::array<uint8_t, 6> keys;
	};
	HIDKeyboard();
	bool isModifier(keyType key);
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll() override;
	
private:
	Report report;
};

