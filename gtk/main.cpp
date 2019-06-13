#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "css.h"
#include "winTorrentPopup.h"
#include "ctrlTorrentListItem.h"
#include "ctrlTorrentToolBar.h"
#include "ctrlTorrentStatusBar.h"
#include "../TransmissionRC.h"
#include "../TransmissionRPCRequest.h"
#include "../config.h"
#include "../Utility.h"
#include "../args.h"
using namespace TransmissionRC;

std::mutex mtx;
std::map<std::string,std::string>Config::config;
std::string Config::sessionID="";

//Gtk::ListBox *lstbox;
ctrlTorrentToolBar * torrentTB;

void showTorrentPopup(int, char**);
static void lstRowSelected(Gtk::ListBoxRow *);
	
static void updateToolbar(int status);

static void updateThread(std::shared_ptr<Gtk::ListBox> lstbox){

	while(true){

		std::vector<rcTorrent> * torrents = getTorrents();		

		if(torrents==NULL){

			if(TransmissionRC::authenticate()){
				torrents=getTorrents();
			}

		}	
		mtx.lock();	
		std::vector<Gtk::Widget*> lrows = lstbox.get()->get_children();
		//need to make this less shitty
		for(int i=0;torrents!=NULL && i<torrents->size();i++){

			if(i>=lrows.size()){
				
				ctrlTorrentListItem * row = 
						new ctrlTorrentListItem(
								(*torrents)[i]); 
				lstbox.get()->append(*row);
				row->show();
				continue;
			}
			
			ctrlTorrentListItem *item = (ctrlTorrentListItem*)lrows[i];
			item->update((*torrents)[i]);
		
		}
		//remove excess rows
		if(torrents!=NULL){
			for(int i= torrents->size();i<lrows.size();i++){
				lstbox.get()->remove(*lrows[i]);	
			}
		}
		//refresh toolbar buttons

		/*Gtk::ListBoxRow *row = lstbox.get()->get_selected_row();
		if(row!=NULL){
		ctrlTorrentListItem *item = (ctrlTorrentListItem*)row;
			updateToolbar(item->torrent.Status);		
		}*/
			
		mtx.unlock();
		//lstRowSelected(row);
		if(torrents!=NULL){
			free(torrents);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
	}
}


static void updateToolbar(int status){
		switch(status){
			case 0:	
			//torrentTB->btnResume->set_sensitive(true);
			//ItorrentTB->btnPause->set_sensitive(false);
			break;
			case 1:	
			case 2:	
			case 3:	
			case 4:	
			case 5:	
			case 6:	
			//torrentTB->btnResume->set_sensitive(false);
			//torrentTB->btnDelete->set_sensitive(true);
			//torrentTB->btnPause->set_sensitive(true);
			break;
		}
}
static void lstRowSelected(Gtk::ListBoxRow *row){
		mtx.lock();

	ctrlTorrentListItem *item = (ctrlTorrentListItem*) row;	
	if(torrentTB != NULL && item!=NULL){
		int status = item->torrent.Status;
		updateToolbar(status);
	}
		mtx.unlock();
}

int main (int args,char **argv){
	Config::config = Config::loadConfig();
	TransmissionRC::authenticate();


	if(args>1){
		showTorrentPopup(args,argv);	
		return 0;
	}

	auto app = Gtk::Application::create(args,argv,"org.trc");
	//need to move this 
	Gtk::Window window;
	window.set_default_size(200,200);

	TransmissionRC::Style::loadCSS((char *)TransmissionRC::Style::style);

	std::shared_ptr<Gtk::ListBox> lstbox = std::make_shared<Gtk::ListBox>();

	Gtk::Box *box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	window.add(*box);
	box->show();

	torrentTB = new ctrlTorrentToolBar(lstbox);
	torrentTB->mtx= &mtx;
	box->add(*torrentTB);
	torrentTB->show();

	Gtk::ScrolledWindow * sw = new Gtk::ScrolledWindow();
	box->pack_start(*sw,true,true,0);
	sw->show();
	sw->add(*lstbox.get());

	lstbox.get()->show();
	lstbox.get()->signal_row_selected().connect(sigc::ptr_fun(&lstRowSelected));

	ctrlTorrentStatusBar * bar = new ctrlTorrentStatusBar();
	box->add(*bar);
	bar->show();
	

	std::thread t(updateThread,lstbox);
	t.detach();

	lstbox.reset();
	return app->run(window);

}


//i3 status bar popup  
void showTorrentPopup(int args,char ** argv){

	int arc =1;
	auto app = Gtk::Application::create(arc,argv,"org.trc.popup");

	TransmissionRC::Style::loadCSS((char *)TransmissionRC::Style::style);
	
	winTorrentPopup *win = new winTorrentPopup(&updateThread);
	app->run(*win);
}
