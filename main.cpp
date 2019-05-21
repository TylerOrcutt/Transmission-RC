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

void handleArgs(int ags, char ** argv);

std::map<std::string,std::string> Config::config;
std::string Config::sessionID="";

int main(int args, char **argv){
	TransmissionRC::init();

	Config::config = Config::loadConfig();
//need a session ID
//	TransmissionRequest request = TransmissionRC::MakeRequest();
//	TransmissionResponse response = TransmissionRC::DoRequest(request);
//	Config::sessionID = response.sessionID;
	TransmissionRC::authenticate();
	TransmissionRC::runUI();
TransmissionRC::cleanup();
// std::vector<rcTorrent>*torrents = TransmissionRC::getTorrents();
//for(int i=0;i<torrents.size();i++){
//	std::cout<<torrents[i].ID<<" | "
//	<<torrents[i]->Name<<" | "
//	<<torrents[i]->totalSize<<" | "
//	<<torrents[i]->percentDone<<" | "
//	<<c_trStatus[torrents[i].Status]
//	<<std::endl;
//}
return 0;
}

