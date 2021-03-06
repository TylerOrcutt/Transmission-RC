#pragma once
#ifndef __TRANSMISSIONRC_H_
#define __TRANSMISSIONRC_H_
#define debug
#include<iostream>
#include<string.h>
#include<map>
#include<memory>
#include<vector>
namespace TransmissionRC{


 typedef struct{
	std::string response;
	int statusCode;
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


 typedef enum{
	TR_STATUS_STOPPED = 0,
	TR_STATUS_CHECK_WAIT =1,
	TR_STATUS_CHECK = 2,
	TR_STATUS_DOWNLOAD_WAIT = 3,
	TR_STATUS_DOWNLOAD = 4,
	TR_STATUS_SEED_WAIT = 5,
	TR_STATUS_SEED = 6
}trStatus;

 static const char * c_trStatus[] = {"STOPPED",
				     "CHECK WAIT",
				     "WAIT",
				     "DOWNLOAD WAIT",
				     "DOWNLOADING",
				     "SEED WAIT",
				     "SEEDING"
				    };

 typedef struct{
	int bytesCompleted;
	int length;
	std::string name;
 }rcFile;

typedef struct{
	public:
	int ID;
	std::string Name;
	int Status;
	int rateDownload=0;
	int rateUpload=0;
	bool isFinished;
	unsigned long totalSize;
	double percentDone=0;
	int eta=-1;
	//info panel vars/
	int desiredAvailable =0;
	int uploadedEver =0;

	std::string errorString ="";
	std::string comment ="";
	
	std::vector<rcFile> files;

}rcTorrent;


 void init();
 void cleanup();
 TransmissionRequest MakeRequest();
 std::unique_ptr<TransmissionResponse> DoRequest(TransmissionRequest);



 size_t header_callback(char * buff, size_t size,size_t nitems, void *data);
 size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp);

}
#endif
