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
#include "MDParser.h"
#include "ChangeIPMode.h"
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

void printer(const Report &rep);
void mousePrinter(const MouseReport &rep);
void verbosePrinter(const Report &rep);
void verboseMousePrinter(const MouseReport &rep);
///////
int main(int argc, char *argv[])
{
	try {
		Macro macro{ bque };
		ChangeIPMode changeIPMode{};
		IRunMode *runmode = new FreeRunningMode{};
		CMDParser parser;
		auto vm = parser.parseCommandLineArguments(argc, argv);
		auto con1 = hidkbd.connect(printer);
		auto con2 = mouse.connect(mousePrinter);
		runmode->keyhookproc = LowLevelProcNoOp;
		runmode->mousehookproc = LowLevelProcNoOp;
		if (vm.count("verbose"))
		{
			con1.disconnect();
			con2.disconnect();
			hidkbd.connect(verbosePrinter);
			mouse.connect(verboseMousePrinter);
		}
		if (vm.count("dual"))
		{
			if (vm.count("mouse"))
				runmode->mousehookproc = &LowLevelMouseProcDual;
			if (vm.count("keyboard"))
				runmode->keyhookproc = &LowLevelKeyboardProcDual;
		}
		else if (vm.count("singular")) {
			if (vm.count("mouse"))
			{
				runmode->mousehookproc = &LowLevelMouseProc;
			}
			if (vm.count("keyboard"))
			{
				runmode->keyhookproc = &LowLevelKeyboardProc;
			}
		}
		else if (vm.count("macro")) {
			try {
				macro.load(vm["macro"].as<std::string>());
				runmode = &macro;
			}
			catch (std::exception &ex) {
				cerr << ex.what() << endl;
				return 1;
			}
		}
		else if (vm.count("setip"))
		{
			try {
				std::vector<std::string> specs;
				boost::split(specs, vm["setip"].as<std::string>(), boost::is_any_of(","), boost::token_compress_off);
				changeIPMode.ParseAddress(boost::lexical_cast<uint16_t>(specs[0]),specs[1],boost::lexical_cast<uint16_t>(specs[2]),specs[3],specs[4]);
				runmode = &changeIPMode;
			}
			catch (std::exception &ex)
			{
				PrintThread{} << ex.what();
				exit(1);
			}
		}
		else if (vm.count("record"))
		{
			PrintThread{} << "Recording not yet available\n";
			return 1;
		}
		TCPThread sendThread{ vm["ip"].as<std::string>(),vm["port"].as<std::string>(),bque };
		bque.clear();
		while (sendThread.getState() == State::none)
			std::this_thread::sleep_for(1s);
		if (sendThread.getState() == State::disconnected)
			exit(2);
		else
			runmode->Run();
		
		//sendThread.internalThread.interrupt();
	}
	catch (std::exception &ex) {
		std::cerr << ex.what();
		return 1;
	}
    return 0;
}






void verbosePrinter(const Report &rep)
{
	char buffer[56];
	auto tp = make_shared<Report>(rep);
	bque.push(tp);
	sprintf_s(buffer,56,"id: %i modifiers: %02x [%02x,%02x,%02x,%02x,%02x,%02x]\n",rep.id,rep.modifiers,rep.keys[0],rep.keys[1],rep.keys[2],rep.keys[3],rep.keys[4],rep.keys[5]);
	PrintThread{} << buffer;
}

void printer(const Report &rep)
{
	auto tp = make_shared<Report>(rep);
	bque.push(tp);
}
void verboseMousePrinter(const MouseReport &rep) {
	auto tp = make_shared<MouseReport>(rep);
	bque.push(tp);
	PrintThread{} << "id: "<<std::to_string(rep.id) << " buttons: " << std::to_string(rep.buttons) << " X: " << std::to_string(rep.X) << " Y: " << std::to_string( rep.Y )<< " Wheel: " << std::to_string (rep.Wheel) << '\n';
}
void mousePrinter(const MouseReport &rep)
{
	auto tp = make_shared<MouseReport>(rep);
	bque.push(tp);
}