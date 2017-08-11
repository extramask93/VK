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
	printf("hello ffrom second thread");
	std::array<uint8_t, 20> request;
	boost::asio::io_service io_service;
	//resolving the name
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(tcp::v4(), ip, port);
	tcp::resolver::iterator iterator = resolver.resolve(query);
	tcp::socket s=tcp::socket(io_service);
	boost::asio::connect(s, iterator);
	while (1)
	{
		std::shared_ptr<IMessage> reportToSend = keyQueue.pop();
		IMessage &message = *reportToSend;
		auto fields = message.getFields();
		request[0] = message.getSize();
		for (int i = 1; i <= request[0]; ++i)
			request[i] = fields[i - 1];

		char request2[10] = {'a','b','c','d','e','f'};
		size_t request_length = 5;
		boost::asio::write(s, boost::asio::buffer(request2, request_length));



		//boost::asio::write(*s, boost::asio::buffer(request, request[0]+1));
		/*char reply[];
		size_t reply_length = boost::asio::read(s,
			boost::asio::buffer(reply, request_length));
		std::cout << "Reply is: ";
		std::cout.write(reply, reply_length);
		std::cout << "\n";*/
	}
}


TCPThread::~TCPThread()
{
}
