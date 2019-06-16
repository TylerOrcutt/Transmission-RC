#include "args.h"


using namespace TransmissionRC;

void TransmissionRC::i3Status(){
	int seeding = 0;
	int leeching = 0;
	int paused = 0;
	auto  torrents = getTorrents();
	int downbps =0;
	int upbps =0;
	for(int i=0;torrents!=NULL && i<torrents->size();i++){
		downbps+= (*torrents)[i].rateDownload;
		upbps += (*torrents)[i].rateUpload;
		switch((*torrents)[i].Status){
			case 0:
				paused++;
			break;
			case 1:
			case 2:
			case 3:
			case 4:
				leeching++;
			break;
			case 5:
			case 6:
				seeding++;
			break;
		}
	}
	std::cout.precision(std::numeric_limits<float>::digits10);
	std::cout<<"D: "<<Utility::convertTransferSpeed(downbps).get()
		<<" U: "<<Utility::convertTransferSpeed(upbps).get()
		<<" ⯅ "<<seeding<<" ⯆ "<<leeching<<" ▮▮ "<<paused;

}
