#pragma once
#include "Parser.h"
class LongParser :
	public Parser
{
public:
	LongParser();
	std::vector<std::function<void()>> Parse(std::string) override;
	~LongParser();
private:
	std::pair<std::string, std::string> GetCommandAndParams(std::string) const;
};

