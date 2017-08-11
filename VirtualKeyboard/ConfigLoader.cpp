#include "stdafx.h"
#include "ConfigLoader.h"


ConfigLoader::ConfigLoader() {
}
void ConfigLoader::load(std::string const &name)
{
	options.clear();
	fstream configStream;
	configStream.open(name, ios_base::in);
	while (!configStream.eof())
	{
		std::string key;
		std::string val;
		configStream >> key >> val;
		options[key] = val;
	}
	configStream.close();
}
ConfigLoader::~ConfigLoader()
{

}

