#include "TransmissionRPCRequest.h"
#include "config.h"
using namespace TransmissionRC;

std::vector<rcTorrent>&TransmissionRC::getTorrents(){
	std::vector<rcTorrent> * torrents=new std::vector<rcTorrent>;

	TransmissionRequest request = MakeRequest();
	request.sessionID = Config::sessionID;
	request.requestData = "{ \"arguments\":{\"fields\":[\"id\",\"name\"]},\"method\":\"torrent-get\"}";

	TransmissionResponse response =	DoRequest(request);

	std::stringstream ss;
	ss<<response.response;

	boost::property_tree::ptree pt;

	try{
	    boost::property_tree::read_json(ss,pt);

	    BOOST_FOREACH(boost::property_tree::ptree::value_type &v,
					pt.get_child("arguments.torrents")){
//	assert(v.first.empty());
	//std::cout<<v.second.get<std::string>("name")<<std::endl;
		rcTorrent torrent;
		torrent.Name = v.second.get<std::string>("name");
		torrents->push_back(torrent);
	    }

	}catch(std::exception const &e){
	    std::cout<<"error: "<<e.what()<<"\r\n";
	}
	return *torrents;
}

