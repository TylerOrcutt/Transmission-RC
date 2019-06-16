#pragma once
#include <curses.h>
#include <memory.h>
#include <sstream>
#include <string>
#include <vector>
#include "tcWindow.h"
#include "TransmissionRC.h"
#include "Utility.h"
namespace TransmissionRC::Terminal{
	class torWindow:public tcWindow	{
		public:


		torWindow(int x,int y,int width,int height):tcWindow(x,y,width,height){

		}

		void Draw(std::shared_ptr<std::vector<rcTorrent>>);
		
		void Draw(rcTorrent);
	};
}
