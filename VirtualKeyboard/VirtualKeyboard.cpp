// VirtualKeyboard.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "boost/program_options.hpp" 
#include "IRunMode.h"
#include "Keyboard.h"
#include <string>
#include <iostream>


namespace po = boost::program_options;
using namespace std;


int main(int argc,char *argv[])
{
	IRunMode *runmode;
	po::options_description desc("Allowed options");
	desc.add_options()
		("help,h", "produce help message")
		("macro file,i", po::value<std::string>(), "run given script")
		("record a macro,r", po::value<std::string>(),"record while in play mode")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}

	else if (vm.count("macro file")) {
		//run script 
		return 0;
	}
	else if (vm.count("racord a macro"))
	{
		cout << "recording a macro" << endl;
		//run play mode and save (should add logger to an run object)
		return 0;
	}
	else
		cout << "Script file not specified. Running record mode";
	runmode->Run();

    return 0;
}

