#pragma once
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#define FILE_NAME "config.txt"
struct Config
{
	std::string ip;
	std::string port;
};
class ConfigLoader
{
public:
	ConfigLoader(std::string const &name);
	Config load();
	~ConfigLoader(void);
private:
	std::string fileName;
	Config config;


};


