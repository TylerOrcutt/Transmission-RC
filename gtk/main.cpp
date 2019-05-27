#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include "ctrlTorrentListItem.h"
#include "../TransmissionRC.h"
#include "../TransmissionRPCRequest.h"
#include "../config.h"
#include "../Utility.h"

using namespace TransmissionRC;

std::map<std::string,std::string>Config::config;
std::string Config::sessionID="";
std::vector<ctrlTorrentListItem*> listRows;

std::mutex mtx;
GtkWidget *lstBox;

static void updateThread(){

	while(true){
		std::vector<rcTorrent> * torrents = getTorrents();		
		if(torrents==NULL){
			if(TransmissionRC::authenticate()){
				torrents=getTorrents();
			}
		}	
		mtx.lock();
		for(int i=0;torrents!=NULL && i<torrents->size();i++){
		   if(i>=listRows.size()){ 

			ctrlTorrentListItem *rowItem = 
				ctrlTorrentListItem::makeListItem((*torrents)[i]);
			listRows.push_back(rowItem);		
			gtk_list_box_insert(GTK_LIST_BOX(lstBox),rowItem->widget,-1);
			
			gtk_widget_show_all(lstBox);
			continue;
		   } 
		
			gtk_label_set_text(GTK_LABEL(listRows[i]->lblName),
					   (*torrents)[i].Name.c_str());
					   
			std::stringstream ss;
			ss<<c_trStatus[(*torrents)[i].Status];

			if((*torrents)[i].Status>0){
				ss<<" D:"<<Utility::convertTransferSpeed((*torrents)[i].rateDownload);
				ss<<" U:"<<Utility::convertTransferSpeed((*torrents)[i].rateUpload);
			}
			gtk_label_set_text(GTK_LABEL(listRows[i]->lblStatus),
					   ss.str().c_str());

			gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(listRows[i]->pbar),
				      (*torrents)[i].percentDone);

			ss.str(std::string());	
			ss<<" downloaded "<<((*torrents)[i].totalSize*(*torrents)[i].percentDone/1024/1024)
			  <<" mb";
			gtk_label_set_text(GTK_LABEL(listRows[i]->lblDL),
					   ss.str().c_str());
		}	
		mtx.unlock();

		if(torrents!=NULL){
			free(torrents);
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(5500));
	}
}

static void  tbItem_Clicked(GtkWidget *widget,gpointer data){
	std::string t = "toolbar item: "  
		""+std::string(
		gtk_tool_button_get_label(GTK_TOOL_BUTTON(widget))) +""
		 " clicked\r\n";
	g_print(t.c_str());

}

static void btn1_Click(GtkWidget *widget, gpointer data){
	if(((ctrlTorrentListItem*)listRows[0])->lblName ==NULL){
		g_print("label NULL\r\n");
	}
	gtk_label_set_text(GTK_LABEL(((ctrlTorrentListItem*)listRows[0])->lblName),"test");
}


static void activate (GtkApplication *app, gpointer user_data){

	GtkWidget *window;
	GtkWidget *stack;
	GtkWidget *toolbar;
	GtkWidget *button;	
//style
	const char *data = "progress,trough{border-radius:5px;"
			   "border:1px solid grey;"
			   //"background-color:green;"
			   "min-height:20px;}"
			   "separator#rsep{min-height:3px; background-color:black;}"
			   "box{background-color:#262626; border-radius:5px;}"
			   "box#bxrow{padding:5px;background-color:#262626;"
				" border-radius:5px;}"
			   "label#rlbl{color:white;}"
			   "label#rlblTitle{color:white;font-size:16px; font-weight:bold;}"
			   "list{background-color:#0d0d0d}";
	GtkCssProvider * provider = gtk_css_provider_new();
	if(!gtk_css_provider_load_from_data(provider,data,std::strlen(data),NULL)){
		g_print("css load failed\r\n");
	}
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
					GTK_STYLE_PROVIDER(provider),
					GTK_STYLE_PROVIDER_PRIORITY_USER);

	//window
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window),"Transmission-RC");
	gtk_window_set_default_size(GTK_WINDOW(window),200,200);

	//baselayout
	stack = gtk_box_new(GTK_ORIENTATION_VERTICAL,0);
	gtk_container_add(GTK_CONTAINER(window),stack);

	//toolbar
	toolbar=gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar),GTK_TOOLBAR_BOTH);
	//gtk_container_add(GTK_CONTAINER(stack),toolbar);

//toolbar items
	//open
	GtkToolItem * tb_openTorrent;
	tb_openTorrent = gtk_tool_button_new(NULL,"Open");
	g_signal_connect(tb_openTorrent,"clicked",
			G_CALLBACK(tbItem_Clicked),NULL);

	//cancel
	GtkToolItem * tb_cancelTorrent;
	tb_cancelTorrent = gtk_tool_button_new(NULL,"Delete");
	g_signal_connect(tb_cancelTorrent,"clicked",
			G_CALLBACK(tbItem_Clicked),NULL);	

	//resume
	GtkToolItem * tb_resumeTorrent;
	tb_resumeTorrent = gtk_tool_button_new(NULL,"Resume");
	g_signal_connect(tb_resumeTorrent,"clicked",
			G_CALLBACK(tbItem_Clicked),NULL);
	//disable resume for now	
	gtk_widget_set_sensitive(GTK_WIDGET(tb_resumeTorrent),false);


	//pause
	GtkToolItem * tb_pauseTorrent;
	tb_pauseTorrent = gtk_tool_button_new(NULL,"Pause");	
	g_signal_connect(tb_pauseTorrent,"clicked",
			G_CALLBACK(tbItem_Clicked),NULL);
	gtk_widget_set_sensitive(GTK_WIDGET(tb_pauseTorrent),false);


	//add items 	
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),tb_openTorrent,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),tb_cancelTorrent,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),
				gtk_separator_tool_item_new(),-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),tb_resumeTorrent,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),tb_pauseTorrent,-1);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar),
				gtk_separator_tool_item_new(),-1);

	
	
	//listbox
	GtkWidget *scrolledWindow;
	scrolledWindow = gtk_scrolled_window_new(NULL,NULL);

	lstBox = gtk_list_box_new();
	gtk_container_add(GTK_CONTAINER(scrolledWindow),lstBox);		
	
	std::vector<rcTorrent> *torrents = getTorrents();
	for(int i=0;i<torrents->size();i++){
		ctrlTorrentListItem *rowItem = 
				ctrlTorrentListItem::makeListItem((*torrents)[i]);
		listRows.push_back(rowItem);		
	 	gtk_list_box_insert(GTK_LIST_BOX(lstBox),rowItem->widget,-1);
	}
	free(torrents);	

	//test button
	button = gtk_button_new_with_label("test");
	//gtk_container_add(GTK_CONTAINER(stack),button);
	g_signal_connect(button,"clicked",G_CALLBACK(btn1_Click),NULL);

	gtk_box_pack_start(GTK_BOX(stack),toolbar,false,false,0);	
	gtk_box_pack_start(GTK_BOX(stack),scrolledWindow,true,true,0);	
	gtk_box_pack_start(GTK_BOX(stack),button,false,false,0);	

	gtk_widget_show_all(window);


	
	std::thread t(updateThread);
	t.detach();

}


int main (int args,char **argv){
	Config::config = Config::loadConfig();
	TransmissionRC::authenticate();
	
	GtkApplication *app;
	int status;
	
	app=gtk_application_new("org.trc",G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app,"activate", G_CALLBACK(activate),NULL);
	status = g_application_run (G_APPLICATION(app),args,argv);
	g_object_unref(app);
return status;
}
