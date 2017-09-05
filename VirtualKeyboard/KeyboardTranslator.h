#pragma once
#include "ITranslator.h"
#include "IKeyboard.h"
#include <cstdint>
#include <initializer_list>
#include <vector>
#include <map>
//! Translates keystrokes from one keyboard representation to another
class KeyboardTranslator :public ITranslator
{
public:
	KeyboardTranslator();
	virtual keyType Translate(keyType key) override;
	~KeyboardTranslator();
private:
	void FillModMap();
	std::map<keyType, uint8_t> modifierMap;
};

