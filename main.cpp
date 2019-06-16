#include<iostream>
#include<curl/curl.h>
#include<map>
#include <sstream>
#include<string>
#include<vector>
#include "TransmissionRC.h"
#include "TransmissionRPCRequest.h"
#include "config.h"
#include "args.h"
#include "termInterface.h"
#include "Utility.h"

using namespace TransmissionRC;

void handleArgs(int, char **);

std::map<std::string,std::string> Config::config;
std::string Config::sessionID="";

int main(int args, char **argv){

	Config::config = Config::loadConfig();

//need a session ID
	if(!TransmissionRC::authenticate()){
		std::cout<<"authentication failed";
		return 0;
	}
	

	if(args<=1){
//start UI
		TransmissionRC::Terminal::runUI();
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
	
		if(std::string(argv[i]) == "-t"){
			auto torrents = getTorrents();
			for(int t=0;torrents!=nullptr && t<torrents->size();t++){
			
				std::cout<<(*torrents)[t].ID<<":"
					 <<(*torrents)[t].Name
					 <<" "<<(*torrents)[t].totalSize/1024/1024<<"MB"
					 <<" "<<c_trStatus[(*torrents)[t].Status]
					 <<" "<<(*torrents)[t].percentDone
					 <<std::endl;
			}
			i--;
		}
	
		if(std::string(argv[i]) == "-i3-t"){
			i3Status();
			return;
		}
		
		if(std::string(argv[i]) == "-r"){
			int id=0;	
			if((id=std::stoi(argv[i+1]))>0){
				std::cout<<"resuming "<<id<<"\r\n";
				resumeTorrent(id);

			}
		}

		if(std::string(argv[i]) == "-s"){
			int id=0;	
			if((id=std::stoi(argv[i+1]))>0){
				std::cout<<"stopping "<<id<<"\r\n";
				stopTorrent(id);

			}
		}

		if(std::string(argv[i])=="-blocklist-update"){
			int c=-1;
			if((c=TransmissionRC::updateBlockList())>-1){
				std::cout<<"Updated "<<c<<"\r\n";
			}else{
				std::cout<<"Updated failed\r\n";
			}
			return;
		}
	}
	if(args==2 && std::strlen(argv[1])>2){
		TransmissionRC::addTorrent(std::string(argv[1]));	
	}
}
