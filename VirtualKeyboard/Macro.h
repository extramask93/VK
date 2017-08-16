#pragma once
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
#define KEY_UP_ARROW    0xDA
#define KEY_DOWN_ARROW    0xD9
#define KEY_LEFT_ARROW    0xD8
#define KEY_RIGHT_ARROW   0xD7
#define KEY_BACKSPACE   0xB2
#define KEY_TAB       0xB3
#define KEY_RETURN      0xB0
#define KEY_ESC       0xB1
#define KEY_INSERT      0xD1
#define KEY_DELETE      0xD4
#define KEY_PAGE_UP     0xD3
#define KEY_PAGE_DOWN   0xD6
#define KEY_HOME      0xD2
#define KEY_END       0xD5
#define KEY_CAPS_LOCK   0xC1
#define KEY_F1        0xC2
#define KEY_F2        0xC3
#define KEY_F3        0xC4
#define KEY_F4        0xC5
#define KEY_F5        0xC6
#define KEY_F6        0xC7
#define KEY_F7        0xC8
#define KEY_F8        0xC9
#define KEY_F9        0xCA
#define KEY_F10       0xCB
#define KEY_F11       0xCC
#define KEY_F12       0xCD
#define NR_OF_KEYS	  6
typedef Report KeyReport;
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
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
	KeyReport keyReport;
	MouseReport mouseReport;
	uint8_t isAModifier(uint8_t k);
	uint8_t isNonPrintable(uint8_t k);
	uint8_t isPrintableReachedWithShift(uint8_t k);
	uint8_t stripModifiers(uint8_t k);
	KeyReport *addToReport(uint8_t k);
	KeyReport* subtractFromReport(uint8_t k);


};

