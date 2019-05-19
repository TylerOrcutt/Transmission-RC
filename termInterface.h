#include<curses.h>
#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include"TransmissionRPCRequest.h"

namespace TransmissionRC{
	void runUI();
	void drawScreen();
	void getKeyPress();
	void updateThread();
	typedef struct{
		int offset=0;
		int my,mx;
		int winH;
		int winW;
	}tcWindow;


};
