#pragma once

#include <stdexcept>
//! Exception thrown in Macro mode, indicates syntax errors in macro file.
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
	ErrorCode error;//! Currently not used
	~ParseException();
};

