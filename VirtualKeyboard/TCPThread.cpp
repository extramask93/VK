#include "stdafx.h"
#include "TCPThread.h"


TCPThread::TCPThread(std::string ip, std::string port, BlockingQueue<std::shared_ptr<IMessage>> &que) :keyQueue{ que }, ip{ ip }, port{ port } 
{

}

void TCPThread::connect()
{
	printf("connected to: %s:%s",ip,port);
}

void TCPThread::operator()()
{
	std::array<uint8_t, 20> request;
	boost::asio::io_service io_service;
	//resolving the name
	try {
		tcp::resolver resolver(io_service);
		tcp::resolver::query query(tcp::v4(), ip, port);
		tcp::resolver::iterator iterator = resolver.resolve(query);
		tcp::socket s = tcp::socket(io_service);
		boost::asio::connect(s, iterator);

		while (1)
		{
			std::shared_ptr<IMessage> reportToSend = keyQueue.pop();
			IMessage &message = *reportToSend;
			auto fields = message.getFields();
			request[0] = message.getSize();
			for (int i = 1; i <= request[0]; ++i)
				request[i] = fields[i - 1];
			boost::asio::write(s, boost::asio::buffer(request, request[0] + 1));
			char reply[3];
			size_t reply_length = boost::asio::read(s,
				boost::asio::buffer(reply, 3));
			std::cout << "Reply is: ";
			std::cout.write(reply, reply_length);
			std::cout << "\n";
		}
	}
	catch (std::exception const &ex)
	{
		std::cerr << ex.what();
		exit(1);
	}
}


TCPThread::~TCPThread()
{
}
