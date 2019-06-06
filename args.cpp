#include "args.h"


using namespace TransmissionRC;

void TransmissionRC::i3Status(){
	int seeding = 0;
	int leeching = 0;
	int paused = 0;
	std::vector<rcTorrent> * torrents = getTorrents();
	for(int i=0;torrents!=NULL && i<torrents->size();i++){
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

	std::cout<<"⯅ "<<seeding<<" ⯆ "<<leeching<<" ▮▮ "<<paused;

}
