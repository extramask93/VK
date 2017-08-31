#include "stdafx.h"
#include "ShortParser.h"
#include "ParseException.h"


std::vector<std::function<void()>> ShortParser::Parse(std::string line)
{
	auto tokens = Tokenize(line);
	std::vector<std::function<void()>> functions;
	for(auto &token:tokens)
	{
		std::pair<std::string, std::string> commandAndParamsPair;
		if (isMouseCommand(token))
			commandAndParamsPair = ParseMouseCommand(token);
		else if (isKeyboardCommand(token))
			commandAndParamsPair = ParseKeyboardCommand(token);
		else
			throw ParseException{"Error: Unknown short type command \"" + token + "\""};
		confirmFunctionExisting(commandAndParamsPair.first);
		functions.push_back(std::bind(mp[commandAndParamsPair.first], commandAndParamsPair.second));
	}
	return functions;
}

std::vector<std::string> ShortParser::Tokenize(std::string line) const
{
	std::vector<std::string> tokens;
	boost::split(tokens, line, boost::is_any_of("\t\r\n "), boost::token_compress_on);
	return tokens;
}

ShortParser::ShortParser(): Parser{}
{
}


ShortParser::~ShortParser()
{
}

std::pair<std::string,std::string> ShortParser::ParseKeyboardCommand(const std::string &command) const
{
	return {"keyboardWrite",command};
}

bool ShortParser::isMouseCommand(const std::string& command) const
{
	auto reg = std::regex{ R"(M\S)" };
	auto m = std::smatch{};
	std::regex_search(command, m, reg);
	return !m.empty();
}
std::pair<std::string, std::string> ShortParser::ParseMouseCommand(const std::string& command) const
{
	auto regMove = std::regex{ R"((MM)(\d+,\s*\d+,\s*\d+))" };
	auto regClick = std::regex{ R"((MC)(\S))" };
	auto match = std::smatch{};
	std::regex_search(command, match, regMove);
	if (match[1].str() == "MM" && match.size() == 3)
		return{ "mouseMove",match[2].str() };
	std::regex_search(command, match, regClick);
	if (match[1].str() == "MC" && match.size() == 3)
		return{ "mouseClick",match[2].str() };
	throw ParseException{"Error: "+command+". Keyboard command paramteres not matching function definition."};

}

bool ShortParser::isKeyboardCommand(const std::string& command) const
{
	return !isMouseCommand(command);
}
