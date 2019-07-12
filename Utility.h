#pragma once
#include<memory>
#include <fstream>
namespace TransmissionRC::Utility{
	std::unique_ptr<char[]> convertTransferSpeed(int);
	std::unique_ptr<char[]> convertSize(int);
	std::string readFile(const char * );


}
