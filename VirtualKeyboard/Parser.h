#pragma once
#include <string>
#include <regex>
#include <functional>
#include <map>
#include <vector>
#include "Mouse.h"
#include "boost/algorithm/string.hpp"
#include "HIDKeyboard.h"
#include "KeyboardAdapter.h"
#include "MouseAdapter.h"
using KeyReport = Report;
using CountType = size_t;
class Parser
{
public:
	Parser();
	uint8_t KeyCodeFromString(std::string s) const;//!< Returns HID key code
	bool ParseLoop(std::string line);//!< Determines if give line is an loop statement
	virtual std::vector<std::function<void()>> Parse(std::string)=0;//!< Returns vector of functors representing each command in a string
	virtual void confirmFunctionExisting(std::string const &name);//!< Throws if function name i not valid
	CountType GetNumberOfLoopRepetitions() const;
	virtual ~Parser();
	//
	void sleepS(std::string  l);
	void sleepMS(std::string l);
protected:
	std::map<std::string, std::function<void(std::string)>> mp;
	std::string loopCount;
	KeyboardAdapter keyboard;
	MouseAdapter mouse;
};

