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
		("version,v", "Prints version number")
		("verbose","Print logs")
		("macro,i", po::value<std::string>(), "run given script")
		("setip",po::value<std::string>(),"set static ip for the target")
		("record,r", po::value<std::string>(), "record")
		("device,d",po::value<std::string>(),"device to capture: m - mosue, k- keyboard")
		("free,f","Run in free mode")
		("mode,m",po::value<std::string>(),"mode to run in: s - singular(mapped only to remote) or d -dual(mapped to remote and host)")
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
		conflicting_options(vm, "setip", "macro");
		conflicting_options(vm, "free", "setip");
		conflicting_options(vm, "setip", "record");
		conflicting_options(vm, "setip", "device");
		conflicting_options(vm, "macro", "device");
		option_dependency(vm, "mode", "ip");
		option_dependency(vm, "free", "ip");
		option_dependency(vm, "free", "port");
		option_dependency(vm, "mode", "port");
	}
	catch (std::exception &ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}
	if (vm.count("help")) {
		cout << "Usage: \n";
		cout << "VirtualKeyboard -h | --help\n" <<
			"VirtualKeyboard --version\n" <<
			"VirtualKeyboard --ip <board-ip> --port <board-port> (--macro | -i) <macro-file-name>\n" <<
			"VirtualKeyboard --ip <board-ip> --port <board-port> (--free | -f) -d <k|m> -m <s|d\n>" <<
			"VirtualKeyboard --ip <board-ip> --port <board-port> (--record | -r) <output-file-name> -d <k|m> -m <s|d>\n"<<
			"VirtualKeyboard --ip <board-ip> --port <board-port> --setip <0,new-ip-address,new-port,new-mask,new-gateway>\n";
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
std::unique_ptr<IRunMode> CMDArgsParser::SelectRunMode(po::variables_map &vm)
{
		std::unique_ptr<IRunMode> runmode = std::make_unique<NoMode>(NoMode{});
		if (vm.count("free"))
		{
			runmode = std::unique_ptr<FreeRunningMode>{ new FreeRunningMode{ bque,vm } };
		}
		else if (vm.count("macro")) {
			runmode = std::unique_ptr<Macro>{ new Macro{ bque,vm } };
		}
		else if (vm.count("record"))
		{
			runmode = std::unique_ptr<RecordMode>(new RecordMode{ bque,vm });
		}
		else if (vm.count("setip"))
		{
			runmode = std::unique_ptr<ChangeIPMode>{ new ChangeIPMode{ bque,vm } };
		}
		return std::move(runmode);
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
