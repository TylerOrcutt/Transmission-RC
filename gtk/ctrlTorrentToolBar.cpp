#include "ctrlTorrentToolBar.h"

using namespace TransmissionRC;
 static void btnClick(Gtk::Widget *widget){

}
ctrlTorrentToolBar::ctrlTorrentToolBar(Gtk::ListBox * box) : Gtk::Toolbar(){

	this->lstBox = box;
//Open
	btnOpen = new Gtk::ToolButton();
	btnOpen->set_label("Open");
	this->append(*btnOpen);
	btnOpen->show();

//Delete
	btnDelete = new Gtk::ToolButton();
	btnDelete->set_label("Delete");
	this->append(*btnDelete);
	btnDelete->show();

//--
	Gtk::SeparatorToolItem *sep = new Gtk::SeparatorToolItem();
	this->append(*sep);
	sep->show();

//Pause
	btnPause= new Gtk::ToolButton();
	btnPause->set_label("Pause");
	this->append(*btnPause);
	btnPause->show();

//Resume
	btnResume = new Gtk::ToolButton();
	btnResume->set_label("Resume");
	this->append(*btnResume);
	btnResume->show();

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
		ctrlTorrentListItem * item = 
				(ctrlTorrentListItem *)lstBox->get_selected_row();
		if(item!=NULL){
			stopTorrent(item->torrent.ID);
		}
	}

}

void ctrlTorrentToolBar::tbResume_Clicked(){

	if(lstBox != NULL){
		ctrlTorrentListItem * item = 
				(ctrlTorrentListItem *)lstBox->get_selected_row();
		if(item!=NULL){
			resumeTorrent(item->torrent.ID);
		}
	}
	
}
