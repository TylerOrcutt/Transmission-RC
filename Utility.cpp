#include "Utility.h"

using namespace TransmissionRC;

std::string TransmissionRC::Utility::convertTransferSpeed(int bps){
	 double  kbps = bps/1000.00;
	std::stringstream ss;

	
	 if(kbps/1000 > 1){
		ss<<kbps/1000<<" MB/s";
	}else{
		ss<<kbps<< " KB/s";
	
	}
	
return ss.str();
}
