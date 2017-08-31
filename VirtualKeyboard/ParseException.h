#pragma once

#include <stdexcept>
class ParseException :
	public std::runtime_error
{
public:
	enum class ErrorCode
	{
		NO_COMMAND_FOUND =2000,

	};
	ParseException(const char *m) : std::runtime_error{ m }{}
	ParseException(const std::string s) : std::runtime_error{s.c_str()}{}
	ErrorCode error;
	~ParseException();
};

