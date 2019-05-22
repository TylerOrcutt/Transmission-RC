#include<iostream>
#include<curl/curl.h>
#include<map>
#include <sstream>
#include<string>
#include<vector>
#include "TransmissionRC.h"
#include "TransmissionRPCRequest.h"
#include "config.h"

#include "termInterface.h"


using namespace TransmissionRC;

void handleArgs(int args, char ** argv);

std::map<std::string,std::string> Config::config;
std::string Config::sessionID="";

int main(int args, char **argv){

	Config::config = Config::loadConfig();
//need a session ID
	TransmissionRC::authenticate();
//start UI
	TransmissionRC::runUI();
return 0;
}

