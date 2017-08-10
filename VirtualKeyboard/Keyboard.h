#pragma once
#include <stdint.h>
#include <array>
#include <boost\signals2.hpp>
#include "KeyboardTranslator.h"
#include "IKeyboard.h"

#define MAX_KEYS 6
typedef std::array<keyType, MAX_KEYS> Keys_t;
class Keyboard:public IKeyboard
{
public:
	Keyboard(ITranslator &tr, IKeyboard &keyboard) :translator{ tr }, keyboard {keyboard} {}
	virtual void push(keyType key) override;
	virtual void release(keyType key) override;
	virtual void releaseAll()override;
	std::array<keyType, MAX_KEYS> getKeys();
	~Keyboard();
private:
	IKeyboard &keyboard;
	ITranslator &translator;
	const keyType id = 1;
	Keys_t keys;
};

