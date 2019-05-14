#include "TransmissionRC.h"
#include <curl/curl.h>
using namespace TransmissionRC;
void TransmissionRC::test(){
 std::cout<<"test\r\n\r\n";
}
TransmissionRC::TransmissionResponse & TransmissionRC::MakeRequest(TransmissionRequest request){
	curl_global_init(CURL_GLOBAL_ALL);
	std::string readBuffer;
	//std::string headerBuffer;
	std::map<std::string,std::string>headerBuffer;
	std::cout<<"Making transmission "
	<<"request\r\n";

	auto hdlr = curl_easy_init();

	std::string url = "http://" + request.username + ":" + request.password ;
		    url+= "@"+request.host + ":"+request.port+"/transmission/rpc/";
	curl_easy_setopt(hdlr,
			CURLOPT_URL,
			url.c_str());
//headers
	struct curl_slist *headers=NULL;
	std::string hdr_sessionID = "X-Transmission-Session-Id: " + request.sessionID;
	headers= curl_slist_append(headers,hdr_sessionID.c_str());

	curl_easy_setopt(hdlr,CURLOPT_HTTPHEADER,headers);
//requst data
	curl_easy_setopt(hdlr,CURLOPT_POSTFIELDS,
		"{ \"arguments\":{\"fields\":[\"id\",\"name\"]},\"method\":\"torrent-get\"}");
	curl_easy_setopt(hdlr,CURLOPT_HEADERFUNCTION,header_callback);
	curl_easy_setopt(hdlr,CURLOPT_HEADERDATA,&headerBuffer);
	curl_easy_setopt(hdlr,CURLOPT_WRITEFUNCTION,write_data);
	curl_easy_setopt(hdlr,CURLOPT_WRITEDATA,&readBuffer);
//do request
	bool success = curl_easy_perform(hdlr);
	//std::cout<<readBuffer<<"\r\n";
	if(success){
		//std::cout<<"Success\n\n";
	}else{
		//std::cout<<"Failed\n\n";
	}

	TransmissionRC::TransmissionResponse *response = new TransmissionRC::TransmissionResponse();
	response->response = readBuffer;
	response->sessionID = headerBuffer["sessionID"];
	response->statusCode = headerBuffer["status"];
  return *response;
 }

 size_t TransmissionRC::write_data(void *buffer,size_t size, size_t nmemb, void *usrp){
	((std::string*)usrp)->append((char*)buffer,size*nmemb);
	return size*nmemb;
 }


 size_t TransmissionRC::header_callback(char * buff, size_t size,size_t nitems, void *data){
	std::string header(buff);
	std::string statusvar = "HTTP/";

	if(header.substr(0,statusvar.length())==statusvar){
		char * splt = strtok(buff," ");
		for(int i=0; splt !=NULL;i++){
			if(i==1){
			  //*((std::string*)data) =std::string(splt);
	   		  ((std::map<std::string,std::string>*)data)->insert(
						std::pair<std::string,std::string>("status",splt));
			}
			 splt=strtok(NULL," ");
		}
	}
	std::string sessionvar = "X-Transmission-Session-Id";

	if(header.substr(0,sessionvar.length()) == sessionvar){
		header=header.substr(sessionvar.length()+2);
		header=header.substr(0,header.length()-2);
	   	((std::map<std::string,std::string>*)data)->insert(
					     std::pair<std::string,std::string>("sessionID",header));
	//*((std::string*)data) = header;
	}
	//std::cout<<buff<<"\r\n";
	return size*nitems;
 }
