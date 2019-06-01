#include "ctrlTorrentListItem.h"

using namespace TransmissionRC;

ctrlTorrentListItem::ctrlTorrentListItem(rcTorrent torrent):Gtk::ListBoxRow(){
	Gtk::Box * box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL,10);
	add(*box);
	box->show();

//name label
	lblName = new  Gtk::Label(torrent.Name.c_str(),Gtk::ALIGN_START,Gtk::ALIGN_START);
	lblName->set_name("rlblTitle");
	box->add(*(lblName));
	lblName->show();

//status label

	std::stringstream ss;
	ss<<c_trStatus[torrent.Status];
	
	if(torrent.Status>0){

		ss<<" D:"<<Utility::convertTransferSpeed(torrent.rateDownload);
		ss<<" U:"<<Utility::convertTransferSpeed(torrent.rateUpload);
	}	

	lblStatus = new Gtk::Label(ss.str().c_str(),Gtk::ALIGN_START,Gtk::ALIGN_START);
	lblStatus->set_name("rlbl");
	box->add(*lblStatus);
	lblStatus->show();

//progress bar
	pbar = new Gtk::ProgressBar();
	pbar->set_fraction(torrent.percentDone);
	box->add(*pbar);
	pbar->show();

	
//download rate label	
	ss.str(std::string());
	ss<<" Downloaded "<<(torrent.totalSize*torrent.percentDone)/1024/1024
	  <<" MB";
	lblDL = new Gtk::Label(ss.str().c_str(),Gtk::ALIGN_START,Gtk::ALIGN_START);
	lblDL->set_name("rlbl");
	box->add(*lblDL);
	lblDL->show();	
	

	this->torrent =torrent;
}

void ctrlTorrentListItem::update(rcTorrent torrent){

	this->torrent = torrent;
	lblName->set_label(torrent.Name);

	std::stringstream ss;
	ss<<c_trStatus[torrent.Status];
	
	if(torrent.Status>0){

		ss<<" D:"<<Utility::convertTransferSpeed(torrent.rateDownload);
		ss<<" U:"<<Utility::convertTransferSpeed(torrent.rateUpload);
	}	
	lblStatus->set_label(ss.str().c_str());


	ss.str(std::string());
	ss<<" Downloaded "<<(torrent.totalSize*torrent.percentDone)/1024/1024
	  <<" MB";
	lblDL->set_label(ss.str().c_str());

	pbar->set_fraction(torrent.percentDone);
}
