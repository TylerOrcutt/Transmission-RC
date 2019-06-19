#include "infoWindow.h"

using namespace TransmissionRC;
using namespace TransmissionRC::Terminal;

void
infoWindow::Draw(rcTorrent torrent){
	werase(win);
	int ln =0;

	mvwprintw(win,ln,0,torrent.Name.c_str());
	ln++;
	//
	std::stringstream ss;

	ss.str("");
	ss<<"Uploaded:"<<Utility::convertSize(torrent.uploadedEver).get();
	mvwprintw(win,ln,0,ss.str().c_str());
	ln++;

	ss.str("");
	ss<<"Available:"<<torrent.desiredAvailable;
	mvwprintw(win,ln,0,ss.str().c_str());
	ln++;
	ln++;
//

//files
	mvwprintw(win,ln,0,"Files:");
	ln++;
	for(int i=0;i<torrent.files.size();i++){
		mvwprintw(win,ln,0,("  " + torrent.files[i].name).c_str());
		ln++;

	}

	ln++;

	mvwprintw(win,ln,0,("Error: " +torrent.errorString).c_str());
	ln++;
//
	mvwprintw(win,ln,0,("Comment: " +torrent.comment).c_str());
	ln++;

	wrefresh(win);

}

void 
infoWindow::Draw(std::shared_ptr<std::vector<rcTorrent>>torrents){
}
