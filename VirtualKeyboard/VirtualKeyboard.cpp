// VirtualKeyboard.cpp: Okre�la punkt wej�cia dla aplikacji konsoli.
//

#include "stdafx.h"
#include "boost/program_options.hpp" 
#include "IRunMode.h"
#include "FreeRunningMode.h"
#include "Keyboard.h"
#include "HIDMouse.h"
#include "HIDKeyboard.h"
#include "BlockingQueue.h"
#include "TCPThread.h"
#include "KeyboardTranslator.h"
#include <string>
#include <iostream>


namespace po = boost::program_options;
using namespace std;
IRunMode *runmode;
HIDKeyboard hidkbd;
KeyboardTranslator tr;
Keyboard keyboard{ tr,hidkbd };
HIDMouse hidMouse;
Mouse mouse{hidMouse};
BlockingQueue<std::shared_ptr<IMessage>> bque{ 50 };
TCPThread sendThread{ "localhost","21",bque };

void printer(const Report &rep)
{
	auto tp = make_shared<Report>(rep);
	bque.push(tp);
	printf("%i %02x [%02x %02x %02x %02x %02x %02x]\n", rep.id, rep.modifiers, rep.keys[0], rep.keys[1], rep.keys[2], rep.keys[3], rep.keys[4], rep.keys[5]);
}
void mousePrinter(const MouseReport &rep)
{
	printf("id: %i buttons: %i X: %i Y: %i Wheel: %i\n",rep.id,rep.buttons,rep.X,rep.Y,rep.Wheel);
}
int main(int argc,char *argv[])
{
	hidkbd.connect(printer);
	mouse.connect(mousePrinter);
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
	else {
		cout << "Script file not specified. Running record mode\n";
		runmode = new FreeRunningMode{};
	}
	boost::thread{ sendThread };
	runmode->Run();
	delete runmode;
    return 0;
}

