#include "winTorrentPopup.h"


using namespace TransmissionRC;

winTorrentPopup::winTorrentPopup(void(*updatefnc)(Gtk::ListBox *)):Gtk::Window(Gtk::WINDOW_TOPLEVEL){
	
	int width = 350,height = 375;
	this->set_type_hint(Gdk::WINDOW_TYPE_HINT_DIALOG);
	//this->set_events(Gdk::FOCUS_CHANGE_MASK);
	this->set_can_focus(true);
	this->set_events(Gdk::FOCUS_CHANGE_MASK);
	this->set_default_size(width,height);
	this->set_border_width(0);
	this->set_decorated(false);

	auto dsp = Gdk::Display::get_default();
	auto scrn = dsp->get_default_screen();

	auto grb = dsp->get_default_seat()->get_pointer();
	if(!grb){
		std::cout<<"grab failed";
	}

	int x,y,yoffset = 20;

	grb->get_position(x,y);
	

	this->move(x-width/2,y+yoffset);
	this->show();
	
	lstbox = std::make_shared<Gtk::ListBox>();
	Gtk::Box *box = new Gtk::Box(Gtk::ORIENTATION_VERTICAL);
	
	//t->get_vbox()->set_border_width(0);
	//dia->get_vbox()->set_name("vbox");
	//dia->get_vbox()->pack_start(*box,true,true,0);
	this->add(*box);
	box->show();


	Gtk::ScrolledWindow * sw = new Gtk::ScrolledWindow();
	box->pack_start(*sw,true,true,0);
	sw->show();
	sw->add(*lstbox);

	lstbox->show();
	//lstbox->signal_row_selected().connect(sigc::ptr_fun(&lstRowSelected));

	
	this->signal_focus_out_event().connect(
					sigc::mem_fun(*this,
					&winTorrentPopup::lost_Focus));


	std::thread t(*updatefnc,lstbox.get());
	t.detach();
}
bool winTorrentPopup::lost_Focus(GdkEventFocus* event){
	this->close();
	//i3Status();
	delete(this);
	return true;
}
