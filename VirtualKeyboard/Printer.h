#pragma once
#include "Mouse.h"
#include "HIDKeyboard.h"
#include "BlockingQueue.h"
#include "ThreadPrinter.h"
//! Encapsulates verbose printers and putting messages to BlockingQueue
class Printer
{
public:
	explicit Printer(BlockingQueue<std::shared_ptr<IMessage>> &bq);
	void printer(const Report &rep) const;//!< Sends IMessage's to TCP Thread
	void mousePrinter(const MouseReport &rep) const;//!<  sends IMessage's to TCP Thread
	void verbosePrinter(const Report &rep) const;//!< Prints and sends IMessage's to TCP Thread
	void verboseMousePrinter(const MouseReport &rep) const;//!< Prints and sends IMessage's to TCP Thread
	~Printer();
private:
	BlockingQueue<std::shared_ptr<IMessage>> &bque;
};

