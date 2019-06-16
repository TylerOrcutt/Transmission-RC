#pragma once
#include <curses.h>
#include <memory>
#include <string>
#include <vector>
#include "tcWindow.h"
#include "TransmissionRC.h"


using namespace TransmissionRC;
namespace TransmissionRC::Terminal{
	class infoWindow :public tcWindow{
		public:
		
		infoWindow(int x,int y,int width,int height):
						tcWindow(x,y,width,height){

		}

		void Draw(std::shared_ptr<std::vector<rcTorrent>>);
		void Draw(rcTorrent);

	};


}
