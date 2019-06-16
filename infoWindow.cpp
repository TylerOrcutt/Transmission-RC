#include "infoWindow.h"

using namespace TransmissionRC;
using namespace TransmissionRC::Terminal;

void
infoWindow::Draw(rcTorrent torrent){
	werase(win);
	int ln =0;


	mvwprintw(win,ln,0,torrent.Name.c_str());

	wrefresh(win);
}

void 
infoWindow::Draw(std::shared_ptr<std::vector<rcTorrent>>torrents){
}
