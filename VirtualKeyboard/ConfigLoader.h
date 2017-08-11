#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <map>
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
	ConfigLoader();
	void load(std::string const &name);
	~ConfigLoader(void);
	std::map<string, string> options;


};


