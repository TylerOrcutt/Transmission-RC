#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "ctrlTorrentListItem.h"
#include "ctrlTorrentToolBar.h"
#include "ctrlTorrentStatusBar.h"
#include "../TransmissionRC.h"
#include "../TransmissionRPCRequest.h"
#include "../config.h"
#include "../Utility.h"
#include "../args.h"
using namespace TransmissionRC;

std::map<std::string,std::string>Config::config;
std::string Config::sessionID="";

Gtk::ListBox *lstbox;
ctrlTorrentToolBar * torrentTB;

std::mutex mtx;
void showTorrentPopup(int, char**);
static void lstRowSelected(Gtk::ListBoxRow *);
	
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
		//need to make this less shitty
		for(int i=0;i<torrents->size();i++){

			if(i>=lrows.size()){
				
				ctrlTorrentListItem * row = 
						new ctrlTorrentListItem((*torrents)[i]); 
				lstbox->append(*row);
				row->show();
				continue;
			}
			
			ctrlTorrentListItem *item = (ctrlTorrentListItem*)lrows[i];
			item->update((*torrents)[i]);
		
		}
		//remove excess rows
		if(torrents!=NULL){
			for(int i= torrents->size();i<lrows.size();i++){
				lstbox->remove(*lrows[i]);	
			}
		}
		//refresh toolbar buttons
		lstRowSelected(lstbox->get_selected_row());
		mtx.unlock();

		if(torrents!=NULL){
			free(torrents);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5500));
	}
}

static void Btn1_Clicked(){
	std::cout<<"btn clicked\r\n";
}
static void lstRowSelected(Gtk::ListBoxRow *row){
	ctrlTorrentListItem *item = (ctrlTorrentListItem*) row;	
	//std::cout<<"Row Selected: "<<item->torrent.Name<<"\r\n";
	if(torrentTB != NULL && item!=NULL){
		switch(item->torrent.Status){
			case 0:	
			torrentTB->btnResume->set_sensitive(true);
			torrentTB->btnPause->set_sensitive(false);
			break;
			case 1:	
			case 2:	
			case 3:	
			case 4:	
			case 5:	
			case 6:	
			torrentTB->btnResume->set_sensitive(false);
			torrentTB->btnDelete->set_sensitive(true);
			torrentTB->btnPause->set_sensitive(true);
			break;
		}
	}
}

void loadCSS(){

	const char *style = "progress,trough{border-radius:5px;"
			   "border:1px solid grey;"
			   //"background-color:green;"
			   "min-height:20px;}"
			   "toolbar>separator{min-height:3px; background-color:white;}"
			   "box{border-color:red; }"
			   "window{background-color:rgba(0,0,0,0);}"
			   //"#vbox{border:4px solid red; border-radius:0px 0px 9px 9px;"
			      //"padding:0px; margin:0px;}"
			   "toolbar * {margin:3px;}"
			   //"box{background-color:#262626; border-radius:
			   //"box#bxrow{padding:15px;background-color:#262626;"
				//" border-radius:5px;}"
			   "label#rlbl{color:white;}"
			   "label#rlblTitle{color:white;font-size:16px; font-weight:bold;}"
			   "list{background-color:#0d0d0d}"
			   "button{color:#fff; background-color:#262626}"
			   "button:hover{color:#fff; background-color:blue}"
			   "list>row:selected{color:#fff; background-color:blue}"
			   "list>row{margin:3px;padding:10px;background-color:#262626;"
				" border-radius:5px;}"
			   "toolbar{background-color:#1a1a1a; border:2px solid #0f0f0f; padding:4px;}"
			   "statusbar>label{color:red;}"
			   "progressbar{color:#fff; }";

	GtkCssProvider * provider = gtk_css_provider_new();

	if(!gtk_css_provider_load_from_data(provider,style,std::strlen(style),NULL)){
		g_print("css load failed\r\n");
	}

	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
					GTK_STYLE_PROVIDER(provider),
					GTK_STYLE_PROVIDER_PRIORITY_USER);

}
int main (int args,char **argv){
	Config::config = Config::loadConfig();
	TransmissionRC::authenticate();


	if(args>1){
		showTorrentPopup(args,argv);	
		return 0;
	}

	auto app = Gtk::Application::create(args,argv,"org.trc");
	Gtk::Window window;
	window.set_default_size(200,200);

	loadCSS();

	lstbox = new Gtk::ListBox();
	Gtk::Box *box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	window.add(*box);
	box->show();

	torrentTB = new ctrlTorrentToolBar(lstbox);
	box->add(*torrentTB);
	torrentTB->show();

	Gtk::ScrolledWindow * sw = new Gtk::ScrolledWindow();
	box->pack_start(*sw,true,true,0);
	sw->show();
	sw->add(*lstbox);

	lstbox->show();
	lstbox->signal_row_selected().connect(sigc::ptr_fun(&lstRowSelected));

	ctrlTorrentStatusBar * bar = new ctrlTorrentStatusBar();
	box->add(*bar);
	bar->show();
	
/*	Gtk::Button* btn1 = new Gtk::Button("Test");
	box->add(*btn1);
	btn1->show();
	btn1->signal_clicked().connect(sigc::ptr_fun(&Btn1_Clicked));
*/
	std::thread t(updateThread);
	t.detach();

	return app->run(window);

}


	Gtk::Dialog *dia;
//i3 status bar popup ?? 
void showTorrentPopup(int args,char ** argv){

	Gtk::Main gtkw(args,argv);
	loadCSS();
	dia = new Gtk::Dialog();
	int width = 350,height = 375;
	dia->set_default_size(width,height);
	dia->set_border_width(0);
	dia->set_decorated(false);

	auto dsp = Gdk::Display::get_default();
	auto scrn = dsp->get_default_screen();

	auto grb = dsp->get_default_seat()->get_pointer();
	if(!grb){
		std::cout<<"grab failed";
	}
	int x,y;
	grb->get_position(x,y);

//TODO: what if this needs to pop up from the bottom?
	dia->move(x-width/2,y+20);
	dia->show();
	
	lstbox = new Gtk::ListBox();
	Gtk::Box *box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	dia->get_vbox()->set_border_width(0);
	dia->get_vbox()->set_name("vbox");
	dia->get_vbox()->pack_start(*box,true,true,0);
	box->show();


	Gtk::ScrolledWindow * sw = new Gtk::ScrolledWindow();
	box->pack_start(*sw,true,true,0);
	sw->show();
	sw->add(*lstbox);

	lstbox->show();
	lstbox->signal_row_selected().connect(sigc::ptr_fun(&lstRowSelected));

	
	std::thread t(updateThread);
	t.detach();

	dia->signal_focus_out_event().connect([](GdkEventFocus *ev ){
			dia->close();
			i3Status();
			return false;
			});
	dia->run();
}
