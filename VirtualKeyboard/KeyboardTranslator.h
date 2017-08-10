#pragma once
#include "ITranslator.h"
#include "IKeyboard.h"
#include <cstdint>
#include <initializer_list>
#include <vector>
#include <map>

class KeyboardTranslator :public ITranslator
{
public:
	KeyboardTranslator();
	virtual keyType translate(keyType key) override;
	uint8_t mapModifier(keyType key);
	~KeyboardTranslator();
private:
	void fillModMap();
	std::map<keyType, uint8_t> modifierMap;
};

