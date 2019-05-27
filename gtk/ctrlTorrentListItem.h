#pragma once
#include <gtk/gtk.h>
#include <string>
#include "../TransmissionRC.h"
#include "../Utility.h"
namespace TransmissionRC{

	class ctrlTorrentListItem{
		public:
		TransmissionRC::rcTorrent torrent;
		GtkLabel  *lblName;
		GtkWidget *lblStatus;
		GtkWidget *pbar;
		GtkWidget *lblDL;
		GtkWidget * widget;
		static ctrlTorrentListItem * makeListItem(TransmissionRC::rcTorrent);

	};




};
