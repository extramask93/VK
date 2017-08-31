// VirtualKeyboard.cpp: Okreœla punkt wejœcia dla aplikacji konsoli.
//
#include "stdafx.h"
#include "boost/program_options.hpp" 
#include "IRunMode.h"
#include "FreeRunningMode.h"
#include "BlockingQueue.h"
#include "Macro.h"
#include "TCPThread.h"
#include "CMDArgsParser.h"
#include "ChangeIPMode.h"
#include <string>
#include <iostream>
#include "Parser.h"
#include "RecordMode.h"
#include "ParseException.h"
#include "NoMode.h"

namespace po = boost::program_options;
using namespace std;

BlockingQueue<std::shared_ptr<IMessage>> bque{ 50 };
int main(int argc, char *argv[])
{
	std::unique_ptr<TCPThread> sendThread;
	std::unique_ptr<IRunMode> runmode = std::make_unique<NoMode>(NoMode{});
	CMDArgsParser parser;
	try {
		auto vm = parser.parseCommandLineArguments(argc, argv);
		if(vm.count("free"))
		{
			runmode = std::unique_ptr<FreeRunningMode>{ new FreeRunningMode{bque,vm} };
		}
		else if (vm.count("macro")) {
			runmode = std::unique_ptr<Macro>{ new Macro{ bque,vm } };
		}
		else if(vm.count("record"))
		{
			runmode = std::unique_ptr<RecordMode>(new RecordMode{bque,vm});
		}
		else if (vm.count("setip"))
		{
			runmode = std::unique_ptr<ChangeIPMode>{ new ChangeIPMode{ bque,vm } };
		}
		if(runmode->RequiresConnection()){
			sendThread = std::unique_ptr<TCPThread>{new TCPThread { vm["ip"].as<std::string>(),vm["port"].as<std::string>(),bque}};
			while (sendThread->getState() == State::none)
				std::this_thread::sleep_for(1s);
			if (sendThread->getState() == State::disconnected)
			exit(2);
		}
		runmode->Run();
		if(sendThread!=nullptr) sendThread->closeConnection();
	}
	catch(ParseException const &ex)
	{
		std::cerr << ex.what() << std::endl;
		system("PAUSE");
		exit(2);
	}
	catch (std::exception &ex) {
		std::cerr << ex.what();
		exit(3);
	}
	catch(...)
	{
		exit(3);
	}
	system("PAUSE");
    return 0;
}