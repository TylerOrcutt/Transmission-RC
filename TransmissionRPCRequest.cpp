#include "TransmissionRPCRequest.h"
#include "config.h"
using namespace TransmissionRC;


bool TransmissionRC::authenticate(){

	TransmissionRequest request = MakeRequest();
	auto response = TransmissionRC::DoRequest(request);
	
	if(response->statusCode == 200 || 
			(response->sessionID !="" && response->statusCode==409)) { 
		TransmissionRC::Config::sessionID = response->sessionID;	
		return true;
	}
	
	return false;
}


std::unique_ptr<std::vector<rcTorrent>>  TransmissionRC::getTorrents(){
	auto  torrents = std::make_unique<std::vector<rcTorrent>>();

	TransmissionRequest request = MakeRequest();

	request.requestData = "{ \"arguments\":{\"fields\":["
							"\"id\","
						   	"\"name\","
							"\"status\","
							"\"rateDownload\","
							"\"rateUpload\","
							"\"isFinished\","
							"\"totalSize\","
							"\"percentDone\","
							"\"eta\","
							//
							"\"desiredAvailable\","
							"\"uploadedEver\","
							//"\"\","
							//"\"\","
							//"\"\","
							"\"files\","
							"\"errorString\","
							"\"comment\""
							"]},\""
							"method\":\"torrent-get\"}";

	auto response = DoRequest(request);
	std::stringstream ss;
	ss<<response->response;
#ifdef debug

	//std::cout<<response->response<<"\r\n";
#endif

	boost::property_tree::ptree pt;

	try{
	    boost::property_tree::read_json(ss,pt);

	    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
					pt.get_child("arguments.torrents")){
//	assert(v.first.empty());
		rcTorrent torrent;
		torrent.ID = v.second.get<int>("id");
		torrent.Name = v.second.get<std::string>("name");
		torrent.Status = v.second.get<int>("status");
		torrent.rateDownload = v.second.get<int>("rateDownload");
		torrent.rateUpload = v.second.get<int>("rateUpload");
		torrent.totalSize = v.second.get<unsigned long>("totalSize");
		torrent.percentDone = v.second.get<double>("percentDone");
		//torrent.eta = v.second.get<int>("eta");
		
		//torrent.desiredAvailable = v.second.get<int>("desiredAvailable");
		//torrent.uploadedEver = v.second.get<int>("uploadedEver");

		torrent.errorString = v.second.get<std::string>("errorString");
		torrent.comment = v.second.get<std::string>("comment");

		//sub files
		boost::property_tree::ptree subt = v.second;
		
		BOOST_FOREACH(boost::property_tree::ptree::value_type &vs,
							subt.get_child("files")){

					rcFile file;
					file.name = 
						vs.second.get<std::string>("name");
					torrent.files.push_back(file);
		}	

		torrents->push_back(torrent);

		if(torrent.Status==TransmissionRC::TR_STATUS_SEED
		  && Config::config["stopSeeding"]=="true"){
			//TransmissionRC::stopTorrent(torrent.ID);	
		}

	    }

	}catch(std::exception const &e){

	   return nullptr;
	}

	return std::move(torrents);
}


bool TransmissionRC::resumeTorrent(int id){
	
	TransmissionRequest request = MakeRequest();

	std::stringstream req;
	req<<"{ \"arguments\":{\"ids\":["<<id<<"]},\"method\":\"torrent-start\"}";
	request.requestData =req.str();
	auto response = DoRequest(request);

	if(response->statusCode == 200){
		return true;
	}
	return false;
}


bool TransmissionRC::stopTorrent(int id){
	
	TransmissionRequest request = MakeRequest();

	std::stringstream req;
	req<<"{ \"arguments\":{\"ids\":["<<id<<"]},\"method\":\"torrent-stop\"}";
	request.requestData =req.str();

	auto response = DoRequest(request);

	if(response->statusCode == 200){
		return true;
	}
	return false;
}

bool TransmissionRC::addTorrent(std::string path){
	
	TransmissionRequest request = MakeRequest();
	
	if(path.substr(0,7) == "http://"
		       	|| path.substr(0,8) == "https://"
			|| path.substr(0,11)=="magnet:?xt="){

		request.requestData = "{ \"arguments\":{\"filename\":"
			"\""+path+"\"},\"method\":\"torrent-add\"}";
	}else{
		
		std::string data = TransmissionRC::Utility::readFile(path.c_str());

		request.requestData = "{ \"arguments\":{\"metainfo\":"
			"\""+data+"\"},\"method\":\"torrent-add\"}";
	}
	
	std::cout<<request.requestData<<std::endl<<std::endl;
	auto response = DoRequest(request);
	std::cout<<response->response;

	if(response->statusCode == 200){
		return true;
	}
	return false;
}


bool TransmissionRC::removeTorrent(int id){
	
	TransmissionRequest request = MakeRequest();
		
	std::stringstream req;
	req<<"{ \"arguments\":{\"ids\":["<<id<<"]},\"method\":\"torrent-remove\"}";
	request.requestData =req.str();

	auto response = DoRequest(request);

	if(response->statusCode == 200){
		return true;
	}
	return false;
}



int TransmissionRC::updateBlockList(){

	TransmissionRequest request = MakeRequest();

	request.requestData = "{\"method\":\"blocklist-update\"}";

	auto response = DoRequest(request);

	std::stringstream ss;
	ss<<response->response;	

	boost::property_tree::ptree pt;

	try{
	    boost::property_tree::read_json(ss,pt);

	    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
					pt.get_child("arguments")){
			return v.second.get_value<int>();

		}

	    

	}catch(std::exception const &e){

		return -1;
	}

	if(response->statusCode == 200){
		return 0;
	}
	return -1;
	

}
