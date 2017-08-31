#include "stdafx.h"
#include "CMDArgsParser.h"


CMDArgsParser::CMDArgsParser()
{
}
po::variables_map CMDArgsParser::parseCommandLineArguments(int argc, char *argv[]) const
{
	std::ifstream configFile{ "config.ini",std::ifstream::in };
	po::options_description configFileOptions("Allowed options");
	configFileOptions.add_options()
		("help,h", "Produce help message")
		("version,v", "Print version number")
		("verbose","Print logs")
		("macro,i", po::value<std::string>(), "run given script")
		("setip",po::value<std::string>(),"set static ip for the target")
		("record,r", po::value<std::string>(), "record")
		("free,f","Run in free mode")
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
		po::store(po::parse_config_file(configFile,configFileOptions),vm);
		po::store(po::parse_command_line(argc, argv, configFileOptions), vm);
	}
	catch (std::exception &ex)
	{
		throw std::runtime_error{ "Error: Urecognised Option!" };
	}
	po::notify(vm);
	try {
		conflicting_options(vm, "macro", "free");
		conflicting_options(vm, "record", "free");
		conflicting_options(vm, "record", "macro");
		conflicting_options(vm, "macro", "keyboard");
		conflicting_options(vm, "setip", "macro");
		conflicting_options(vm, "free", "setip");
		conflicting_options(vm, "setip", "record");
		conflicting_options(vm, "setip", "keyboard");
		conflicting_options(vm, "setip", "mouse");
		conflicting_options(vm, "macro", "mouse");
		option_dependency(vm, "dual", "ip");
		option_dependency(vm, "free", "ip");
		option_dependency(vm, "free", "port");
		option_dependency(vm, "dual", "port");
		option_dependency(vm, "singular", "ip");
		option_dependency(vm, "singular", "port");
		option_dependency(vm, "setmask", "setgate");
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
void CMDArgsParser::conflicting_options(const po::variables_map& vm,
	const char* opt1, const char* opt2)
{
	if (vm.count(opt1) && !vm[opt1].defaulted()
		&& vm.count(opt2) && !vm[opt2].defaulted())
		throw logic_error(string("Conflicting options '")
			+ opt1 + "' and '" + opt2 + "'.");
}
void CMDArgsParser::option_dependency(const po::variables_map& vm,
	const char* for_what, const char* required_option)
{
	if (vm.count(for_what) && !vm[for_what].defaulted())
		if (vm.count(required_option) == 0 || vm[required_option].defaulted())
			throw logic_error(string("Option '") + for_what
				+ "' requires option '" + required_option + "'.");
}

CMDArgsParser::~CMDArgsParser()
{
}
