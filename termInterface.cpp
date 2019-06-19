#include"termInterface.h"
//
//TODO clean this shit up
using namespace TransmissionRC;
using namespace TransmissionRC::Terminal;

	std::shared_ptr<std::vector<rcTorrent>> torrents;

	std::unique_ptr<torWindow> torwin=nullptr;
	std::unique_ptr<infoWindow> infowin=nullptr;

	int screenmx,screenmy;
	bool running=true;
	std::mutex mtx;
	std::string statusMsg="";

	bool showInfo = false;
	bool isInserting=false;
	int pchar=0;
	std::string insertTxt="";


void TransmissionRC::Terminal::updateThread(){

	while(running){
	mtx.lock();


	auto tt = TransmissionRC::getTorrents();

	if(tt==nullptr){
		if(TransmissionRC::authenticate()){
  			torrents = TransmissionRC::getTorrents();
		}else{
			statusMsg = "authentication failed.";	
		}
	}else{

		torrents=nullptr;
		torrents=std::move(tt);
	}

	mtx.unlock();
	drawScreen();
	std::this_thread::sleep_for(std::chrono::milliseconds(1500));

	}
}
void TransmissionRC::Terminal::resizeWindow(int sig){
	
	int nh,nw;
	getmaxyx(stdscr,nh,nw);

}

void TransmissionRC::Terminal::runUI(){

	TransmissionRC::init();

	initscr();
	noecho();
	curs_set(0);
	start_color();

	signal(SIGWINCH,resizeWindow);
	getmaxyx(stdscr,screenmy,screenmx);
	//nodelay(stdscr,true);
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_WHITE, COLOR_BLUE);


	torwin = std::make_unique<torWindow>(0,0,screenmx,screenmy);
	infowin = std::make_unique<infoWindow>(screenmx/2,0,screenmx/2,screenmy);

	clear();
	refresh();

//update thread
	std::thread t(TransmissionRC::Terminal::updateThread);
	t.detach();
//UI loop
	while(running){
		getKeyPress();
		drawScreen();
		//wmove(torwin,my,mx);
	}

	endwin();
	TransmissionRC::cleanup();
}

void TransmissionRC::Terminal::getKeyPress(){

	int ch = getch();
	int tid = torwin.get()->my/4 + torwin.get()->offset;

	if(isInserting && ch!=27){
		if(ch == 10){
			handleCommand();
			return;
		}
		if(ch == KEY_BACKSPACE){
			insertTxt = insertTxt.substr(0,insertTxt.length()-1);
		}else{
			insertTxt +=(char)ch;
		}
		return;
	}

	switch (ch){

	case 'j' :
	case KEY_DOWN : 
		if(torwin.get()->my/4>=torwin.get()->winH/4-1 
		   && torrents!=NULL
		   && torwin.get()->my/4 + torwin.get()->offset<torrents->size()-1){

			torwin.get()->offset++;
		}else if(torwin.get()->my/4<=torwin.get()->winH/4 - 2
			 && torwin.get()->my/4< torrents->size()-1 ){
			torwin.get()->my+=4;
		}
		
	break;

	case 'k':
	case KEY_UP:

		if(torwin.get()->my<=0&&torwin.get()->offset>0){

			torwin.get()->offset--;

		}else if(torwin.get()->my>0){
			torwin.get()->my-=4;
		}
	break;

	case 'q':
		running=false;
	break;

	case 'd': 

		if(torrents != NULL && tid<torrents->size()){
			statusMsg="Delete "+ (*torrents)[tid].Name+ "?(y/n)";
		}else{
			//stop from setting pchar
		return;
		}

	break;

	case 'i':
		if((*torwin).winW==screenmx){
			showInfo=true;
			(*torwin).winW/=2;	
		}else{
			showInfo=false;
			(*torwin).winW=screenmx;		
		}	
	break;
	case 'y': 

		if( pchar=='d'){
			statusMsg="Deleting "+ (*torrents)[tid].Name +".";
			pchar=0;
			TransmissionRC::removeTorrent((*torrents)[tid].ID);
			return;
		}
	break;
	case ':':
		isInserting = true;
	break;
	//start stop
	case 27:

		isInserting = false;
		insertTxt="";
		statusMsg = "";
	break;
	case 's':

		if(torrents==nullptr){break;}
		
		std::stringstream ss;

		if((*torrents)[tid].Status ==0){
			bool r = TransmissionRC::resumeTorrent(
						(*torrents)[tid].ID);
			ss<<"resuming "<<(*torrents)[tid].Name;
		}else{
				
			TransmissionRC::stopTorrent((*torrents)[tid].ID);
			ss<<"stopping "<<(*torrents)[tid].Name;
		}

		statusMsg = ss.str();
	break;
	}

	std::stringstream sc;
	sc<<ch;
	//statusMsg=sc.str();
	pchar=ch; 
}

void TransmissionRC::Terminal::drawScreen(){

	//getmaxyx(stdscr,screenmy,screenmx);

	mtx.lock();
	(*torwin).Draw(torrents);
	if(showInfo){
		int tid = torwin.get()->my/4 + torwin.get()->offset;
		(*infowin).Draw((*torrents)[tid]);
	}

	mtx.unlock();
//draw status msg;
	if(isInserting){
		std::string itxt = ":" + insertTxt;
		mvwprintw(stdscr,screenmy-1,0,itxt.c_str());
	}else{
		mvwprintw(stdscr,screenmy-1,0,statusMsg.c_str());
	}
	
	wrefresh(stdscr);
}


void TransmissionRC::Terminal::handleCommand(){

	//TODO async 
	//

	if(insertTxt == "update blocklist"){
		int  cnt = updateBlockList();
		if(cnt >-1){
			std::stringstream ss;
			ss<<"block list updated, "<<cnt<<" records found.";	
			statusMsg = ss.str();
		}else {
			statusMsg = "update Failed";
		}
	
	}


	insertTxt="";
	isInserting = false;
}

