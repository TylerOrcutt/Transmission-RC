#pragma once
#include <gtk/gtk.h>
#include <gtkmm.h>
#include <gtkmm/widget.h>
#include <gtkmm/listboxrow.h>
#include <string>
#include "../TransmissionRC.h"
#include "../Utility.h"
namespace TransmissionRC{

	class ctrlTorrentListItem:public Gtk::ListBoxRow{
		public:
		Gtk::Label * lblName;
		Gtk::Label * lblStatus;
		Gtk::ProgressBar * pbar;
		Gtk::Label * lblDL;
		TransmissionRC::rcTorrent torrent;

		ctrlTorrentListItem(rcTorrent torrent) ;
		void update(rcTorrent torrent);
		

	};




}
