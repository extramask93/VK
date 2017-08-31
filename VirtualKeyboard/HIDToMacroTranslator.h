#pragma once
#include <string>

class HIDToMacroTranslator
{
public:
	HIDToMacroTranslator();
	std::string Translate(uint8_t key);
	~HIDToMacroTranslator();
};

