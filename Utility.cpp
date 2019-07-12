#include <iostream>
#include<vector>
#include "Utility.h"
#include <sstream>

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/insert_linebreaks.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/ostream_iterator.hpp>

using namespace boost::archive::iterators;
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



std::string TransmissionRC::Utility::readFile(const char * path){

	std::ifstream fi(path,std::ios::binary | std::ios::ate );
	

	std::stringstream odata;

	if(fi.is_open()){
		int size = fi.tellg();
		char * buffer = new char[size];
		fi.seekg(0,std::ios::beg);
		
		if(fi.read(buffer,size)){
			fi.close();

			typedef 
			base64_from_binary<transform_width<const char *,6,8>>
			base64_text;

	std::copy(base64_text(buffer),
				base64_text(buffer + size),
				ostream_iterator<char>(odata));
	return odata.str();
	}
	}
	return "";
}
