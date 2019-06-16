#include<curses.h>
#include<iostream>
#include<vector>
#include<string>
#include<thread>
#include<mutex>
#include <signal.h>
#include "tcWindow.h"
#include"TransmissionRPCRequest.h"
#include "Utility.h"

namespace TransmissionRC::Terminal{
	void runUI();
	void drawScreen();
	void getKeyPress();
	void updateThread();

	void handleCommand();
	void resizeWindow(int);
	
	enum{
		Success=0,
		Failure=1
		
	}tcStatus;

	typedef struct{
		std::string msg;
		int status;
		
	}tcStatusMessage;


};
