#include<iostream>
#include<curl/curl.h>
#include<map>
#include <sstream>
#include<string>
#include<vector>
#include<curses.h>
#include "TransmissionRC.h"
#include "TransmissionRPCRequest.h"
#include "config.h"

using namespace TransmissionRC;

void handleArgs(int ags, char ** argv);

std::map<std::string,std::string> Config::config;
std::string Config::sessionID;
void initcurses();

int main(int args, char **argv){
initcurses();
return 0;

Config::config = Config::loadConfig();


//need a session ID
TransmissionRequest request = TransmissionRC::MakeRequest();
TransmissionResponse response = TransmissionRC::DoRequest(request);
Config::sessionID = response.sessionID;

std::vector<TransmissionRC::rcTorrent> torrents = TransmissionRC::getTorrents();
for(int i=0;i<torrents.size();i++){
	std::cout<<torrents[i].ID<<" | "
	<<torrents[i].Name<<" | "
	<<torrents[i].totalSize<<" | "
	<<torrents[i].percentDone<<" | "
	<<c_trStatus[torrents[i].Status]
	<<std::endl;
}
return 0;
}

void initcurses(){
	initscr();
	noecho();
	int mx=0,my=0;
	getmaxyx(stdscr,mx,my);


	wrefresh(stdscr);
	mvwprintw(stdscr,10,10,"Hello world");
	mvwprintw(stdscr,11,10,"Test");
	while(true){
		int ch = getch();
	wrefresh(stdscr);
	mvwprintw(stdscr,10,10,"Hello world");
	mvwprintw(stdscr,11,10,"Test");
		//if(ch=='q'){
		 //break;
		//}
		switch (ch){
			case 'j':
			wmove(stdscr,11,10);
			  break;
			case 'k':
			wmove(stdscr,10,10);
			  break;
			case 'q':
			mvwaddch(stdscr,my,0,':');
			wmove(stdscr,my,1);
			echo();
			break;
		}
	}


endwin();
}


