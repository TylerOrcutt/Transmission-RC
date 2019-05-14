#include "TransmissionRPCRequest.h"
using namespace TransmissionRC;

rcTorrent * TransmissionRC::getTorrents(){
	TransmissionRC::rcTorrent * torrents =(TransmissionRC::rcTorrent*) malloc(sizeof(TransmissionRC::rcTorrent));
	torrents->Name = "Test";

	return torrents;
}

