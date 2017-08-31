#include "stdafx.h"
#include "TCPThread.h"
#include "ThreadPrinter.h"


TCPThread::TCPThread(std::string ip, std::string port, BlockingQueue<std::shared_ptr<IMessage>> &que) :keyQueue{ que }, ip{ ip }, port{ port },socket{nullptr}
{
	try {
		state = State::none;
		stateMutex = std::shared_ptr<boost::mutex>{ new boost::mutex{} };
		internalThread = boost::thread(&TCPThread::operator(), this);
	}
	catch(...)
	{
		std::cerr << "Error occurred during TCP Thread creation";
		exit(1);
	}

}

void TCPThread::connect()
{
	PrintThread{} << "Connecting to: " << ip << ": " << port << '\n';
	tcp::resolver resolver(io_service);
	tcp::resolver::query query(tcp::v4(), ip, port);
	tcp::resolver::iterator iterator = resolver.resolve(query);
	socket= std::make_shared<tcp::socket>(io_service);
	boost::asio::connect(*socket, iterator);
	setState(State::waitingForHandshake);
}
void TCPThread::decodeMessage()
{
	auto reportToSend = keyQueue.pop();
	IMessage &message = *reportToSend;
	auto fields = message.getFields();
	request[0] = message.getSize();
	if (fields[0] == DISCONNECT_REQ)
		state = State::disconnected;
	for (int i = 1; i <= request[0]; ++i)
		request[i] = fields[i - 1];
}

void TCPThread::writeRequest()
{
	boost::asio::write(*socket, boost::asio::buffer(request, request[0] + 1));
}

void TCPThread::getResponse()
{
	size_t reply_length = boost::asio::read(*socket,
		boost::asio::buffer(response, 2));
}

State TCPThread::getState() const
{
	return state;
}

void TCPThread::setState(State state_)
{
	stateMutex->lock();
	state = state_;
	stateMutex->unlock();
}

void TCPThread::operator()()
{
	try {
		connect();
		performHandshake();
		logIn("root", "root");
		while (getState() != State::disconnected)
		{
			decodeMessage();
			writeRequest();
			getResponse();
			handleResponse(2);
		}
	}
	catch(std::exception const &ex)
	{
		setState(State::disconnected);
		std::cerr << ex.what() << std::endl;
		exit(1);
	}
	catch(...)
	{
		std::cerr << "Unknown error occurred during connection\n";
		exit(1);
	}
}


TCPThread::~TCPThread()
{
	setState(State::disconnected);
	if(socket->is_open())
		socket->close();
	internalThread.join();
}

void TCPThread::handleResponse(size_t len)
{
	uint8_t buffer[2];
	static int failures=0;
	if (response[1] == DISCONNECT_REQ && getState()!=State::disconnected)
	{
		PrintThread{}<<("Disconnect requested \n");
		buffer[0] = 0x01; buffer[1] = DISCONNECT_REQ;
		boost::asio::write(*socket, boost::asio::buffer(buffer, 2));

	}
	if (response[1] == REQUEST_FAILED)
	{
		if(++failures > 10) throw(new std::exception("Command failed more than 10 times. Closing the connection"));
	}
}

void TCPThread::performHandshake()
{
	PrintThread{}<<("Performing handshake...\n");
	uint8_t buffer[2] = { 0x01,PROTOCOL_VER };
	uint8_t replyBuffer[2];
	boost::asio::write(*socket, boost::asio::buffer(buffer, 2));
	size_t reply_length = boost::asio::read(*socket,
		boost::asio::buffer(replyBuffer, 2));
	if (replyBuffer[1] == UNSUPORTED_PROTOCOL) {
		setState(State::disconnected);
		throw(std::exception("Handshake failed due to unsuported protocol version"));
	}
	else
		setState(State::connected);
}

void TCPThread::logIn(const std::string & username, const std::string & password)
{
	PrintThread{}<<("Loggin in...\n");
	std::string total = username + ',' + password;
	std::vector<unsigned char> buffer;
	buffer.push_back(total.size() + 2);
	buffer.push_back(0x10);
	for each (auto c in total)
	{
		buffer.push_back(c);
	}
	buffer.push_back('\0');
	uint8_t replyBuffer[2];
	boost::asio::write(*socket, boost::asio::buffer(buffer, buffer[0]));
	size_t reply_length = boost::asio::read(*socket,
		boost::asio::buffer(replyBuffer, 2));
	if (replyBuffer[1] != LOGIN_OK) {
		setState(State::disconnected);
		throw(new std::exception{ "Login failed" });
	}
	else
		setState(State::logged);
	PrintThread{} << "Ready\n";
}

void TCPThread::closeConnection() 
{
	keyQueue.push(std::make_shared<DisconnectMessage>(dmessage));
}
