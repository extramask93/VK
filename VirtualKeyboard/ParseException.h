#pragma once
#include "vcruntime_exception.h"
#include <exception>
#include <stdexcept>
class ParseException :
	public std::runtime_error
{
public:
	ParseException(const char *m) : std::runtime_error{ m }{}
	ParseException(const std::string s) : std::runtime_error{s.c_str()}{}
	~ParseException();
};

