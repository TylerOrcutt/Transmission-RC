#include <gtk/gtk.h>
#include <map>
#include <string>
#include <vector>
#include "../TransmissionRC.h"
#include "../TransmissionRPCRequest.h"
#include "../config.h"

using namespace TransmissionRC;

std::map<std::string,std::string>Config::config;
std::string Config::sessionID="";

static void  tbItem_Clicked(GtkWidget *widget,gpointer data){
	std::string t = "toolbar item: "  
		""+std::string(
		gtk_tool_button_get_label(GTK_TOOL_BUTTON(widget))) +""
		 " clicked\r\n";
	g_print(t.c_str());

}

static void btn1_Click(GtkWidget *widget, gpointer data){

}

GtkWidget * makeRow(std::string text){
	GtkWidget *row;
	row = gtk_list_box_row_new();
	
	GtkWidget * label = gtk_label_new(text.c_str());
	gtk_container_add(GTK_CONTAINER(row),label);

	return row;
}

static void activate (GtkApplication *app, gpointer user_data){

	GtkWidget *window;
	GtkWidget *stack;
	GtkWidget *toolbar;
	GtkWidget *button;	

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
	tb_cancelTorrent = gtk_tool_button_new(NULL,"Cancel");
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
	GtkWidget* listbox;
	listbox = gtk_list_box_new();
	gtk_container_add(GTK_CONTAINER(scrolledWindow),listbox);		
	
	std::vector<rcTorrent> *torrents = getTorrents();
	for(int i=0;i<torrents->size();i++){
	 gtk_list_box_insert(GTK_LIST_BOX(listbox),
			makeRow((*torrents)[i].Name),-1);
	}
	

	//test button
	button = gtk_button_new_with_label("test");
	//gtk_container_add(GTK_CONTAINER(stack),button);
	g_signal_connect(button,"clicked",G_CALLBACK(btn1_Click),NULL);

	gtk_box_pack_start(GTK_BOX(stack),toolbar,false,false,0);	
	gtk_box_pack_start(GTK_BOX(stack),scrolledWindow,true,true,0);	
	gtk_box_pack_start(GTK_BOX(stack),button,false,false,0);	

	gtk_widget_show_all(window);

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
