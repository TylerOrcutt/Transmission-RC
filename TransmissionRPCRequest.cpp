#include "TransmissionRPCRequest.h"
#include "config.h"
using namespace TransmissionRC;


bool TransmissionRC::authenticate(){

	TransmissionRequest request = MakeRequest();
	TransmissionResponse response = TransmissionRC::DoRequest(request);
	
	if(response.statusCode == 200 || 
			(response.sessionID !="" && response.statusCode==409)) { 
		TransmissionRC::Config::sessionID = response.sessionID;	
		return true;
	}
	
	return false;
}


std::vector<rcTorrent>*TransmissionRC::getTorrents(){
	std::vector<rcTorrent> * torrents=new std::vector<rcTorrent>;

	TransmissionRequest request = MakeRequest();
	//request.sessionID = Config::sessionID;
	request.requestData = "{ \"arguments\":{\"fields\":["
							"\"id\","
						   	"\"name\","
							"\"status\","
							"\"rateDownload\","
							"\"rateUpload\","
							"\"isFinished\","
							"\"totalSize\","
							"\"percentDone\""
							"]},\""
							"method\":\"torrent-get\"}";

	TransmissionResponse response =	DoRequest(request);
	std::stringstream ss;
	ss<<response.response;

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
		torrents->push_back(torrent);

		if(torrent.Status==TransmissionRC::TR_STATUS_SEED
		  && Config::config["stopSeeding"]=="true"){
			//TransmissionRC::stopTorrent(torrent.ID);	
		}

	    }

	}catch(std::exception const &e){
//	    std::cout<<"error: "<<e.what()<<"\r\n";
//	std::cout<<response.response<<std::endl
//	<<"\r\n'"<<Config::sessionID<<"'"<<std::endl;
	   free(torrents);
	   return NULL;
	}

	return torrents;
}


bool TransmissionRC::resumeTorrent(int id){
	
	TransmissionRequest request = MakeRequest();

	std::stringstream req;
	req<<"{ \"arguments\":{\"ids\":["<<id<<"]},\"method\":\"torrent-start\"}";
	request.requestData =req.str();
	TransmissionResponse response =	DoRequest(request);

	if(response.statusCode == 200){
		return true;
	}
	return false;
}


bool TransmissionRC::stopTorrent(int id){
	
	TransmissionRequest request = MakeRequest();

	std::stringstream req;
	req<<"{ \"arguments\":{\"ids\":["<<id<<"]},\"method\":\"torrent-stop\"}";
	request.requestData =req.str();
	TransmissionResponse response =	DoRequest(request);

	if(response.statusCode == 200){
		return true;
	}
	return false;
}

bool TransmissionRC::addTorrent(std::string URL){
	
	TransmissionRequest request = MakeRequest();

	request.requestData = "{ \"arguments\":{\"filename\":"
				"\""+URL+"\"},\"method\":\"torrent-add\"}";
	std::cout<<request.requestData<<std::endl<<std::endl;
	TransmissionResponse response =	DoRequest(request);
	std::cout<<response.response;
	if(response.statusCode == 200){
		return true;
	}
	return false;
}
