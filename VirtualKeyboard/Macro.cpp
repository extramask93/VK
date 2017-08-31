#include "stdafx.h"
#include "Macro.h"
#include "Parser.h"
#include "ShortParser.h"
#include "LongParser.h"
#include "ParseException.h"
#include "maps.h"
#include <fstream>


Macro::Macro(BlockingQueue<std::shared_ptr<IMessage>> &que, boost::program_options::variables_map const &vm) :  bque{ que }
{
	Load(vm["macro"].as<std::string>());
}

int Macro::Load(std::string const &fname)
{

		int lineFlag = 0;
		int counter = 0;
		Parser *parser;
		longParser = new LongParser{};
		shortParser = new ShortParser{};
		parser = longParser;
		int i = -1;
		LoadFile(fname);
		try{
			for (i = 0; i < lines.size(); i++)
			{
				if (lines[i] == SHORT_FLAG)
					parser = shortParser;
				else if (lines[i] == LONG_FLAG)
					parser = longParser;
				else if (parser->ParseLoop(lines[i]))
				{
					lineFlag = i;
					counter = parser->GetNumberOfLoopRepetitions();
				}
				else if (lines[i] == LOOP_END_FLAG)
				{
					counter--;
					(counter > 0) ? i = lineFlag : 1;
				}
				else
				{
					auto functionsToCall = parser->Parse(lines[i]);
					requests.insert(requests.end(),functionsToCall.begin(),functionsToCall.end());
				}
			}
		}
		catch(ParseException const &ex)
		{
			throw ParseException{ "Line " + std::to_string(i + 1) + ':' + ex.what()};
		}
		catch(...)
		{
			throw ParseException{"Unknown Error."};
		}
	return 0;
}


Macro::~Macro()
{
	delete longParser;
	delete shortParser;
}

void Macro::LoadFile(std::string name)
{
	try {
		std::ifstream file;
		std::string buffer;
		file.open(name, std::fstream::in);
		if (!file.is_open())
		{
			throw std::runtime_error{ "Error: File "+name+" does not exist"};
		}
		while (std::getline(file, buffer))
		{
			boost::trim(buffer);
			if (buffer.empty())
				continue;
			lines.push_back(buffer);
		}
		file.close();
	}
	catch(std::exception const &ex)
	{
		throw std::runtime_error{ "Error: unable to load " + name + " file." };
	}
}

void Macro::Run()
{
	for (auto request : requests) {
		request();
		boost::this_thread::sleep(boost::posix_time::millisec(10));
	}
}