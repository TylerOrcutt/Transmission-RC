#include"termInterface.h"
using namespace TransmissionRC;

std::vector<rcTorrent> torrents;

WINDOW *torwin;

int screenmx,screenmy;
int mx=0;
int my=0;
int toffset=0;
bool running=true;

	int winH = 0;
	int winW = 0;
void TransmissionRC::runUI(){
 torrents = TransmissionRC::getTorrents();
	initscr();
	noecho();
	start_color();
	getmaxyx(stdscr,screenmx,screenmy);

	init_pair(1,COLOR_BLACK,COLOR_GREEN);

	 winH = screenmx -3;
	 winW = screenmy-10;
	clear();
	torwin = newwin(winH,winW,3,10);
	refresh();
	wrefresh(torwin);
	drawScreen();
	wmove(torwin,my,mx);
	while(running){
		getKeyPress();
		wrefresh(stdscr);
		drawScreen();
		wmove(torwin,my,mx);
	}

endwin();
}

void TransmissionRC::getKeyPress(){
		int ch = getch();
		switch (ch){
			case 'j':
			if(my>winH-4 && toffset<torrents.size()){
				toffset++;
			}else if(my<winH){
				my+=3;
			}
			  break;
			case 'k':

			if(my<=0&&toffset>0){
				toffset--;
			}else if(my>0){
				my-=3;
			}
			  break;
			case 'q':
			running=false;
			break;
			case 'r':
			torrents=TransmissionRC::getTorrents();
			break;
		}
}

void TransmissionRC::drawScreen(){
	wclear(torwin);
	for(int i=0,t=toffset;t<torrents.size();i++,t++){
		int posy = i+(i*2);
		if(posy==my){
	//	  wmove(stdscr,posy,10);
		  wattron(torwin,A_STANDOUT);
		  wattron(torwin,COLOR_PAIR(1));
		  mvwprintw(torwin,posy,0,torrents[t].Name.c_str());
		  posy++;
		  std::string ln = "";
		  ln.insert(ln.begin(),
				(winW-2)*torrents[t].percentDone,'*');
		  ln.insert(ln.end(),
			      (winW-2)*(1-torrents[t].percentDone),'-');
		  ln.insert(ln.begin(),1,'[');
		  ln.insert(ln.end(),1,']');
		  std::stringstream strper;
		  strper<<(torrents[t].percentDone*100);
		 ln.replace(((ln.length()-2)/2)-(strper.str().length()/2),
			   strper.str().length(),
			   strper.str().c_str());
		  mvwprintw(torwin,posy,0,ln.c_str());

		  wattroff(torwin,A_STANDOUT);
		  wattroff(torwin,COLOR_PAIR(1));

		}else{
		 mvwprintw(torwin,posy,0,torrents[t].Name.c_str());
		  posy++;
		  std::string ln = "";
		  ln.insert(ln.begin(),
				(winW-2)*torrents[t].percentDone,'*');
		  ln.insert(ln.end(),
			      (winW-2)*(1-torrents[t].percentDone),'-');
		  ln.insert(ln.begin(),1,'[');
		  ln.insert(ln.end(),1,']');
		  std::stringstream strper;
		  strper<<(torrents[t].percentDone*100);
		 ln.replace(((ln.length()-2)/2)-(strper.str().length()/2),
			   strper.str().length(),
			   strper.str().c_str());
		  mvwprintw(torwin,posy,0,ln.c_str());
		}
	}
	wrefresh(torwin);
}
