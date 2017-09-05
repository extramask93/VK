#pragma once
#include "Parser.h"

//! Class parsing macro in [LONG] format
class LongParser :
	public Parser
{
public:
	LongParser();
	/*! @param[in] line Line containing macro command to parse
		\return Return vector of functors representing given line.*/
	std::vector<std::function<void()>> Parse(std::string) override;
	~LongParser();
private:
	std::pair<std::string, std::string> GetCommandAndParams(std::string) const;
};

