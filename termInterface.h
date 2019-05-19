#include<curses.h>
#include<iostream>
#include<vector>
#include"TransmissionRPCRequest.h"

namespace TransmissionRC{
	void runUI();
	void drawScreen();
	void getKeyPress();

	typedef struct{
		int offset=0;
		int my,mx;
		int winH;
		int winW;
	}tcWindow;


};
