#include "stdafx.h"
#include "LongParser.h"
#include "ParseException.h"


std::vector<std::function<void()>> LongParser::Parse(std::string line)
{
	std::vector< std::function<void()>> functions;
	auto commandAndParamsPair = GetCommandAndParams(line);
	confirmFunctionExisting(commandAndParamsPair.first);
	functions.push_back(std::bind(mp[commandAndParamsPair.first], commandAndParamsPair.second));
	return functions;
}

LongParser::LongParser():Parser{}
{
}


LongParser::~LongParser()
{
}

std::pair<std::string, std::string> LongParser::GetCommandAndParams(std::string line) const
{
	auto reg = std::regex{ R"(^([a-zA-z]+)\s+(\S+)$)" };
	auto match = std::smatch{};
	std::regex_search(line, match, reg);
	if (match.empty() || match.size()!=3)
		throw ParseException{ "Error during parsing command \"" + line + "\"" };
	auto command = match[1].str();
	auto params = match[2].str();
	return std::pair<std::string, std::string>{command, params};

}
