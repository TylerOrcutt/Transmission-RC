#pragma once
#include <iostream>
#include <fstream>
#include<map>
#include<stdlib.h>
namespace TransmissionRC::Config{
	 static std::string cfgPath = std::string(getenv("HOME")) + "/.config/trc.conf";

	std::map<std::string,std::string> loadConfig();
	void makeConfig();
}


