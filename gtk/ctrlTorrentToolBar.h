#pragma once
#include<iostream>
#include<gtkmm.h>
#include<gtkmm/toolbar.h>
#include<string>


namespace TransmissionRC{

	class ctrlTorrentToolBar : public Gtk::Toolbar{
		private:
			Gtk::ToolButton * btnOpen;
			Gtk::ToolButton * btnDelete;

			Gtk::ToolButton * btnPause;
			Gtk::ToolButton * btnResume;

		public:
			ctrlTorrentToolBar();	
			void tbOpen_Clicked();
			void tbDelete_Clicked();
			void tbPause_Clicked();
			void tbResume_Clicked();
	};
}
