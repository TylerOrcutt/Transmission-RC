#include "config.h"
using namespace TransmissionRC::Config;
std::map<std::string,std::string> TransmissionRC::Config::loadConfig(){
	std::cout<<"Loading config\n\n";
	std::map<std::string,std::string> cfg;
	std::ifstream rf(cfgPath,std::ios::in);
	if(rf.is_open()){
	   	//std::cout<<"opened file\r\n";
		std::string line;
		while(getline(rf,line)){
			//std::cout<<line<<"\r\n";
			int pos = line.find("=");
			if(pos<0){continue;}
			std::string key = line.substr(0,pos);
			std::string value = line.substr(pos+1);
			cfg.insert(std::pair<std::string,std::string>(key,value));
			//std::cout<<"key: "<<key<<"  value:"<<value<<"\r\n";
		 }

	rf.close();
	//TransmissionRC::Config::config = cfg;
	return cfg;
	}

	std::cout<<"error opening file\r\n";
  return cfg;
}
