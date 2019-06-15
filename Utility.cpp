#include "Utility.h"

using namespace TransmissionRC;

std::string TransmissionRC::Utility::convertTransferSpeed(int bps){
static const char *  xrate[] = {"B/s","KB/s","MB/s","GB/s","TB/s","PB/s"};
static const int xsize = 6;
	std::stringstream ss;
	float b = (float)bps;
	for(int i=0;i<xsize;i++){
		if(b < 1000 || i==xsize-1){
			ss<<b<<" "<<xrate[i];
			break;
		}
		b/=1000;

	}
return ss.str();
}
