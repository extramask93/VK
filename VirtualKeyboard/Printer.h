#pragma once
#include "Mouse.h"
#include "HIDKeyboard.h"
#include "BlockingQueue.h"

class Printer
{
public:
	explicit Printer(BlockingQueue<std::shared_ptr<IMessage>> &bq);
	void printer(const Report &rep) const;
	void mousePrinter(const MouseReport &rep) const;
	void verbosePrinter(const Report &rep) const;
	void verboseMousePrinter(const MouseReport &rep) const;
	~Printer();
private:
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
};

