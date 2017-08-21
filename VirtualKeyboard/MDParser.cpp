#include "stdafx.h"
#include "MDParser.h"


CMDParser::CMDParser()
{
}
po::variables_map CMDParser::parseCommandLineArguments(int argc, char *argv[]) {
	po::options_description configFileOptions("Allowed options");
	configFileOptions.add_options()
		("help,h", "Produce help message")
		("version,v", "Print version number")
		("verbose","Print logs")
		("macro,i", po::value<std::string>(), "run given script")
		("record,r", po::value<std::string>(), "record")
		("keyboard,k", "hook keyboard")//done
		("mouse,m", "hook mouse")//done
		("dual,d", "run in dual mode")//done
		("singular,s", "run in sigular mode")
		("ip", po::value<std::string>(), "specify target ip address")
		("port", po::value<std::string>(), "specify target ip address")
		;
	po::variables_map vm;
	auto a = *argv;
	try {
		po::store(po::parse_command_line(argc, argv, configFileOptions), vm);
	}
	catch (std::exception &ex)
	{
		cout << ex.what();
	}
	ifstream ifs("config.cfg");
	po::notify(vm);
	try {
		conflicting_options(vm, "record", "macro");
		conflicting_options(vm, "macro", "keyboard");
		conflicting_options(vm, "macro", "mouse");
		option_dependency(vm, "dual", "ip");
		option_dependency(vm, "dual", "port");
		option_dependency(vm, "singular", "ip");
		option_dependency(vm, "singular", "port");
	}
	catch (std::exception &ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}
	if (vm.count("help")) {
		cout << configFileOptions << "\n";
		exit(0);
	}
	if (vm.count("version"))
	{
		cout << "v.1.0\n" << endl;
		exit(0);
	}
	return vm;
}
void CMDParser::conflicting_options(const po::variables_map& vm,
	const char* opt1, const char* opt2)
{
	if (vm.count(opt1) && !vm[opt1].defaulted()
		&& vm.count(opt2) && !vm[opt2].defaulted())
		throw logic_error(string("Conflicting options '")
			+ opt1 + "' and '" + opt2 + "'.");
}
void CMDParser::option_dependency(const po::variables_map& vm,
	const char* for_what, const char* required_option)
{
	if (vm.count(for_what) && !vm[for_what].defaulted())
		if (vm.count(required_option) == 0 || vm[required_option].defaulted())
			throw logic_error(string("Option '") + for_what
				+ "' requires option '" + required_option + "'.");
}

CMDParser::~CMDParser()
{
}
