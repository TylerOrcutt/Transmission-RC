#include "TransmissionRC.h"
#include <curl/curl.h>
#include "config.h"
using namespace TransmissionRC;

void TransmissionRC::init(){

	curl_global_init(CURL_GLOBAL_ALL);
}
void TransmissionRC::cleanup(){
	curl_global_cleanup();
}


std::unique_ptr<TransmissionRC::TransmissionResponse> TransmissionRC::DoRequest(
						TransmissionRequest request){
	std::string readBuffer;
	std::map<std::string,std::string>headerBuffer;

	auto hdlr = curl_easy_init();

	std::string url;
			url= "http://" + request.username + ":" + request.password ;
			url+= "@"+request.host + ":"+request.port+"/transmission/rpc/";

	curl_easy_setopt(hdlr,
			CURLOPT_URL,
			url.c_str());
//headers
	struct curl_slist *headers=NULL;
	std::string hdr_sessionID = "X-Transmission-Session-Id:" + request.sessionID;
	headers= curl_slist_append(headers,hdr_sessionID.c_str());

//	curl_easy_setopt(hdlr,CURLOPT_VERBOSE,1L);
	curl_easy_setopt(hdlr,CURLOPT_HTTPHEADER,headers);
//request data
	curl_easy_setopt(hdlr,CURLOPT_POSTFIELDS,request.requestData.c_str());
	curl_easy_setopt(hdlr,CURLOPT_HEADERFUNCTION,header_callback);
	curl_easy_setopt(hdlr,CURLOPT_HEADERDATA,&headerBuffer);
	curl_easy_setopt(hdlr,CURLOPT_WRITEFUNCTION,write_data);
	curl_easy_setopt(hdlr,CURLOPT_WRITEDATA,&readBuffer);
//do request
	bool success = curl_easy_perform(hdlr);
//cleanup
	curl_slist_free_all(headers);
	headers=NULL;
	curl_easy_cleanup(hdlr);	
//response	
	std::unique_ptr<TransmissionRC::TransmissionResponse> response =
					std::make_unique<TransmissionRC::TransmissionResponse>();
	response.get()->response = readBuffer;
	response.get()->sessionID = headerBuffer["sessionID"];
	try{
		response.get()->statusCode = std::stoi(headerBuffer["status"]);
	}catch (std::exception const &e){

	response.get()->statusCode = 0;
	}

  return std::move(response);
 }

 TransmissionRequest TransmissionRC::MakeRequest(){
	TransmissionRC::TransmissionRequest  request;
	request.username=Config::config["username"];
	request.password=Config::config["password"];
	request.host=Config::config["host"];
	request.port=Config::config["port"];
	request.sessionID = Config::sessionID;
	return request;
}



 size_t TransmissionRC::write_data(void *buffer,size_t size, size_t nmemb, void *usrp){
	((std::string*)usrp)->append((char*)buffer,size*nmemb);
	return size*nmemb;
 }


 size_t TransmissionRC::header_callback(char * buff, 
					size_t size,
					size_t nitems, 
					void *data){
	std::string header(buff);
	static const std::string statusvar = "HTTP/";

	if(header.substr(0,statusvar.length())==statusvar){
		char * splt = strtok(buff," ");
		for(int i=0; splt !=NULL;i++){
			if(i==1){
	   		  ((std::map<std::string,std::string>*)data)->insert(
				std::pair<std::string,std::string>("status",splt));
			}
			 splt=strtok(NULL," ");
		}
	}
	static const std::string sessionvar = "X-Transmission-Session-Id";

	if(header.substr(0,sessionvar.length()) == sessionvar){
		header=header.substr(sessionvar.length()+2);
		header=header.substr(0,header.length()-2);
	   	((std::map<std::string,std::string>*)data)->insert(
			std::pair<std::string,std::string>("sessionID",header));
	}

	return size*nitems;
 }
