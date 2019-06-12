#ifndef __TRANSMISSION_STYLE_H
#define __TRANSMISSION_STYLE_H
namespace TransmissionRC::Style {

 const  char  *style = "progress,trough{border-radius:5px;"
			   "border:1px solid grey;"
			   //"background-color:green;"
			   "min-height:20px;}"
			   "toolbar>separator{min-height:3px; background-color:white;}"
			   "box{border-color:red; }"
			   "window{background-color:rgba(1,0,0,9); border-radius:30px 30px;}"
			   "#vbox{border:4px solid red; border-radius:0px 0px 20px 20px;"
			      "padding:0px; margin:0px;}"
			   "toolbar * {margin:3px;}"
			   //"box{background-color:#262626; border-radius:
			   //"box#bxrow{padding:15px;background-color:#262626;"
				//" border-radius:5px;}"
			   "label#rlbl{color:white;}"
			   "label#rlblTitle{color:white;font-size:16px; font-weight:bold;}"
			   "list{background-color:#0d0d0d}"
			   "button{color:#fff; background-color:#262626}"
			   "button:hover{color:#fff; background-color:blue}"
			   "list>row:selected{color:#fff; background-color:blue}"
			   "list>row{margin:3px;padding:10px;background-color:#262626;"
				" border-radius:5px;}"
			   "toolbar{background-color:#1a1a1a; border:2px solid #0f0f0f; padding:4px;}"
			   "statusbar>label{color:red;}"
			   "progressbar{color:#fff; }";


	bool loadCSS( char * style){

		GtkCssProvider * provider = gtk_css_provider_new();

		if(!gtk_css_provider_load_from_data(provider,
						    style,
						    strlen(style),NULL)){

			return false;	
		}

		gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
					GTK_STYLE_PROVIDER(provider),
					GTK_STYLE_PROVIDER_PRIORITY_USER);
		return true;
	}
}
#endif
