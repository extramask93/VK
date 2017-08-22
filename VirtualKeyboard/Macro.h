#pragma once
#include "ParseException.h"
#include "ThreadPrinter.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>
#include <boost\thread.hpp>
#include <map>
#include <memory>
#include "BlockingQueue.h"
#include "IMessage.h"
#include "IMouse.h"
#include "IRunMode.h"
#include "HIDKeyboard.h"
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\string.hpp>
#include "maps.h"
typedef Report KeyReport;
enum class Format
{
	Short,
	Long,
};
class Macro:public IRunMode
{
public:
	Macro(BlockingQueue<std::shared_ptr<IMessage>> &que);
	int load(std::string const fname);
	void sleepS(std::string  l);
	void sleepMS(std::string l);
	void keyboardPush(std::string l);
	size_t keyboardRelease(std::string l);
	void keyboardReleaseAll(std::string l);
	size_t mousePush(std::string);
	void mouseReleaseAll(std::string);
	size_t mouseRelease(std::string);
	void mouseMove(std::string);
	std::vector<std::string> lines;
	std::map<std::string, std::function<void(std::string)>> mp;
	// Odziedziczono za poœrednictwem elementu IRunMode
	virtual void Run() override;
	~Macro();
private:
	void ReadFileInLongFormat();
	void RunLongMacro();
	void ReadFileInShortFormat();
	void RunShortMacro();
	uint8_t KeyCodeFromString(std::string s);
	std::vector<std::string> tokens;
	Format ParseHeader(std::string header);
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	KeyReport keyReport;
	std::fstream file;
	std::string buffer;
	MouseReport mouseReport;
	uint8_t isAModifier(uint8_t k);
	uint8_t isNonPrintable(uint8_t k);
	uint8_t isPrintableReachedWithShift(uint8_t k);
	uint8_t stripModifiers(uint8_t k);
	KeyReport *addToReport(uint8_t k);
	KeyReport* subtractFromReport(uint8_t k);


};

