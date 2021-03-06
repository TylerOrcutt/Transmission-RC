#pragma once
#include <curses.h>
#include <memory.h>
#include <sstream>
#include <string>
#include <vector>
#include "TransmissionRC.h"
#include "Utility.h"
namespace TransmissionRC::Terminal{
	class tcWindow	{
		public:
		int offset=0;
		int my=0,mx=0;
		//int posx=0,posy=0;
		WINDOW* win=nullptr;
		int winH=0;
		int winW=0;


		tcWindow(int x,int y,int width,int height){

			winH = height;
			winW = width;

			win = newwin(winH,winW,y,x);
			keypad(stdscr,true);
			keypad(win,true);

		}
		~tcWindow(){
			delwin(win);
		}


		virtual void Draw(std::shared_ptr<std::vector<rcTorrent>>);
		virtual void Draw(rcTorrent);
	};
}
