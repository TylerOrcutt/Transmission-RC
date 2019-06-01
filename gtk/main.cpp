#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "ctrlTorrentListItem.h"
#include "../TransmissionRC.h"
#include "../TransmissionRPCRequest.h"
#include "../config.h"
#include "../Utility.h"

using namespace TransmissionRC;

std::map<std::string,std::string>Config::config;
std::string Config::sessionID="";
Gtk::ListBox *lstbox;

std::mutex mtx;

static void updateThread(){
	while(true){

		std::vector<rcTorrent> * torrents = getTorrents();		

		if(torrents==NULL){

			if(TransmissionRC::authenticate()){
				torrents=getTorrents();
			}

		}	
		mtx.lock();
		
		std::vector<Gtk::Widget*> lrows = lstbox->get_children();
		//need fix this
		for(int i=0;i<torrents->size();i++){

			if(i>=lrows.size()){
				
				ctrlTorrentListItem * row = new  ctrlTorrentListItem((*torrents)[i]); 
				lstbox->append(*row);
				row->show();
				continue;
			}
			
			ctrlTorrentListItem *item = (ctrlTorrentListItem*)lrows[i];
			item->update((*torrents)[i]);
		
		}
		mtx.unlock();

		if(torrents!=NULL){
			free(torrents);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5500));
	}
}


static void lstRowSelected(Gtk::ListBoxRow *row){
	ctrlTorrentListItem *item = (ctrlTorrentListItem*) row;	
	std::cout<<"Row Selected: "<<item->torrent.Name<<"\r\n";
}

int main (int args,char **argv){

	Config::config = Config::loadConfig();
	TransmissionRC::authenticate();

	auto app = Gtk::Application::create(args,argv,"org.trc");
	Gtk::Window window;
	window.set_default_size(200,200);

	const char *style = "progress,trough{border-radius:5px;"
			   "border:1px solid grey;"
			   //"background-color:green;"
			   "min-height:20px;}"
			   "separator#rsep{min-height:3px; background-color:black;}"
			   "box{background-color:#262626; border-radius:5px;}"
			   "box#bxrow{padding:5px;background-color:#262626;"
				" border-radius:5px;}"
			   "label#rlbl{color:white;}"
			   "label#rlblTitle{color:white;font-size:16px; font-weight:bold;}"
			   "list{background-color:#0d0d0d}";

	GtkCssProvider * provider = gtk_css_provider_new();

	if(!gtk_css_provider_load_from_data(provider,style,std::strlen(style),NULL)){
		g_print("css load failed\r\n");
	}

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
					GTK_STYLE_PROVIDER(provider),
					GTK_STYLE_PROVIDER_PRIORITY_USER);

	lstbox = new Gtk::ListBox();
	Gtk::Box *box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	window.add(*box);
	box->show();

	Gtk::ScrolledWindow * sw = new Gtk::ScrolledWindow();
	box->pack_start(*sw,true,true,0);
	sw->show();
	sw->add(*lstbox);

	lstbox->show();
	lstbox->signal_row_selected().connect(sigc::ptr_fun(&lstRowSelected));

	Gtk::Button* btn1 = new Gtk::Button("Test");
	box->add(*btn1);
	btn1->show();

	std::thread t(updateThread);
	t.detach();

	return app->run(window);
}
