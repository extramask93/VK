// VirtualKeyboard.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "boost/program_options.hpp" 
#
#include <string>
#include <iostream>
namespace po = boost::program_options;
using namespace std;
int main(int argc,char *argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "produce help message")
		("script file", po::value<std::string>(), "run given script")
		;
	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		return 1;
	}

	if (vm.count("script file")) {
		cout << "Compression level was set to "
			<< vm["compression"].as<int>() << ".\n";
	}
	else
		cout << "Script file not specified. Running record mode";

    return 0;
}

