// VirtualKeyboard.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "boost/program_options.hpp" 
#include "IRunMode.h"
#include "FreeRunningMode.h"
#include "Keyboard.h"
#include "HIDMouse.h"
#include "HIDKeyboard.h"
#include "BlockingQueue.h"
#include "ConfigLoader.h"
#include "Macro.h"
#include "TCPThread.h"
#include "KeyboardTranslator.h"
#include <string>
#include <iostream>


namespace po = boost::program_options;
using namespace std;
HIDKeyboard hidkbd;
KeyboardTranslator tr;
Keyboard keyboard{ tr,hidkbd };
HIDMouse hidMouse;
Mouse mouse{hidMouse};
BlockingQueue<std::shared_ptr<IMessage>> bque{ 50 };
///////
po::variables_map parseCommandLineArguments(int argc, char *argv[]);
void option_dependency(const po::variables_map& vm,
	const char* for_what, const char* required_option);
void conflicting_options(const po::variables_map& vm,
	const char* opt1, const char* opt2);
void printer(const Report &rep);
void mousePrinter(const MouseReport &rep);
///////
int main(int argc, char *argv[])
{
	IRunMode &runmode = FreeRunningMode{};
	//auto vm = parseCommandLineArguments(argc, argv);
	//po::options_description generic("Generic options");
	//generic.add_options()
	po::options_description configFileOptions("Allowed options");
	configFileOptions.add_options()
		("help,h", "Produce help message")
		("version,v", "Print version number")
		("macro,i", po::value<std::string>(), "run given script")
		("record,r", po::value<std::string>(), "record")
		("keyboard,k", "hook keyboard")//done
		("mouse,m", "hook mouse")//done
		("dual,d", "run in dual mode")//done
		("singular,s", "run in sigular mode")
		("ip", po::value<std::string>(), "specify target ip address")
		("port", po::value<std::string>(), "specify target ip address")
		;
	//po::options_description cmdOptions{};
	//cmdOptions..add(configFileOptions);
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
	//po::store(po::parse_config_file(ifs, configFileOptions), vm);
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
	hidkbd.connect(printer);
	mouse.connect(mousePrinter);
	runmode.keyhookproc = LowLevelProcNoOp;
	runmode.mousehookproc = LowLevelProcNoOp;
	if (vm.count("dual"))
	{
		if (vm.count("mouse"))
			runmode.mousehookproc = &LowLevelMouseProcDual;
		if (vm.count("keyboard"))
			runmode.keyhookproc = &LowLevelKeyboardProcDual;
	}
	else if(vm.count("singular")) {
		if (vm.count("mouse"))
		{
			runmode.mousehookproc = &LowLevelMouseProc;
		}
		if (vm.count("keyboard"))
		{
			runmode.keyhookproc = &LowLevelKeyboardProc;
		}
	}
	else if(vm.count("macro")) {
		try {
			Macro macro{ bque };
			macro.load(vm["macro"].as<std::string>());
			runmode = macro;
		}
		catch (std::exception &ex) {
			cerr << ex.what() << endl;
			return 1;
		}
	}
	else if (vm.count("record"))
	{
		cout << "Record in not yet available" << endl;
		return 1;//TODO
	}
	TCPThread sendThread{ vm["ip"].as<std::string>(),"21",bque};
	while (sendThread.getState() == State::none);
	if (sendThread.getState() == State::disconnected)
		exit(2);
	else 
		runmode.Run();
    return 0;
}





po::variables_map parseCommandLineArguments(int argc, char *argv[]) {
	//po::options_description generic("Generic options");
	//generic.add_options()
	po::options_description configFileOptions("Allowed options");
	configFileOptions.add_options()
		("help,h", "Produce help message")
		("version,v", "Print version number")
		("macro,i", po::value<std::string>(), "run given script")
		("record,r", po::value<std::string>(), "record")
		("keyboard,k", "hook keyboard")//done
		("mouse,m", "hook mouse")//done
		("dual,d", "run in dual mode")//done
		("singular,s","run in sigular mode")
		("ip",po::value<std::string>(),"specify target ip address")
		("port",po::value<int>(),"specify target port")
		;
	//po::options_description cmdOptions{};
	//cmdOptions..add(configFileOptions);
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
	//po::store(po::parse_config_file(ifs, configFileOptions), vm);
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
		cout <<configFileOptions << "\n";
		 exit(0);
	}
	if (vm.count("version"))
	{
		cout << "v.1.0\n" << endl;
		exit(0);
	}
	return vm;
}
void conflicting_options(const po::variables_map& vm,
	const char* opt1, const char* opt2)
{
	if (vm.count(opt1) && !vm[opt1].defaulted()
		&& vm.count(opt2) && !vm[opt2].defaulted())
		throw logic_error(string("Conflicting options '")
			+ opt1 + "' and '" + opt2 + "'.");
}
void option_dependency(const po::variables_map& vm,
	const char* for_what, const char* required_option)
{
	if (vm.count(for_what) && !vm[for_what].defaulted())
		if (vm.count(required_option) == 0 || vm[required_option].defaulted())
			throw logic_error(string("Option '") + for_what
				+ "' requires option '" + required_option + "'.");
}
void printer(const Report &rep)
{
	auto tp = make_shared<Report>(rep);
	bque.push(tp);
	printf("%i %02x [%02x %02x %02x %02x %02x %02x]\n", rep.id,rep.modifiers, rep.keys[0], rep.keys[1], rep.keys[2], rep.keys[3], rep.keys[4], rep.keys[5]);
}
void mousePrinter(const MouseReport &rep)
{
	auto tp = make_shared<MouseReport>(rep);
	bque.push(tp);
	printf("id: %i buttons: %i X: %i Y: %i Wheel: %i\n", rep.id, rep.buttons, rep.X, rep.Y, rep.Wheel);
}