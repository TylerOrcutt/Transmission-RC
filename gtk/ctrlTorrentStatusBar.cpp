#include "ctrlTorrentStatusBar.h"

using namespace TransmissionRC;

ctrlTorrentStatusBar::ctrlTorrentStatusBar() : Gtk::Statusbar(){
	
	this->set_halign(Gtk::ALIGN_START);	


	btnSettings = new Gtk::MenuButton();
	btnSettings->set_label("settings");
	
	//Gtk::Image *img =new Gtk::Image();
	//img->set(Gdk::Pixbuf::create_from_xpm_data(gear_xpm));
	//btnSettings->set_image(*img);
	//img->show();
	
	this->pack_end(*btnSettings,false,false);
	btnSettings->show();
	
	 menu = new Gtk::Menu();
	menu->set_halign(Gtk::ALIGN_START);

	Gtk::Label * lbl = new Gtk::Label("test");
	Gtk::MenuItem * mi = new Gtk::MenuItem(*lbl);
	mi->set_halign(Gtk::ALIGN_START);	

	
	Gtk::Label * lbl2 = new Gtk::Label("test2");
	Gtk::MenuItem * mi2 = new Gtk::MenuItem(*lbl2);
	mi2->set_halign(Gtk::ALIGN_START);	

	Gtk::Menu * submenu = new Gtk::Menu();
	mi2->set_submenu(*submenu);			
	submenu->show();

	menu->append(*mi);
	menu->append(*mi2);
	lbl->show();
	lbl2->show();
	mi->show();
	mi2->show();
	btnSettings->set_menu(*menu);
	btnSettings->set_direction(Gtk::ARROW_UP);

	//menu->show();	
/*	
	btnSettings->signal_clicked().connect([this](){
	});
*/
}
