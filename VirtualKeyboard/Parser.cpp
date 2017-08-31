#include "stdafx.h"
#include "Parser.h"
#include <boost/lexical_cast.hpp>
#include "ParseException.h"
#include "maps.h"

extern BlockingQueue<std::shared_ptr<IMessage>> bque;

Parser::Parser()
{
	mp["delayS"] = std::bind(&Parser::sleepS, this, std::placeholders::_1); //&Parser::sleepS;
	mp["delayMS"] = std::bind(&Parser::sleepMS, this, std::placeholders::_1);
	mp["keyboardPush"] = std::bind(&KeyboardAdapter::keyboardPush,&keyboard, std::placeholders::_1);
	mp["keyboardWrite"] = std::bind(&KeyboardAdapter::keyboardWrite, &keyboard, std::placeholders::_1);
	mp["keyboardRelease"] = std::bind(&KeyboardAdapter::keyboardRelease, &keyboard, std::placeholders::_1);
	mp["keyboardReleaseAll"] = std::bind(&KeyboardAdapter::keyboardReleaseAll, &keyboard, std::placeholders::_1);
	mp["mouseMove"] = std::bind(&MouseAdapter::mouseMove, &mouse, std::placeholders::_1);
	mp["mouseReleaseAll"] = std::bind(&MouseAdapter::mouseReleaseAll, &mouse, std::placeholders::_1);
	mp["mouseRelease"] = std::bind(&MouseAdapter::mouseRelease, &mouse, std::placeholders::_1);
	mp["mousePush"] = std::bind(&MouseAdapter::mousePush, &mouse, std::placeholders::_1);
	mp["mouseClick"] = std::bind(&MouseAdapter::mouseClick, &mouse, std::placeholders::_1);
}

bool Parser::ParseLoop(std::string line)
{
	auto reg = std::regex{R"(^(DO)\s+(\d{1,3}))"};
	auto m = std::smatch{};
	if(!std::regex_search(line, m, reg))
		return false;
	loopCount=m[2].str();
	return true;
}

void Parser::confirmFunctionExisting(std::string const & name)
{
	try {
		mp.at(name);
	}
	catch (std::exception const &ex)
	{
		throw ParseException{ "Unknown command: " + name + "." };
	}

}

CountType Parser::GetNumberOfLoopRepetitions() const
{
	if (loopCount.empty())
		throw ParseException{"Error: Loop counter invalid."};
	try {
		auto lp = boost::lexical_cast<CountType>(loopCount);
		return lp;
	}
	catch(boost::bad_lexical_cast const &ex)
	{
		throw ParseException{ "Error: Loop counter invalid." };
	}
}


Parser::~Parser()
{
}


void Parser::sleepS(std::string l)
{
	unsigned int delay;
	try {
		delay = boost::lexical_cast<unsigned int>(l);
	}
	catch (const boost::bad_lexical_cast)
	{
		throw ParseException{ "Error: Sleep delay invalid." };
	}
	boost::this_thread::sleep(boost::posix_time::seconds(delay));
}

void Parser::sleepMS(std::string l)
{
	unsigned int delay;
	try {
		delay = boost::lexical_cast<unsigned int>(l);
	}
	catch (const boost::bad_lexical_cast& ex)
	{
		throw ParseException{ "Error: Sleep delay invalid." };
	}
	boost::this_thread::sleep(boost::posix_time::millisec(delay));
}




