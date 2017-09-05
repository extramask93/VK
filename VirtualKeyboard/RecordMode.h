#pragma once
#include "IRunMode.h"
#include <string>
#include "FreeRunningMode.h"
#include "boost/algorithm/string.hpp"
#include "KeyboardAdapter.h"
#include <fstream>
#include <sstream>
#define LONG_FLAG "[LONG]"
#define SHORT_FLAG "[SHORT]"
//! Mode in which one could record events sent to remote device
class RecordMode :
	public FreeRunningMode
{
public:
	/*! @param[in] que Message queue used to send by TCP Thread
		@param[in] vm Map containing supplied command  options*/
	RecordMode(BlockingQueue<std::shared_ptr<IMessage>> &que,boost::program_options::variables_map const &vm);
	RecordMode(RecordMode const &r);
	void Run() override;
	~RecordMode();
private:
	std::stringstream buffer;
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	std::string fileName;
	std::ofstream file;
	uint8_t scan2ascii(DWORD scancode);
	void saveMouse(MouseReport report, std::string name);
	void saveKeyboard(uint32_t, std::string name);
};

