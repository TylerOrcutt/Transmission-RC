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

void handleArgs(int, char **);

std::map<std::string,std::string> Config::config;
std::string Config::sessionID="";

int main(int args, char **argv){
	
	Config::config = Config::loadConfig();
//need a session ID
	TransmissionRC::authenticate();

	if(args<=1){
//start UI
		TransmissionRC::runUI();
	}else{
	 	handleArgs(args,argv);
	}

return 0;
}

void handleArgs(int args,char ** argv){
	
	for(int i=1; i<args;i+=2){
		//std::cout<<argv[i]<<std::endl;
		if(std::string(argv[i])=="-add"){
			TransmissionRC::addTorrent(std::string(argv[i+1]));	
		//	std::cout<<argv[i+1]<<std::endl;
		}

	}

}
