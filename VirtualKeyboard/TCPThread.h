#pragma once
#include <boost\thread\thread.hpp>
#include "BlockingQueue.h"
#include "HIDKeyboard.h"
#include "IMessage.h"
#include <memory>
#include <boost\asio.hpp>
using boost::asio::ip::tcp;
class TCPThread
{
public:
	TCPThread(std::string ip, std::string port, BlockingQueue<std::shared_ptr<IMessage>> &que);
	void connect();
	void operator()();
	~TCPThread();
	BlockingQueue<std::shared_ptr<IMessage>> &keyQueue;
private:
	
	std::string ip;
	std::string port;
};
