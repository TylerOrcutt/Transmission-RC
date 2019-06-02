#pragma once
#include <gtkmm.h>
#include <string>
#include <iostream>
#include "../TransmissionRC.h"
#include "../TransmissionRPCRequest.h"
//#include "images/gear.xpm"
namespace TransmissionRC{

	class ctrlTorrentStatusBar : public Gtk::Statusbar {
		
		private:
		Gtk::MenuButton * btnSettings;
		Gtk::Label * lblTst;
		Gtk::Menu * menu;
		public:
		ctrlTorrentStatusBar();

	};



}



