#include "ctrlTorrentToolBar.h"
using namespace TransmissionRC;
 static void btnClick(Gtk::Widget *widget){

}
ctrlTorrentToolBar::ctrlTorrentToolBar(std::shared_ptr<Gtk::ListBox>  lbox) : Gtk::Toolbar(){
	
	this->set_toolbar_style(Gtk::TOOLBAR_BOTH);
	//this->set_icon_size(Gtk::ICON_SIZE_SMALL_TOOLBAR);

	this->lstBox = lbox;
//Open
	btnOpen = new Gtk::ToolButton();
	btnOpen->set_label("Open");
	this->append(*btnOpen);
	btnOpen->show();

//Delete
	btnDelete = new Gtk::ToolButton();
	btnDelete->set_label("Delete");
	btnDelete->set_sensitive(false);
	this->append(*btnDelete);
	btnDelete->show();

//Delete img
	Gtk::Image *imgDelete = new Gtk::Image(); 
	imgDelete->set( Gdk::Pixbuf::create_from_xpm_data(delete_xpm));
	btnDelete->set_icon_widget(*imgDelete);
	imgDelete->show();
//--
	Gtk::SeparatorToolItem *sep = new Gtk::SeparatorToolItem();
	this->append(*sep);
	sep->show();

//Pause
	btnPause= new Gtk::ToolButton();
	btnPause->set_label("Pause");
	//btnPause->set_sensitive(false);
	this->append(*btnPause);
	btnPause->show();
//Pause img
	Gtk::Image *imgPause = new Gtk::Image(); 
	imgPause->set( Gdk::Pixbuf::create_from_xpm_data(pause_xpm));
	btnPause->set_icon_widget(*imgPause);
	imgPause->show();

//Resume
	btnResume = new Gtk::ToolButton();
	btnResume->set_label("Start");
	//btnResume->set_sensitive(false);
	this->append(*btnResume);
	btnResume->show();
//Resume img
	Gtk::Image *imgResume = new Gtk::Image(); 
	imgResume->set( Gdk::Pixbuf::create_from_xpm_data(resume_xpm));
	btnResume->set_icon_widget(*imgResume);
	imgResume->show();
//--
	sep = new Gtk::SeparatorToolItem();
	this->append(*sep);
	sep->show();


//Button signals
	btnOpen->signal_clicked().connect(
				sigc::mem_fun(*this,
				&ctrlTorrentToolBar::tbOpen_Clicked));

	btnDelete->signal_clicked().connect(
				sigc::mem_fun(*this,
				&ctrlTorrentToolBar::tbDelete_Clicked));

	btnPause->signal_clicked().connect(
				sigc::mem_fun(*this,
				&ctrlTorrentToolBar::tbPause_Clicked));

	btnResume->signal_clicked().connect(
				sigc::mem_fun(*this,
				&ctrlTorrentToolBar::tbResume_Clicked));
}

void ctrlTorrentToolBar::tbOpen_Clicked(){
}


void ctrlTorrentToolBar::tbDelete_Clicked(){
}

void ctrlTorrentToolBar::tbPause_Clicked(){

	if(lstBox != NULL){

		mtx->lock();
		ctrlTorrentListItem * item = 
				(ctrlTorrentListItem *)lstBox.get()->get_selected_row();
		if(item!=NULL){
			stopTorrent(item->torrent.ID);
		}
		mtx->unlock();
	}
}

void ctrlTorrentToolBar::tbResume_Clicked(){

	if(lstBox != NULL){

		mtx->lock();
		ctrlTorrentListItem * item = 
				(ctrlTorrentListItem *)lstBox.get()->get_selected_row();
		if(item!=NULL){
			resumeTorrent(item->torrent.ID);
		}

		mtx->unlock();
	}
}
