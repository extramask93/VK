#pragma once
#include "boost\program_options.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include "IRunMode.h"
#include "Macro.h"
#include "RecordMode.h"
#include "ChangeIPMode.h"
#include "FreeRunningMode.h"
#include "NoMode.h"
namespace po = boost::program_options;
using namespace std;
//! Class used to obtain map of parameters supplied to the command line and to check dependencies/conflicts of options 
class CMDArgsParser
{
public:
	CMDArgsParser();
	/*! \return variable_map containing all supplied command/file arguments
		@param[in] argc Command line argument counter
		@param[in] argv Pointer to a array of arguments*/
	po::variables_map parseCommandLineArguments(int argc, char *argv[]) const;
	std::unique_ptr<IRunMode> SelectRunMode(po::variables_map &vm);
	~CMDArgsParser();
private:
	static void conflicting_options(const po::variables_map& vm,
		const char* opt1, const char* opt2);
	static void option_dependency(const po::variables_map& vm,
		const char* for_what, const char* required_option);
};

