#pragma once
#ifndef __TRANSMISSIONRC_H_
#define __TRANSMISSIONRC_H_
#include<iostream>
#include<string.h>
#include<map>
namespace TransmissionRC{


 typedef struct{
	std::string response;
	std::string statusCode;
	std::string sessionID;

 }TransmissionResponse;

 typedef struct{
	std::string username;
	std::string password;
	std::string host;
	std::string port;
	std::string sessionID;
	std::string requestData;
 } TransmissionRequest;


 typedef struct{
	std::string Name;
}rcTorrent;


 TransmissionRequest& MakeRequest();
 TransmissionResponse& DoRequest(TransmissionRequest);



 size_t header_callback(char * buff, size_t size,size_t nitems, void *data);
 size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

}
#endif
