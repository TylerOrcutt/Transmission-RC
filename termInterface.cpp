#include"termInterface.h"
//TODO clean this shit up
using namespace TransmissionRC;

std::vector<rcTorrent> *torrents;

tcWindow torwin;

int screenmx,screenmy;
bool running=true;
std::mutex mtx;
std::string statusMsg="";

bool isInserting=false;
int pchar=0;
std::string insertTxt="";

void TransmissionRC::updateThread(){

	while(running){
	mtx.lock();


	std::vector<rcTorrent> *tt = TransmissionRC::getTorrents();

	if(tt==NULL){
		if(TransmissionRC::authenticate()){
  			torrents = TransmissionRC::getTorrents();
		}else{
			statusMsg = "authentication failed.";	
		}
	}else{

		if(torrents!=NULL){
			free(torrents);
			torrents=NULL;
		}
		torrents=tt;
	}

	mtx.unlock();
	drawScreen();
	std::this_thread::sleep_for(std::chrono::milliseconds(5500));

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
	init_pair(2,COLOR_WHITE, COLOR_BLUE);
	torwin.winH = screenmx;
	torwin.winW = screenmy;

	clear();

	torwin.win = newwin(torwin.winH,torwin.winW,0,0);
	keypad(stdscr,true);
	keypad(torwin.win,true);
				
	//wbkgd(addTorwin.win,COLOR_PAIR(2));		
	refresh();
	wrefresh(torwin.win);
	//box(addTorwin.win,0,0);
	//wrefresh(addTorwin.win);
	drawScreen();
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
	int tid = torwin.my/4 + torwin.offset;

	switch (ch){

	case 'j' :
	case KEY_DOWN : 
		if(torwin.my/4>=torwin.winH/4-1 
		   && torrents!=NULL
		   && torwin.my/4 + torwin.offset<torrents->size()-1){

			torwin.offset++;
		}else if(torwin.my/4<=torwin.winH/4 - 2
			 && torwin.my/4< torrents->size()-1 ){
			torwin.my+=4;
		}
	break;

	case 'k':
	case KEY_UP:

		if(torwin.my<=0&&torwin.offset>0){

			torwin.offset--;

		}else if(torwin.my>0){
			torwin.my-=4;
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
	case 's':

		if(torrents==NULL){break;}

			int id = torwin.my/4 + torwin.offset;
			std::stringstream ss;

			if((*torrents)[id].Status ==0){
				bool r = TransmissionRC::resumeTorrent(
								(*torrents)[id].ID);
				ss<<"resuming "<<(*torrents)[id].Name;
			}else{
				
				TransmissionRC::stopTorrent((*torrents)[id].ID);
				ss<<"stopping "<<(*torrents)[id].Name;
			}

		statusMsg = ss.str();
	break;

	}
	pchar=ch; 
}

void TransmissionRC::drawScreen(){

	werase(torwin.win);
	mtx.lock();
	for(int i=0,t=torwin.offset;torrents !=NULL &&t<torrents->size();i++,t++){
		int posy = i+(i*3);

		if(posy/4>=torwin.winH/4){
		  break;
		}

		if(posy==torwin.my){
		  wattron(torwin.win,A_STANDOUT);
		  wattron(torwin.win,COLOR_PAIR(1));
		}
		
		std::string ln = (*torrents)[t].Name;
		ln.insert(ln.length(),torwin.winW-ln.length(),' ');

		mvwprintw(torwin.win,posy,0,ln.c_str());

		posy++;
		ln ="";
	
		ln.insert(ln.begin(),
				(torwin.winW-2)*(*torrents)[t].percentDone,'=');
		ln.insert(ln.end(),
			      (torwin.winW-2)*(1-(*torrents)[t].percentDone),'-');

		ln.insert(ln.begin(),1,'[');
		ln.insert(ln.end(),1,']');

		std::stringstream strper;

		strper<<((*torrents)[t].percentDone*100);

		ln.replace(((ln.length()-2)/2)-(strper.str().length()/2),
			   strper.str().length(),
			   strper.str().c_str());

		mvwprintw(torwin.win,posy,0,ln.c_str());
		posy++;

		strper.str(std::string());
		strper<<std::string("["+std::string(
					c_trStatus[(*torrents)[t].Status])+"]")
		      <<"  "<<"D:"
		      <<Utility::convertTransferSpeed((*torrents)[t].rateDownload)
		      <<"  "<<"U:"
		      <<Utility::convertTransferSpeed((*torrents)[t].rateUpload);

		ln = strper.str();
		ln.insert(ln.length(),torwin.winW-ln.length(),' ');

		mvwprintw(torwin.win,posy,0,ln.c_str());

		wattroff(torwin.win,A_STANDOUT);
		wattroff(torwin.win,COLOR_PAIR(1));
	}
	mtx.unlock();
//draw status msg;
	mvwprintw(torwin.win,torwin.winH-1,0,statusMsg.c_str());
	
	wrefresh(stdscr);
	wrefresh(torwin.win);
}
