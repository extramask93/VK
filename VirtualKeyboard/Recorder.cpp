#include "stdafx.h"
#include "Recorder.h"


Recorder::Recorder()
{
}


Recorder::~Recorder()
{
	saveToDisc();
}

void Recorder::operator()(const Report & report)
{
	reportVector.push_back(report);
	if (reportVector.size() > maxReportCount)
		saveToDisc();
}

void Recorder::saveToDisc()
{
	std::fstream file{};
	file.open(filename,std::fstream::out||std::fstream::binary||std::fstream::app);
	for (auto &report : reportVector)
	{
		auto f=report.getFields();
		char * buffer = reinterpret_cast<char*>(&f[0]);
		file.write(buffer,1);
		for (int i = 1; i <= report.getSize(); i++)
		{
			buffer = reinterpret_cast<char*>(&f[i]);
			file.write(buffer, 1);
		}
	}
	file.close();
}
