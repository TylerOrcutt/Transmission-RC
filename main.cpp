#include<iostream>
#include<curl/curl.h>
#include<map>
#include <sstream>
#include<string>
#include<vector>
#include "TransmissionRC.h"
#include "TransmissionRPCRequest.h"
#include "config.h"

using namespace TransmissionRC;

void handleArgs(int ags, char ** argv);

std::map<std::string,std::string> Config::config;
std::string Config::sessionID;

int main(int args, char **argv){

Config::config = Config::loadConfig();


TransmissionRequest request = TransmissionRC::MakeRequest();
//need a session ID
TransmissionResponse response = TransmissionRC::DoRequest(request);
Config::sessionID = response.sessionID;

std::vector<TransmissionRC::rcTorrent> torrents = TransmissionRC::getTorrents();
for(int i=0;i<torrents.size();i++){
	std::cout<<torrents[i].Name<<std::endl;
}
return 0;
}


