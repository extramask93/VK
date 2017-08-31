#pragma once
#include "Mouse.h"
#include "BlockingQueue.h"
#include "boost\algorithm\string.hpp"
#include "boost\lexical_cast.hpp"
#include "boost\numeric\conversion\cast.hpp"
#include "ParseException.h"
extern BlockingQueue<std::shared_ptr<IMessage>> bque;
class MouseAdapter
{
public:
	MouseAdapter();
	~MouseAdapter();
	size_t mousePush(std::string);
	void mouseReleaseAll(std::string);
	size_t mouseRelease(std::string);
	void mouseMove(std::string);
	void mouseClick(std::string);
private:
	MouseReport mouseReport;

};

