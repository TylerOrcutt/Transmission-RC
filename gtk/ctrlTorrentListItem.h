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
		ctrlTorrentListItem():Gtk::ListBoxRow(){

		}
		Gtk::Label * lblName;
	//	TransmissionRC::rcTorrent torrent;
	//	GtkLabel  *lblName;
	//	GtkWidget *lblStatus;
	//	GtkWidget *pbar;
	//	GtkWidget *lblDL;
	//	GtkWidget * widget;
		static  ctrlTorrentListItem    *makeListItem(rcTorrent);

	};




};
