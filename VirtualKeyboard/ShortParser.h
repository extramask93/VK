#pragma once
#include "Parser.h"

class ShortParser :
	public Parser
{
public:
	ShortParser();
	virtual std::vector<std::function<void()>> Parse(std::string) override;
	std::vector<std::string> Tokenize(std::string) const;
	~ShortParser();
private:
	std::pair<std::string,std::string> ParseKeyboardCommand(const std::string &command) const;
	bool isMouseCommand(const std::string &command) const;
	std::pair<std::string,std::string> ParseMouseCommand(const std::string &command) const;
	bool isKeyboardCommand(const std::string &command) const;
};

