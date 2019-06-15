#include "Utility.h"

using namespace TransmissionRC;

std::string TransmissionRC::Utility::convertTransferSpeed(int bps){
static const char *  xrate[] = {"B/s","KB/s","MB/s","GB/s","TB/s","PB/s"};
static const int xsize = 6;
	std::stringstream ss;
	for(int i=0;i<xsize;i++){
		if((bps>>(i*10)) <= 1000 || i==xsize-1){
			ss<<(float)bps/(1000^(10*i))<<" "<<xrate[i];
			break;
		}

	}
return ss.str();
}
