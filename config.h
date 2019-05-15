#pragma once
#include <iostream>
#include <fstream>
#include<map>
#include<stdlib.h>
namespace TransmissionRC::Config{
	extern std::map<std::string,std::string> config;
	extern std::string sessionID;
	std::map<std::string,std::string>  loadConfig();
}


