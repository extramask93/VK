#pragma once
#include "boost\program_options.hpp"
#include <iostream>
#include <fstream>

namespace po = boost::program_options;
using namespace std;
class CMDArgsParser
{
public:
	CMDArgsParser();
	po::variables_map parseCommandLineArguments(int argc, char *argv[]) const;
	~CMDArgsParser();
private:
	static void conflicting_options(const po::variables_map& vm,
		const char* opt1, const char* opt2);
	static void option_dependency(const po::variables_map& vm,
		const char* for_what, const char* required_option);
};

