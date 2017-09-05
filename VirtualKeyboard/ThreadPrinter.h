#pragma once
#include <mutex>
#include <iostream>
#include <functional>
#include <sstream>

//! Class used to concurrent printing to standard output
class PrintThread : public std::ostringstream
{
public:
	PrintThread() = default;

	~PrintThread()
	{
		std::lock_guard<std::mutex> guard(mutexPrint);
		std::cout << this->str();
	}

private:
	static std::mutex mutexPrint;
};

