#pragma once
#include<memory>

namespace TransmissionRC::Utility{
	std::unique_ptr<char[]> convertTransferSpeed(int);
	std::unique_ptr<char[]> convertSize(int);


}
