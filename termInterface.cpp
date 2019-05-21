#include"termInterface.h"
//TODO clean this shit up
using namespace TransmissionRC;

std::vector<rcTorrent> *torrents;

WINDOW *torwin;

int screenmx,screenmy;
int mx=0;
int my=0;
int toffset=0;
bool running=true;

	int winH = 0;
	int winW = 0;
std::mutex mtx;
std::string statusMsg="";

void TransmissionRC::updateThread(){

	while(running){
	mtx.lock();

	if(torrents!=NULL){
		free(torrents);
		torrents=NULL;
	}

	torrents = TransmissionRC::getTorrents();

	if(torrents==NULL){
		if(TransmissionRC::authenticate()){
  			torrents = TransmissionRC::getTorrents();
		}
	}
	mtx.unlock();

	drawScreen();
	std::this_thread::sleep_for(std::chrono::milliseconds(2500));

	}
}

void TransmissionRC::runUI(){

	TransmissionRC::init();

	initscr();
	noecho();
	curs_set(0);
	start_color();
	getmaxyx(stdscr,screenmx,screenmy);
	//nodelay(stdscr,true);
	init_pair(1,COLOR_WHITE,COLOR_BLACK);

	 winH = screenmx;
	 winW = screenmy;
//	 winH = screenmx -3;
//	 winW = screenmy-10;
	clear();
//	torwin = newwin(winH,winW,3,10);
	torwin = newwin(winH,winW,0,0);
	refresh();
	wrefresh(torwin);
	drawScreen();
	wmove(torwin,my,mx);
//update thread
	std::thread t(TransmissionRC::updateThread);
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

void TransmissionRC::getKeyPress(){
		int ch = getch();
		switch (ch){
			case 'j':
			if(my/4>=winH/4-1 &&torrents!=NULL && toffset<torrents->size()){
				toffset++;
			}else if(my/4<=winH/4 - 2){
				my+=4;
			}
			  break;
			case 'k':

			if(my<=0&&toffset>0){
				toffset--;
			}else if(my>0){
				my-=4;
			}
			  break;
			case 'q':
			running=false;
			break;
			case 'r':

			break;
			//start stop
			case 's':
			if(torrents==NULL){break;}
			 int id = my/4 + toffset;
			std::stringstream ss;
			 if((*torrents)[id].Status ==0){
			  bool r = TransmissionRC::resumeTorrent((*torrents)[id].ID);
			  ss<<"resuming "<<(*torrents)[id].Name;
			}else{
			  TransmissionRC::stopTorrent((*torrents)[id].ID);
			  ss<<"stopping "<<(*torrents)[id].Name;
			}
			  statusMsg = ss.str();
			   break;
		}
}

void TransmissionRC::drawScreen(){
	werase(torwin);
mtx.lock();
	for(int i=0,t=toffset;torrents !=NULL &&t<torrents->size();i++,t++){
		int posy = i+(i*3);
		if(posy/4>=winH/4){
		  break;
		}
		if(posy==my){
	//	  wmove(stdscr,posy,10);
		  wattron(torwin,A_STANDOUT);
		  wattron(torwin,COLOR_PAIR(1));
		}
		  std::string ln = (*torrents)[t].Name;
		  ln.insert(ln.length(),winW-ln.length(),' ');
		  mvwprintw(torwin,posy,0,ln.c_str());
		  posy++;
	          ln ="";
		  ln.insert(ln.begin(),
				(winW-2)*(*torrents)[t].percentDone,'=');
		  ln.insert(ln.end(),
			      (winW-2)*(1-(*torrents)[t].percentDone),'-');
		  ln.insert(ln.begin(),1,'[');
		  ln.insert(ln.end(),1,']');
		  std::stringstream strper;
		  strper<<((*torrents)[t].percentDone*100);
		 ln.replace(((ln.length()-2)/2)-(strper.str().length()/2),
			   strper.str().length(),
			   strper.str().c_str());
		  mvwprintw(torwin,posy,0,ln.c_str());
		posy++;
		strper.str(std::string());
		strper<<std::string("["+std::string(c_trStatus[(*torrents)[t].Status])+"]")
		      <<"  "<<"D:"<<(*torrents)[t].rateDownload
		      <<"  "<<"U:"<<(*torrents)[t].rateUpload;
		ln = strper.str();
		ln.insert(ln.length(),winW-ln.length(),' ');
		mvwprintw(torwin,posy,0,ln.c_str());

		  wattroff(torwin,A_STANDOUT);
		  wattroff(torwin,COLOR_PAIR(1));
	}
mtx.unlock();
//draw status msg;
	mvwprintw(torwin,winH-1,0,statusMsg.c_str());
	wrefresh(stdscr);
wrefresh(torwin);
}
