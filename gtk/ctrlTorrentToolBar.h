#pragma once
#include<iostream>
#include<gtkmm.h>
#include<gtkmm/toolbar.h>
#include<string>
#include "ctrlTorrentListItem.h"
#include "../TransmissionRPCRequest.h"

#include "images/delete.xpm"
#include "images/pause.xpm"
#include "images/resume.xpm"

namespace TransmissionRC{

	class ctrlTorrentToolBar : public Gtk::Toolbar{
		private:
		public:
			Gtk::ToolButton * btnOpen;
			Gtk::ToolButton * btnDelete;

			Gtk::ToolButton * btnPause;
			Gtk::ToolButton * btnResume;
			
		  	Gtk::Image *imgPause;			
			Gtk::ListBox * lstBox;
		public:
			ctrlTorrentToolBar(Gtk::ListBox *);	
			void tbOpen_Clicked();
			void tbDelete_Clicked();
			void tbPause_Clicked();
			void tbResume_Clicked();
	};
}
