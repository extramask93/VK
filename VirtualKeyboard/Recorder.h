#pragma once
#include "HIDKeyboard.h"
#include <string>
#include <stddef.h>
#include <fstream>
class Recorder
{
public:
	Recorder(std::string filename, size_t bfsize) :filename{ filename }, maxReportCount{ bfsize } {}
	Recorder();
	~Recorder();
	void operator()(const Report &report);
	void saveToDisc();
private:
	std::vector<Report> reportVector;
	std::string filename;
	size_t maxReportCount;
};

