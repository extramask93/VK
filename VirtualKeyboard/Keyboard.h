#pragma once
#include <stdint.h>
#include <array>
#include <boost\signals2.hpp>
#include "KeyboardTranslator.h"
#include "IKeyboard.h"
#include "boost\signals2.hpp"
using signal2_t = boost::signals2::signal<void(const uint32_t key, std::string)>;
#define MAX_KEYS 6
using Keys_t= std::array<keyType, MAX_KEYS>;
class Keyboard: public IKeyboard
{
public:
	Keyboard(ITranslator &tr, IKeyboard &keyboard) :keyboard {keyboard}, translator { tr } {}
	boost::signals2::connection connect(const signal2_t::slot_type &subscriber);
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll()override;
	Keys_t getKeys() const;
	~Keyboard();
private:
	signal2_t keyChanged;
	IKeyboard &keyboard;
	ITranslator &translator;
	const keyType id = 1;
	Keys_t keys;
};

