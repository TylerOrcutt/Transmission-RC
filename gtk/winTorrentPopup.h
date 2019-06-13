#pragma once
#include <gtkmm.h>
#include <thread>
#include "../args.h"
namespace TransmissionRC{

	class winTorrentPopup:public Gtk::Window {
		private:
		std::shared_ptr<Gtk::ListBox> lstbox;
		public:
			winTorrentPopup(void(*)(Gtk::ListBox*));
			bool lost_Focus(GdkEventFocus*);

	};

}
