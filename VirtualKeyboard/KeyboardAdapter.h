#pragma once

#include "IMessage.h"
#include "BlockingQueue.h"
#include "HIDKeyboard.h"
#include "maps.h"
#include "boost\bimap.hpp"
using KeyReport = Report;
using CountType = size_t;

class KeyboardAdapter 
{
public:
	KeyboardAdapter();
	~KeyboardAdapter();
	void keyboardPush(std::string l);
	void keyboardWrite(std::string l);
	size_t keyboardRelease(std::string l);
	void keyboardReleaseAll(std::string l);
private:
	KeyReport keyReport;
	KeyReport *addToReport(uint8_t k);
	KeyReport* subtractFromReport(uint8_t k);
	uint8_t isAModifier(uint8_t k);
	uint8_t isNonPrintable(uint8_t k) const;
	uint8_t isPrintableReachedWithShift(uint8_t k) const;
	uint8_t stripModifiers(uint8_t k) const;
	uint8_t KeyCodeFromString(std::string s) const;
};

