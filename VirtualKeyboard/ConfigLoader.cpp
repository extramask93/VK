#include "stdafx.h"
#include "ConfigLoader.h"


ConfigLoader::ConfigLoader(std::string const &name) :fileName{ name } {
}
Config ConfigLoader::load()
{
	fstream configStream;
	configStream.open(fileName, ios_base::in);
	configStream >> config.ip >> config.ip;
	configStream >> config.port >> config.port;
	configStream.close();
	return config;
}
ConfigLoader::~ConfigLoader()
{

}

