#pragma once
#include <boost\thread\thread.hpp>
#include "BlockingQueue.h"
#include "HIDKeyboard.h"
#include "IMessage.h"
#include <memory>
#include <boost\asio.hpp>
#include <functional>
#define PROTOCOL_VER 0x01
#define LOGIN_REQ 0x10
#define KEYBOARD_WRITE_REQ 0x30
#define MOUSE_WRITE_REQ 0x40
#define DISCONNECT_REQ 0xF0
#define UNSUPORTED_PROTOCOL 0xF1
#define LOGIN_FAILED 0xF2
#define LOGIN_OK 0xF3
#define REQUEST_FAILED 0xF4
#define REQUEST_OK 0xF5
#define USER_ID "root,root"

using boost::asio::ip::tcp;
struct DisconnectMessage: IMessage
{
	uint8_t getSize() override { return 1; }
	uint8_t getPacketId() override { return DISCONNECT_REQ; }
	std::vector<uint8_t> getFields() override { return std::vector<uint8_t>{getPacketId()}; }
	~DisconnectMessage()override{}
};

enum class State {
	none,
	disconnected,
	connected,
	logged,
	waitingForHandshake
};

class TCPThread
{
public:
	TCPThread(std::string ip, std::string port, BlockingQueue<std::shared_ptr<IMessage>> &que);
	void connect();
	volatile State state;
	State getState() const;
	void setState(State state_);
	void operator()();
	~TCPThread();
	boost::thread internalThread;
	void closeConnection();
private:
	BlockingQueue<std::shared_ptr<IMessage>> &keyQueue;
	DisconnectMessage dmessage;
	std::function<void(std::string)> print;
	boost::asio::io_service io_service;
	std::array<uint8_t, 30> request;
	std::array<uint8_t, 2> response;
	void decodeMessage();
	void writeRequest();
	void getResponse();
	void handleResponse(size_t len);
	std::shared_ptr<boost::mutex> stateMutex;
	void performHandshake();
	void logIn(const std::string &username, const std::string &password);
	std::string ip;
	std::string port;
	std::shared_ptr<tcp::socket> socket;
};

