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

class Macro:public IRunMode
{
public:
	explicit Macro(BlockingQueue<std::shared_ptr<IMessage>> &que,boost::program_options::variables_map const &vm);
	int Load(std::string const &fname);
	virtual void Run() override;
	~Macro();
private:
	void LoadFile(std::string name);
	std::vector<std::string> lines;
	std::vector<CallbackType> requests;
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	LongParser *longParser;
	ShortParser *shortParser;
};

