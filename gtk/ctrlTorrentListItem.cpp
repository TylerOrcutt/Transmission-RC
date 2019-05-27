#include "ctrlTorrentListItem.h"

using namespace TransmissionRC;
 
TransmissionRC::ctrlTorrentListItem * TransmissionRC::ctrlTorrentListItem::makeListItem(rcTorrent torrent ){
	ctrlTorrentListItem * item = new ctrlTorrentListItem();
	GtkWidget *row;
	row = gtk_list_box_row_new();
	

	GtkWidget *wrapper = gtk_box_new(GTK_ORIENTATION_VERTICAL,10);
	gtk_widget_set_name(wrapper,"bxrow");

	item->lblName  = (GtkLabel*)gtk_label_new(torrent.Name.c_str());
	gtk_widget_set_name(GTK_WIDGET(item->lblName),"rlblTitle");
	gtk_label_set_xalign(item->lblName,0);
	gtk_box_pack_start(GTK_BOX(wrapper),GTK_WIDGET(item->lblName),false,false,0);

	std::stringstream ss;
	ss<<c_trStatus[torrent.Status];

	if(torrent.Status>0){
		ss<<" D:"<<Utility::convertTransferSpeed(torrent.rateDownload);
		ss<<" U:"<<Utility::convertTransferSpeed(torrent.rateUpload);
	}
	item->lblStatus = gtk_label_new(ss.str().c_str());
	gtk_widget_set_name(item->lblStatus,"rlbl");
	gtk_label_set_xalign(GTK_LABEL(item->lblStatus),0);
	gtk_box_pack_start(GTK_BOX(wrapper),item->lblStatus,false,false,0);

	
	item->pbar = gtk_progress_bar_new();
	gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(item->pbar),
				      torrent.percentDone);
	
	gtk_box_pack_start(GTK_BOX(wrapper),item->pbar,false,false,0);


	ss.str(std::string());	
	ss<<" downloaded "<<(torrent.totalSize*torrent.percentDone/1024/1024)
	<<" mb";
	item->lblDL = gtk_label_new(ss.str().c_str());
	gtk_widget_set_name(item->lblDL,"rlbl");
	gtk_label_set_xalign(GTK_LABEL(item->lblDL),0);
	gtk_box_pack_start(GTK_BOX(wrapper),item->lblDL,false,false,0);
	
	
	GtkWidget *separator = gtk_separator_new(GTK_ORIENTATION_HORIZONTAL);
	gtk_widget_set_name(separator,"rsep");
	//gtk_box_pack_start(GTK_BOX(wrapper),
	//		   separator,false,false,0);
	gtk_container_add(GTK_CONTAINER(row),wrapper);
	
	item->widget=row;	

	return item;
}
