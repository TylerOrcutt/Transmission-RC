#pragma once
#include <gtkmm.h>
#include <thread>
#include "../args.h"
namespace TransmissionRC{

	class winTorrentPopup:public Gtk::Window {
		public:
			winTorrentPopup(void(*)(Gtk::ListBox*));
			bool lost_Focus(GdkEventFocus*);

	};

}
