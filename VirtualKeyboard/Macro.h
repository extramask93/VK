#pragma once
#include <vector>
#include <functional>
#include <memory>
#include "BlockingQueue.h"
#include "IMessage.h"
#include "IRunMode.h"
#include "boost/program_options.hpp"
#include "ShortParser.h"
#include "LongParser.h"

using CallbackType = std::function<void()>;
//! Mode used to run macro files
class Macro:public IRunMode
{
public:
	/*! @param[in] que Message queue used to send by TCP Thread
		@param[in] vm Map containing supplied command  options*/
	explicit Macro(BlockingQueue<std::shared_ptr<IMessage>> &que,boost::program_options::variables_map const &vm);
	int Load(std::string const &fname);//!< Loads macro file line by line to an internal vector
	virtual void Run() override;//!< Runs the macro
	~Macro();
private:
	void LoadFile(std::string name);
	std::vector<std::string> lines;
	std::vector<CallbackType> requests;
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	LongParser *longParser;
	ShortParser *shortParser;
};

