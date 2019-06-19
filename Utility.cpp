#include "Utility.h"

using namespace TransmissionRC;

std::unique_ptr<char[]> TransmissionRC::Utility::convertTransferSpeed(int bps){
	static const char *  xrate[] = {"B/s","KB/s","MB/s","GB/s","TB/s","PB/s"};
	static const int xsize = 6;
	std::unique_ptr<char[]> ar(new char[12]);
	float b = (float)bps;

	for(int i=0;i<xsize;i++){
		if(b < 1000 || i==xsize-1){
			sprintf(ar.get(),"%.2f %s",b,xrate[i]);
			break;
		}
		b/=1000;
	}

return std::move(ar);
}

std::unique_ptr<char[]> TransmissionRC::Utility::convertSize(int bps){
	static const char *  xrate[] = {"B","kB","mB","gB","tB","pB"};
	static const int xsize = 6;
	std::unique_ptr<char[]> ar(new char[12]);
	float b = (float)bps;

	for(int i=0;i<xsize;i++){
		if(b < 1000 || i==xsize-1){
			sprintf(ar.get(),"%.2f %s",b,xrate[i]);
			break;
		}
		b/=1000;
	}

return std::move(ar);
}
