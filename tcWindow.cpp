#include "tcWindow.h"
using namespace TransmissionRC::Terminal;

void tcWindow::Draw(std::shared_ptr<std::vector<rcTorrent>> torrents){
	werase(win);

//	std::cout<<winH<<std::endl;
	//return;

	for(int i=0,t=offset;torrents !=nullptr &&t<torrents->size();i++,t++){
		int posy = i+(i*3);

		if(posy/4>=winH/4){
		  break;
		}

		if(posy==my){
		  wattron(win,A_STANDOUT);
		  wattron(win,COLOR_PAIR(1));
		}
		
		std::string ln = (*torrents)[t].Name;
		ln.insert(ln.length(),winW-ln.length(),' ');

		mvwprintw(win,posy,0,ln.c_str());

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

		mvwprintw(win,posy,0,ln.c_str());
		posy++;

		strper.str(std::string());
		strper<<std::string("["+std::string(
					c_trStatus[(*torrents)[t].Status])+"]")
		      <<"  "<<"D:"
		      <<Utility::convertTransferSpeed(
				      (*torrents)[t].rateDownload).get()
		      <<"  "<<"U:"
		      <<Utility::convertTransferSpeed(
				      (*torrents)[t].rateUpload).get();

		ln = strper.str();
		ln.insert(ln.length(),winW-ln.length(),' ');
 
		mvwprintw(win,posy,0,ln.c_str());

		wattroff(win,A_STANDOUT);
		wattroff(win,COLOR_PAIR(1));
	}
	wrefresh(win);


}
