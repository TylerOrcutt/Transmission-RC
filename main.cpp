#include<iostream>
#include<curl/curl.h>
#include<map>
#include "TransmissionRC.h"
#include "TransmissionRPCRequest.h"
#include "config.h"

using namespace TransmissionRC;

void handleArgs(int ags, char ** argv);

int main(int args, char **argv){

TransmissionRC::test();
std::map<std::string,std::string> cfg = TransmissionRC::Config::loadConfig();

//TransmissionRC::rcTorrent * torrent = TransmissionRC::getTorrents();
//std::cout<<"Torrent : "<<torrent->Name<<"\r\n";
//free(torrent);

TransmissionRequest request;
request.username = cfg["username"];
request.password=cfg["password"];
request.host=cfg["host"];
request.port=cfg["port"];

TransmissionResponse response = TransmissionRC::MakeRequest(request);
std::cout<<response.sessionID<<"\r\n";
request.sessionID = response.sessionID;
response = TransmissionRC::MakeRequest(request);

std::cout<<response.statusCode<<"\r\n";
std::cout<<response.sessionID<<"\r\n";
std::cout<<response.response<<"\r\n";

return 0;
}


