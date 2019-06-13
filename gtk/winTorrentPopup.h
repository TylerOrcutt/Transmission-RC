#pragma once
#include <gtkmm.h>
#include <thread>
#include "../args.h"
namespace TransmissionRC{

	class winTorrentPopup:public Gtk::Window {
		public:
			winTorrentPopup(void(*)(std::shared_ptr<Gtk::ListBox>));
			bool lost_Focus(GdkEventFocus*);

	};

}
