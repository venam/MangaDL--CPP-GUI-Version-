/*
COPYRIGHT AND PERMISSION NOTICE

Copyright (c) 2013, Patrick Louis <patrick at unixhub.net>

All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    1.  The author is informed of the use of his/her code. The author does not have to consent to the use; however he/she must be informed.
    2.  If the author wishes to know when his/her code is being used, it the duty of the author to provide a current email address at the top of his/her code, above or included in the copyright statement.
    3.  The author can opt out of being contacted, by not providing a form of contact in the copyright statement.
    4.  If any portion of the author's code is used, credit must be given.
            a. For example, if the author's code is being modified and/or redistributed in the form of a closed-source binary program, then the end user must still be made somehow aware that the author's work has contributed to that program.
            b. If the code is being modified and/or redistributed in the form of code to be compiled, then the author's name in the copyright statement is sufficient.
    5.  The following copyright statement must be included at the beginning of the code, regardless of binary form or source code form.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Except as contained in this notice, the name of a copyright holder shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization of the copyright holder.
 */ 

#include "gui.hpp"


/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */
gui::~gui()
{
	delete gui_config;
	delete downloader;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Initialize the GtkWidget Elements */
gui::gui(int argc, char *argv[]) 
{
	/* Initialize GTK+ */
	g_log_set_handler ("Gtk", G_LOG_LEVEL_DEBUG, (GLogFunc) gtk_false, NULL);
	gtk_init (&argc, &argv);
	g_log_set_handler ("Gtk", G_LOG_LEVEL_DEBUG, g_log_default_handler, NULL);
	gui_config           = new config();
	init_config();

	/* the main win */
	win                  = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	/* vbox -- contains 7 hbox */
	vbox                 = gtk_vbox_new (false,0);
	vbox2                = gtk_vbox_new(false,0);
	vbox3                = gtk_vbox_new(false,0);

	mainvbox             = gtk_vbox_new(false,0);
	hbox                 = gtk_hbox_new (false,10);
	stop_button          = gtk_button_new_with_label("Stop");
	/* Create a label for the button */
	label                = gtk_label_new ("");

	stat_label           = gtk_label_new ("");
	image_buton          = gtk_button_new();
	add_queue_buton      = gtk_button_new();

	// imbox;
	wheel                = gtk_spin_button_new_with_range(1,2000,1);
	wheel2               = gtk_spin_button_new_with_range(1,2000,1);
	texta                = gtk_entry_new();
	std::string loc_str  = "Downloader Location "+ gui_config->cfg_current_location;
	location_text        = gtk_label_new( loc_str.c_str() );
	dl_manager_combo     = gtk_combo_box_new_text ();
	name_dl              = gtk_entry_new();
	command_dl           = gtk_entry_new();
	queue_combo          = gtk_combo_box_text_new();
	q_name_entry         = gtk_entry_new();
	q_start_entry        = gtk_spin_button_new_with_range(1,2000,1);
	q_end_entry          = gtk_spin_button_new_with_range(1,2000,1);
	pbar1                = gtk_progress_bar_new();
	pbar2                = gtk_progress_bar_new();
	provider_combo       = gtk_combo_box_new_text ();
	revert_item          = gtk_menu_item_new_with_mnemonic ("_Revert Configs");
	accel_group          = gtk_accel_group_new ();

	//queue              = ;
	//factory
	
	downloader           = NULL;

	//current_location  = db.get_mg_location();
	//download_managers = db.get_dl_managers();
	downloader_state     = 0;
	init_callbacks();
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* initialize config */
void
gui::init_config()
{
	gui_config->set_current_location(gui_config->get_mg_location());
	gui_config->set_revert_current_location(gui_config->get_mg_location());

	gui_config->set_download_managers(gui_config->db_get_dl_managers());
	gui_config->set_revert_download_managers(gui_config->db_get_dl_managers());

	gui_config->set_current_provider("mangareader");
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* initialize the callbacks */
void 
gui::init_callbacks()
{
	callbacks_data.callbacks_win = win;
	callbacks_data.callbacks_vbox = vbox;
	callbacks_data.callbacks_vbox2 = vbox2;
	callbacks_data.callbacks_vbox3 = vbox3;
	callbacks_data.callbacks_mainvbox = mainvbox;
	callbacks_data.callbacks_hbox = hbox;
	callbacks_data.callbacks_stop_button = stop_button;
	callbacks_data.callbacks_label = label ;
	callbacks_data.callbacks_stat_label = stat_label;
	callbacks_data.callbacks_image_buton= image_buton;
	callbacks_data.callbacks_add_queue_buton= add_queue_buton;
	callbacks_data.callbacks_imbox = imbox;
	callbacks_data.callbacks_wheel =wheel ;
	callbacks_data.callbacks_wheel2 = wheel2 ;
	callbacks_data.callbacks_texta = texta;
	callbacks_data.callbacks_location_text = location_text;
	callbacks_data.callbacks_dl_manager_combo =dl_manager_combo;
	callbacks_data.callbacks_name_dl = name_dl;
	callbacks_data.callbacks_command_dl = command_dl;
	callbacks_data.callbacks_queue_combo = queue_combo ;
	callbacks_data.callbacks_q_name_entry = q_name_entry;
	callbacks_data.callbacks_q_start_entry = q_start_entry;
	callbacks_data.callbacks_q_end_entry = q_end_entry;
	callbacks_data.callbacks_pbar1 = pbar1 ;
	callbacks_data.callbacks_pbar2 =pbar2 ;
	callbacks_data.callbacks_provider_combo = provider_combo;
	callbacks_data.callbacks_revert_item = revert_item;
	callbacks_data.callbacks_accel_group = accel_group;

	//the state of the download
	callbacks_data.callbacks_downloader_state = &downloader_state ;

	//the downloader
	callbacks_data.callbacks_downloader = downloader;
	
	//the queue to be downloaded
	//callbacks_data.callbacks_config->set_queue( queue) ;  

	//config which contains the temporary data and the revert data
	/*
	std::string                   cfg_current_location;
	std::vector<dl_mngr>          cfg_download_managers;
	std::vector<manga_queue_data> cfg_queue;

	std::string                   cfg_revert_current_location;
	std::vector<dl_mngr>          cfg_revert_download_managers;
	*/
	callbacks_data.callbacks_config = gui_config;
	callbacks_data.callbacks_factory = &factory;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* About Pop Up */
void 
gui::about_this (GtkWidget *wid, GtkWidget *win)
{
	GtkWidget *dialog = NULL;
	dialog            = gtk_message_dialog_new (GTK_WINDOW (win), 
			GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, 
"MangaDL (beta) v0.2 \n\
Coded By Venam \n\
<patrick@unixhub.net>");
	gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

void 
gui::wpopup( std::string message )
{
	GtkWidget *dialog1 = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
		GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, message.c_str());
	gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dialog1));
	gtk_widget_destroy (dialog1);
}

/* Delete Event to quit the GUI */
gboolean 
gui::delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
	/* If you return FALSE in the "delete-event" signal handler,
	 * GTK will emit the "destroy" signal. Returning TRUE means
	 * you don't want the window to be destroyed.
	 * This is useful for popping up 'are you sure you want to quit?'
	 * type dialogs. */
	//DEBUG
#ifdef DEBUG
	g_print ("delete event occurred\n");
#endif
	/* Change TRUE to FALSE and the main window will be destroyed with
	 * a "delete-event". */
	gtk_main_quit();
	return FALSE;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Set the image in a box */
GtkWidget *
gui::xpm_label_box( gchar *xpm_filename, gchar *label_text )
{
	GtkWidget *box;
	GtkWidget *image;
	/* Create box for image and label */
	box = gtk_hbox_new (true, 0);
	gtk_container_set_border_width (GTK_CONTAINER (box), 2);
	/* Now on to the image stuff */
	image = gtk_image_new_from_file (xpm_filename);
	/* Pack the image and label into the box */
	gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 0);
	//gtk_box_pack_start (GTK_BOX (box), label_text, FALSE, FALSE, 0);
	gtk_widget_show (image);
	//gtk_widget_show (label);
	return box;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* call back used when the download button is pressed */
void 
gui::download_callback(GtkWidget *wid, gpointer user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;

	/* nothing state */
	if( *user_callbacks->callbacks_downloader_state == 0) {
		
		/* Check if the queue is empty */
		if ( (user_callbacks->callbacks_config->cfg_queue).size() == 0 ) {
			wpopup("The queue is empty");
			return;
		}

		gtk_button_set_label( (GtkButton*)user_callbacks->callbacks_image_buton, "Pause");
		/* download state */
		*user_callbacks->callbacks_downloader_state = 1;
		/* that's how to get the provider */
		//gtk_combo_box_get_active_text( (GtkComboBox*) user_callbacks->callbacks_provider_combo );

		/* start the thread that manages the download */
		pthread_t th_1;
		pthread_create(&th_1,NULL,&start_download, user_callbacks);
		return;
	}
	/* download state */
	else if( *user_callbacks->callbacks_downloader_state == 1) {
		gtk_button_set_label( (GtkButton*)user_callbacks->callbacks_image_buton, "Unpause");
		/* pause state */
		*user_callbacks->callbacks_downloader_state = 2;
		/* pause the current class instance */
		user_callbacks->callbacks_downloader->pause_unpause();
		return;
	}
	/* pause state */
	else if ( *user_callbacks->callbacks_downloader_state ==2) {
		gtk_button_set_label( (GtkButton*)user_callbacks->callbacks_image_buton, "Pause");
		/* download state */
		*user_callbacks->callbacks_downloader_state = 1;

		user_callbacks->callbacks_downloader->pause_unpause();
		return;
	}
	gtk_widget_show_all (user_callbacks->callbacks_win);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* function called as a new thread */
void*
gui::start_download( void *user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;
	std::string download_command = "default";
	std::string download_location = "";
	for (unsigned int i=0; i< (user_callbacks->callbacks_config->get_download_managers() ).size() ; i++) {
		if ( user_callbacks->callbacks_config->cfg_download_managers[i].selected) {
			download_command = user_callbacks->callbacks_config->cfg_download_managers[i].command ;
		}
		else {
			// it's the default download manager
		}
	}
	download_location = user_callbacks->callbacks_config->cfg_current_location;

	g_timeout_add_seconds (3, update_bars, (gpointer) user_callbacks);
	/* download until the queue is empty */
	while ( user_callbacks->callbacks_config->cfg_queue.size() != 0 ) {
		gtk_entry_set_text( (GtkEntry*)user_callbacks->callbacks_q_name_entry, " ");
		gtk_spin_button_set_value( (GtkSpinButton*)user_callbacks->callbacks_q_start_entry, 1);
		gtk_spin_button_set_value( (GtkSpinButton*)user_callbacks->callbacks_q_end_entry, 1);
		/* initialized the progress bars */
		gtk_progress_bar_set_fraction( (GtkProgressBar*) user_callbacks->callbacks_pbar1, 0.0);
		gtk_progress_bar_set_fraction( (GtkProgressBar*) user_callbacks->callbacks_pbar2, 0.0);
		/* put some status text */
		std::string stat_text = 
			user_callbacks->callbacks_config->cfg_queue[0].manga_name 
			+": "+user_callbacks->callbacks_config->cfg_queue[0].start_chapter
			+"->"+ user_callbacks->callbacks_config->cfg_queue[0].end_chapter;
		gtk_label_set_text( (GtkLabel*) user_callbacks->callbacks_stat_label, stat_text.c_str() );

		user_callbacks->callbacks_downloader = user_callbacks->callbacks_factory->get_provider(
				gtk_combo_box_get_active_text( 
					(GtkComboBox*) user_callbacks->callbacks_provider_combo )
				);

		user_callbacks->callbacks_downloader->init( 
				user_callbacks->callbacks_config->cfg_queue[0].manga_name,
				download_location,
				download_command,
				user_callbacks->callbacks_config->cfg_queue[0].start_chapter,
				user_callbacks->callbacks_config->cfg_queue[0].end_chapter
		);
		/* remove the first value from the queue and the combo box */
		if (user_callbacks->callbacks_config->cfg_queue.size()!=0) {
			user_callbacks->callbacks_config->cfg_queue.erase( 
					user_callbacks->callbacks_config->cfg_queue.begin() );
		}
		gtk_combo_box_remove_text( (GtkComboBox*)user_callbacks->callbacks_queue_combo, 0);

		/* start the download */
		user_callbacks->callbacks_downloader->run();
	}
	/* reinit the stuff */
	gtk_button_set_label( (GtkButton*)user_callbacks->callbacks_image_buton, " Download ");
	gtk_widget_show(user_callbacks->callbacks_image_buton);
	gtk_entry_set_text( (GtkEntry*)user_callbacks->callbacks_q_name_entry, " ");
	gtk_spin_button_set_value( (GtkSpinButton*)user_callbacks->callbacks_q_start_entry, 1);
	gtk_spin_button_set_value( (GtkSpinButton*)user_callbacks->callbacks_q_end_entry, 1);
	gtk_progress_bar_set_fraction( (GtkProgressBar*)user_callbacks->callbacks_pbar1, 0.001);
	gtk_progress_bar_set_fraction( (GtkProgressBar*)user_callbacks->callbacks_pbar2, 0.001);
	gtk_label_set_text( (GtkLabel*) user_callbacks->callbacks_stat_label, " " );
	*user_callbacks->callbacks_downloader_state  = 0;
	gtk_widget_show_all (user_callbacks->callbacks_win);

	/* here, the timeout thing that updates the progress bars should have stopped */
	return NULL;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* update the bars during the download */
gint
gui::update_bars( gpointer user_data )
{
	callbacks* user_callbacks = (callbacks*) user_data;

	/* stop updating when the queue is empty */
	//if ( (*w->Q).size() == 0 || (*w->DLDER).finished() ||
	if ( *user_callbacks->callbacks_downloader_state == 0 ) { 
		return false;
	}
	else {
		mg_status stat =  user_callbacks->callbacks_downloader->get_status();
		/*
		* nb of chapters downloading = stat.end_chapter-stat.start_chapter+1;
		* nb of already dlded        = stat.cur_chapter - stat.start_chapter+1;
		*/ 
		if (stat.end_chapter == 0 )  {
			return true;
		}
		else {
			gdouble chapter_progress = 
				(gdouble)(stat.cur_chapter-stat.start_chapter+1)
				/
				(gdouble)(stat.end_chapter-stat.start_chapter+1);
			gdouble page_progress = 
				(gdouble)(stat.cur_page)/
				(gdouble)(stat.end_page);
			if ( chapter_progress < 0 || chapter_progress > 1.0 ) {
				return true;
			}
			else if ( page_progress < 0 || page_progress > 1.0 ) {
				return true;
			}
			else {

				gtk_progress_bar_set_fraction( (GtkProgressBar*) user_callbacks->callbacks_pbar1, chapter_progress);
				gtk_progress_bar_set_fraction( (GtkProgressBar*) user_callbacks->callbacks_pbar2, page_progress);
			}
		}
		gtk_widget_show_all (user_callbacks->callbacks_win);
	}
	return true;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* call back used when the download button is pressed */
void 
gui::stop_callback(GtkWidget *wid, gpointer user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;
	
	/* if we are not in the "nothing state" */
	if(*user_callbacks->callbacks_downloader_state!=0) {
		GtkWidget *dialog1 = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
			GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Stoping download");
		gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
		gtk_dialog_run (GTK_DIALOG (dialog1));
		gtk_widget_destroy (dialog1);
		/* reset stuffs */
		gtk_entry_set_text( (GtkEntry*)user_callbacks->callbacks_q_name_entry, "  ");
		gtk_spin_button_set_value( (GtkSpinButton*)user_callbacks->callbacks_q_start_entry, 1);
		gtk_spin_button_set_value( (GtkSpinButton*)user_callbacks->callbacks_q_end_entry, 1);
		gtk_progress_bar_set_fraction( (GtkProgressBar*)user_callbacks->callbacks_pbar1, 0.001);
		gtk_progress_bar_set_fraction( (GtkProgressBar*)user_callbacks->callbacks_pbar2, 0.001);
		gtk_button_set_label( (GtkButton*)user_callbacks->callbacks_image_buton, "Download");
		gtk_label_set_text( (GtkLabel*) user_callbacks->callbacks_stat_label, "  " );
		*user_callbacks->callbacks_downloader_state = 0;
		user_callbacks->callbacks_downloader->stop();
		/* set the current provider to the one that is selected atm */
		user_callbacks->callbacks_config->cfg_current_provider = 
			gtk_combo_box_get_active_text( (GtkComboBox*) user_callbacks->callbacks_provider_combo);
	}
	else {
		//already not downloading anything
	}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* pop up to select the location where the manga will be drop into */
void 
gui::location_callback(GtkWidget *wid, gpointer user_data)
{
	std::string dirpath = "";
	callbacks* user_callbacks = (callbacks*) user_data;

	GtkWidget *select_dir = gtk_file_chooser_dialog_new("Select a Dir", NULL,
		GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL );

	if (gtk_dialog_run(GTK_DIALOG (select_dir) ) == GTK_RESPONSE_ACCEPT)
		dirpath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (select_dir) );
	gtk_widget_destroy(select_dir);

	user_callbacks->callbacks_config->cfg_current_location = dirpath;
	dirpath = "Download Location "+dirpath;
	gtk_label_set_text( (GtkLabel*) user_callbacks->callbacks_location_text, dirpath.c_str() );
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* choose the download manager */
void 
gui::dl_manager_callback(GtkWidget *wid, gpointer user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;

	int position_to_choose  = gtk_combo_box_get_active ( (GtkComboBox*) user_callbacks->callbacks_dl_manager_combo );
	if (position_to_choose == -1) {
		wpopup("Cannot select the command");
		return;
	}

	std::string selected_dl   = gtk_combo_box_get_active_text ( (GtkComboBox*)user_callbacks->callbacks_dl_manager_combo );
	for (unsigned int i=0; i< user_callbacks->callbacks_config->cfg_download_managers.size() ; i++)
		if ( user_callbacks->callbacks_config->cfg_download_managers[i].name==selected_dl){
			user_callbacks->callbacks_config->cfg_download_managers[i].selected = true;
#ifdef DEBUG
			//DEBUG
			std::cout<< user_callbacks->callbacks_config->cfg_download_managers[i].name<<" Selected "<<"\n";
#endif
		}
		else {
			user_callbacks->callbacks_config->cfg_download_managers[i].selected = false;
#ifdef DEBUG
			//DEBUG
			std::cout<< user_callbacks->callbacks_config->cfg_download_managers[i].name<<"\n";
#endif
		}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Add a Download manager */
void 
gui::add_dlmngr_callback(GtkWidget *wid, gpointer user_data) 
{
	callbacks* user_callbacks = (callbacks*) user_data;

	std::string dl_name = gtk_entry_get_text( (GtkEntry*) user_callbacks->callbacks_name_dl );
	std::string command = gtk_entry_get_text( (GtkEntry*) user_callbacks->callbacks_command_dl );
	bool problem = false;
	for (unsigned int i=0; i< user_callbacks->callbacks_config->cfg_download_managers.size() ; i++)
		if ( user_callbacks->callbacks_config->cfg_download_managers[i].name==dl_name) {
			problem = true;
		}
		else if ( user_callbacks->callbacks_config->cfg_download_managers[i].command== command ) {
			problem = true;
		}
		else {
			//something weird happened
		}
	if (problem || dl_name=="Name" || command =="Command"|| dl_name==""||command =="") {
		wpopup("Cannot add the command");
		return;
	}
	else {
		/* Add to the combo box */
		gtk_combo_box_append_text( (GtkComboBox*) user_callbacks->callbacks_dl_manager_combo , dl_name.c_str());
		/* Clear the Text boxes */
		gtk_entry_set_text( (GtkEntry*) user_callbacks->callbacks_name_dl, "");
		gtk_entry_set_text( (GtkEntry*) user_callbacks->callbacks_command_dl, "");
		/* Add to the list */
		dl_mngr new_dl_mngr;
		new_dl_mngr.name     = dl_name;
		new_dl_mngr.command  = command;
		new_dl_mngr.selected = false;
		user_callbacks->callbacks_config->cfg_download_managers.push_back(new_dl_mngr);
	}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Remove a Download manager */
void 
gui::del_dlmngr_callback(GtkWidget *wid, gpointer user_data) 
{
	callbacks* user_callbacks = (callbacks*) user_data;
	int position_to_delete  = gtk_combo_box_get_active ( (GtkComboBox*) user_callbacks->callbacks_dl_manager_combo );

	if (position_to_delete == -1) {
		wpopup("Cannot delete the command");
		return;
	}

	std::string selected_dl = gtk_combo_box_get_active_text ( (GtkComboBox*)user_callbacks->callbacks_dl_manager_combo );
	/* remove from list */
	for (unsigned int i=0; i< user_callbacks->callbacks_config->cfg_download_managers.size() ; i++) {
		if ( user_callbacks->callbacks_config->cfg_download_managers[i].name==selected_dl) {
			user_callbacks->callbacks_config->cfg_download_managers.erase( user_callbacks->callbacks_config->cfg_download_managers.begin()+i );
		}
		else {
		}
	}
	/* Remove from the combo box */
	gtk_combo_box_remove_text ( (GtkComboBox*) user_callbacks->callbacks_dl_manager_combo, position_to_delete);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Save the configs in the database */
void 
gui::save_cfg_callback(GtkWidget *wid, gpointer user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;
	user_callbacks->callbacks_config->update_mg_location( user_callbacks->callbacks_config->cfg_current_location );
	//DEBUG
#ifdef DEBUG
	std::cout<< user_callbacks->callbacks_config->cfg_current_location<<"\n";
#endif
	user_callbacks->callbacks_config->db_update_dl_manager( user_callbacks->callbacks_config->cfg_download_managers );

	GtkWidget *dialog1 = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
		GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Database has been updated");
	gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dialog1));
	gtk_widget_destroy (dialog1);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Revert the config */
void
gui::rever_callback(GtkWidget *wid, gpointer user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;
	/* revert the location */
	std::string mg_loc = user_callbacks->callbacks_config->get_mg_location();
	user_callbacks->callbacks_config->cfg_current_location = mg_loc;
	mg_loc = "Download Location "+mg_loc;
	gtk_label_set_text( (GtkLabel*) user_callbacks->callbacks_location_text, mg_loc.c_str());
	/* revert the dl managers */
	for (unsigned int i=0; i<user_callbacks->callbacks_config->cfg_download_managers.size(); i++) {
		gtk_combo_box_remove_text( (GtkComboBox*) user_callbacks->callbacks_dl_manager_combo, 0);
	}
	user_callbacks->callbacks_config->cfg_download_managers.clear();
	user_callbacks->callbacks_config->cfg_download_managers = user_callbacks->callbacks_config->db_get_dl_managers();
	for(unsigned int i=0;i< user_callbacks->callbacks_config->cfg_download_managers.size();i++) {
		gtk_combo_box_append_text( (GtkComboBox*) user_callbacks->callbacks_dl_manager_combo , 
			user_callbacks->callbacks_config->cfg_download_managers[i].name.c_str());
		if ( user_callbacks->callbacks_config->cfg_download_managers[i].selected ) {
			gtk_combo_box_set_active( (GtkComboBox*)  user_callbacks->callbacks_dl_manager_combo, i );
		}
		else {
		}
	}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Add the manga selected to the queue */
void 
gui::add_to_q_callback(GtkWidget *wid, gpointer user_data)
{
	callbacks* user_callbacks = (callbacks*) user_data;

	/* check if the start chapter is less than the end chapter */
	int start_int = gtk_spin_button_get_value_as_int( (GtkSpinButton*) user_callbacks->callbacks_wheel );
	int end_int   = gtk_spin_button_get_value_as_int( (GtkSpinButton*) user_callbacks->callbacks_wheel2 );

	if (start_int > end_int) {
		wpopup("can't download in reverse order");
		return;
	}
	else {
		//ok so it's not in reverse
	}

	/* get the manga name */
	std::string mgname_to_add = gtk_entry_get_text( (GtkEntry*) user_callbacks->callbacks_texta );
	if (mgname_to_add == "Manga Name" || mgname_to_add == "") {
		wpopup("You didn't specify a correct manga name");
		return;
	}
	else {
		//so the user changed the default value... but does the manga really exist,
		//are we getting garbage, who knows?
		//TODO: do some verification
	}

	replaceAll(mgname_to_add, " ","");

	std::string to_add_to_combo = 
		mgname_to_add+" | "+to_string(start_int)+" -> "+to_string(end_int);
	/* Add to the combo box */
	gtk_combo_box_append_text( (GtkComboBox*) user_callbacks->callbacks_queue_combo , 
			to_add_to_combo.c_str());

	manga_queue_data new_to_queue;
	new_to_queue.manga_name             = mgname_to_add;
	new_to_queue.start_chapter          = to_string(start_int);
	new_to_queue.end_chapter            = to_string(end_int);
	user_callbacks->callbacks_config->cfg_queue.push_back(new_to_queue);

	for(unsigned int i=0; i< user_callbacks->callbacks_config->cfg_queue.size() ; i++) {
		//DEBUG
#ifdef DEBUG
		std::cout<< user_callbacks->callbacks_config->cfg_queue[i].manga_name <<"\n";
#endif
	}

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* change the widget according to the selected manga in the queue */
void 
gui::select_q_callback(GtkWidget *wid, gpointer user_data )
{
	callbacks* user_callbacks = (callbacks*) user_data;
	/* starting from 0 */
	int active = gtk_combo_box_get_active( (GtkComboBox*) user_callbacks->callbacks_queue_combo);
	if (active==-1) {
		return;
	}
	else {
		//we can still remove from the queue because it's not empty
	}
	std::string active_name  = user_callbacks->callbacks_config->cfg_queue[active].manga_name;
	std::string active_start =  user_callbacks->callbacks_config->cfg_queue[active].start_chapter;
	std::string active_end   =  user_callbacks->callbacks_config->cfg_queue[active].end_chapter;

	gtk_entry_set_text( (GtkEntry*) user_callbacks->callbacks_q_name_entry, active_name.c_str() );
	gtk_spin_button_set_value( (GtkSpinButton*) user_callbacks->callbacks_q_start_entry, 
			atoi(active_start.c_str()));
	gtk_spin_button_set_value( (GtkSpinButton*) user_callbacks->callbacks_q_end_entry, atoi(active_end.c_str()));

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* change the queue */
void 
gui::change_q_callback(GtkWidget *wid, gpointer user_data )
{
	callbacks* user_callbacks = (callbacks*) user_data;
	/* starting from 0 */
	int active = gtk_combo_box_get_active( (GtkComboBox*) user_callbacks->callbacks_queue_combo);
	if (active ==-1) {
		return;
	}
	else {
		//we can continue because we have at least one thing in the combo
	}
	int st = gtk_spin_button_get_value_as_int( (GtkSpinButton*) user_callbacks->callbacks_q_start_entry);
	int en = gtk_spin_button_get_value_as_int( (GtkSpinButton*) user_callbacks->callbacks_q_end_entry);

	if (st>en) {
		wpopup("Can't download in reverse order");
		return;
	}
	else {
		//at this point everything should be fine
	}

	std::string active_start = to_string(st);
	std::string active_end   = to_string(en);

	user_callbacks->callbacks_config->cfg_queue[active].start_chapter = active_start;
	user_callbacks->callbacks_config->cfg_queue[active].end_chapter   = active_end;

	//DEBUG
#ifdef DEBUG
	std::cout<<"here\n"
		<<active
		<<"\n";
#endif

	std::string to_add_to_combo = 
		user_callbacks->callbacks_config->cfg_queue[active].manga_name+" | "+active_start+" -> "+active_end;

	//DEBUG
#ifdef DEBUG
	std::cout<<to_add_to_combo <<"\n";
#endif

	/* Add to the combo box */
	gtk_combo_box_text_insert_text( (GtkComboBoxText*) user_callbacks->callbacks_queue_combo, 
			active,to_add_to_combo.c_str());
	gtk_combo_box_set_active ( (GtkComboBox*) user_callbacks->callbacks_queue_combo, active);

	gtk_combo_box_text_remove( (GtkComboBoxText*) user_callbacks->callbacks_queue_combo, active+1);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* delete from the queue */
void 
gui::delete_q_callback(GtkWidget *wid, gpointer user_data )
{
	callbacks* user_callbacks = (callbacks*) user_data;
	/* starting from 0 */
	int active = gtk_combo_box_get_active( (GtkComboBox*) user_callbacks->callbacks_queue_combo);
	if (active ==-1) {
		return;
	}
	else {
		//has at least one mg
	}


	/* clear the text boxes */
	gtk_entry_set_text( (GtkEntry*) user_callbacks->callbacks_q_name_entry, "");
	gtk_spin_button_set_value( (GtkSpinButton*) user_callbacks->callbacks_q_start_entry, 1);
	gtk_spin_button_set_value( (GtkSpinButton*) user_callbacks->callbacks_q_end_entry, 1);
	/* remove from the combo box */
	//DEBUG
#ifdef DEBUG
	std::cout<<active<<"\n";
#endif
	gtk_combo_box_remove_text ( (GtkComboBox*) user_callbacks->callbacks_queue_combo, active);
	/* remove from the queue */
	user_callbacks->callbacks_config->cfg_queue.erase( 
			user_callbacks->callbacks_config->cfg_queue.begin()+active );
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* fill the file menu */
void
gui::file_menu_fill(GtkWidget *menubar)
{
	GtkWidget *file_menu = gtk_menu_item_new_with_mnemonic ("_File");
	GtkWidget *item_container = gtk_menu_new ();


	GtkWidget *hide_item = gtk_menu_item_new_with_mnemonic ("_Hide");
//	g_signal_connect (hide_item, "activate", G_CALLBACK(about_this), (gpointer) win);
	gtk_menu_append (GTK_MENU (item_container), hide_item);
	gtk_widget_add_accelerator (hide_item, "activate", accel_group, 
		GDK_i, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	GtkWidget *quit_item = gtk_menu_item_new_with_mnemonic ("_Quit");
	g_signal_connect (quit_item, "activate", G_CALLBACK(delete_event), (gpointer) win);
	gtk_menu_append (GTK_MENU (item_container), quit_item);
	gtk_widget_add_accelerator (quit_item, "activate", accel_group, 
		GDK_q, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (file_menu), item_container);
	gtk_menu_bar_append (GTK_MENU_BAR (menubar), file_menu);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* fill the config menu */
void
gui::config_menu_fill(GtkWidget *menubar)
{
	/* location */
	GtkWidget *config_menu = gtk_menu_item_new_with_mnemonic ("_Configs");
	GtkWidget *item_container = gtk_menu_new ();

	GtkWidget *location_item = gtk_menu_item_new_with_mnemonic ("Mg _Location");
	g_signal_connect (location_item, "activate", G_CALLBACK(location_callback), &callbacks_data);

	gtk_menu_append (GTK_MENU (item_container), location_item);

	gtk_widget_add_accelerator (location_item, "activate", accel_group, 
			GDK_l, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	GtkWidget *save_item = gtk_menu_item_new_with_mnemonic ("_Save Configs");
	g_signal_connect (save_item, "activate", G_CALLBACK (save_cfg_callback), &callbacks_data);
	gtk_menu_append (GTK_MENU (item_container), save_item);
	gtk_widget_add_accelerator (save_item, "activate", accel_group, 
		GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


	
	gtk_menu_append (GTK_MENU (item_container), revert_item);
	gtk_widget_add_accelerator (revert_item, "activate", accel_group, 
		GDK_r, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (config_menu), item_container);
	gtk_menu_bar_append (GTK_MENU_BAR (menubar), config_menu);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* fill the updater menu */
void
gui::updater_menu_fill(GtkWidget *menubar)
{
	GtkWidget *updater_menu = gtk_menu_item_new_with_mnemonic ("_Updater");
	GtkWidget *item_container = gtk_menu_new ();

//	GtkWidget *location_item = gtk_menu_item_new_with_mnemonic ("Mg _Location");
//	g_signal_connect (location_item, "activate", G_CALLBACK(about_this), (gpointer) win);
//	gtk_menu_append (GTK_MENU (item_container), location_item);
//	gtk_widget_add_accelerator (location_item, "activate", accel_group, 
//			GDK_l, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (updater_menu), item_container);
	gtk_menu_bar_append (GTK_MENU_BAR (menubar), updater_menu);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* fill the help menu */
void
gui::help_menu_fill(GtkWidget *menubar)
{
	GtkWidget *help_menu = gtk_menu_item_new_with_mnemonic ("_Help");
	GtkWidget *item_container = gtk_menu_new ();

	GtkWidget *howto_item = gtk_menu_item_new_with_mnemonic ("_Howto");
//	g_signal_connect (howto_item, "activate", G_CALLBACK(about_this), (gpointer) win);
	gtk_menu_append (GTK_MENU (item_container), howto_item);
	gtk_widget_add_accelerator (howto_item, "activate", accel_group, 
			GDK_h, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	GtkWidget *about_item = gtk_menu_item_new_with_mnemonic ("_about");
	g_signal_connect (about_item, "activate", G_CALLBACK(about_this), (gpointer) win);
	gtk_menu_append (GTK_MENU (item_container), about_item);
	gtk_widget_add_accelerator (about_item, "activate", accel_group, 
			GDK_a, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (help_menu), item_container);
	gtk_menu_bar_append (GTK_MENU_BAR (menubar), help_menu);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the top hpack */
void
gui::hpack_1()
{
	GtkWidget *menubar  = gtk_menu_bar_new();
	file_menu_fill(menubar);
	config_menu_fill(menubar);
	updater_menu_fill(menubar);
	help_menu_fill(menubar);
	gtk_box_pack_start(GTK_BOX (mainvbox), menubar, FALSE, FALSE, 0);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the Program Name label - 2nd pack - Program Status */
void
gui::hpack_2()
{
	/* create a new label. */
	label = gtk_label_new ("MangaDL");
	gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
	gtk_misc_set_padding(GTK_MISC (label),50,20);
	gtk_label_set_markup( GTK_LABEL(label),
		"<span foreground='#007ef9' size='large'><b>MangaDL</b></span>");
	gtk_box_pack_start (GTK_BOX (mainvbox), label, FALSE, FALSE, 0);

	/* create a new label. */
	gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
	gtk_misc_set_alignment (GTK_MISC (stat_label), 0.5, 0.5);
	gtk_box_pack_start (GTK_BOX (mainvbox), stat_label, FALSE, FALSE, 0);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the text box where you insert the manga name*/
void 
gui::hpack_3()
{
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, false, 0);

	gtk_entry_set_text((GtkEntry*)texta,"Manga Name");
	gtk_box_pack_start (GTK_BOX (hbox),texta , true, true, 10); //box,child,expand,fill,padding

	gtk_button_set_label( (GtkButton*)add_queue_buton, "Add to Queue");
	gtk_box_pack_start (GTK_BOX (hbox), add_queue_buton, false, false, 10);
	
	/* The callback is in hpack3_1() because the wheel doesn't exist before that */

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* The wheels */
void
gui::hpack_4()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, true, 5);

	label = gtk_label_new ("From");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
	//spin
	gtk_box_pack_start(GTK_BOX (hbox), wheel, true, true, 10);

	label = gtk_label_new ("to");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
	//spin
	gtk_box_pack_start(GTK_BOX (hbox), wheel2, true, true, 10);

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* progress bar - chapter */
void
gui::hpack_5()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, false, 10);
	
	gtk_box_pack_start( GTK_BOX(hbox), pbar1, true, true, 10);

	label = gtk_label_new ("Chapters");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* progress bar - page */
void
gui::hpack_6()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, false, 10);
	
	gtk_box_pack_start( GTK_BOX(hbox), pbar2, true, true, 10);

	label = gtk_label_new ("Pages   ");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Buttons at the bottom */
void 
gui::hpack_7()
{
	///hbox
	hbox = gtk_hbox_new (TRUE,4);
	//gtk_container_add (GTK_CONTAINER (vbox), hbox);
	gtk_box_pack_end(GTK_BOX (vbox), hbox,TRUE, TRUE, 0);
	/* the callback is at the end of the packs */
	
	/* This calls our box creating function , the box is inserted inside the button*/
	gtk_button_set_label((GtkButton*)image_buton, "Download");
	//	imbox = xpm_label_box ((gchar *)"data/button.png",(gchar *)"");
	//	gtk_container_add (GTK_CONTAINER (image_buton), imbox);
	//	gtk_container_border_width (GTK_CONTAINER(image_buton), 0);
	gtk_box_pack_start (GTK_BOX (hbox), image_buton, true, true, 10);
	gtk_box_pack_start (GTK_BOX (hbox), stop_button, true,true, 10);

	gtk_widget_realize (win);
	gtk_widget_show_all (win);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the text box with the location */
void 
gui::hpack2_1()
{
	hbox = gtk_hbox_new (false,20);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 20);

	std::string label_text = "Download Location "+gui_config->cfg_current_location;
	gtk_box_pack_start (GTK_BOX (hbox),location_text, true, true, 10); //box,child,expand,fill,padding

	GtkWidget *location_buton = gtk_button_new();
	/* Connect the "clicked" signal of the button to our callback */
	g_signal_connect (location_buton, "released", G_CALLBACK (location_callback), &callbacks_data);
	gtk_button_set_label( (GtkButton*)location_buton, "Choose");
	gtk_box_pack_start (GTK_BOX (hbox), location_buton, false, false, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw a combo box with all the dl_managers name + 2 buttons select and delete */
void 
gui::hpack2_2()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 10);
	GtkWidget *dl_label = gtk_label_new("Download Manager ");
	gtk_box_pack_start (GTK_BOX (hbox), dl_label, false, false, 10); //box,child,expand,fill,padding
	
	for(unsigned int i=0;i<gui_config->cfg_download_managers.size();i++) {
		gtk_combo_box_append_text( (GtkComboBox*) dl_manager_combo , 
			gui_config->cfg_download_managers[i].name.c_str());
		if ( gui_config->cfg_download_managers[i].selected )
			gtk_combo_box_set_active( (GtkComboBox*)  dl_manager_combo, i );
	}

	gtk_box_pack_start (GTK_BOX (hbox), dl_manager_combo, true, true, 3); //box,child,expand,fill,padding

	GtkWidget *select_button = gtk_button_new();
	GtkWidget *delete_button = gtk_button_new();

	gtk_button_set_label( (GtkButton*)select_button, "Select");
	gtk_button_set_label( (GtkButton*)delete_button, "Delete");

	gtk_box_pack_start (GTK_BOX (hbox), select_button, false, false, 10);
	gtk_box_pack_start (GTK_BOX (hbox), delete_button, false, false, 10);

	g_signal_connect (select_button, "released", 
		G_CALLBACK (dl_manager_callback), &callbacks_data);
	g_signal_connect (delete_button, "released", 
		G_CALLBACK (del_dlmngr_callback), &callbacks_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw 2 text box, name of dl, dl command + a button to add */
void 
gui::hpack2_3()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 10);

	GtkWidget *label_add = gtk_label_new("Add a DL Manager");
	gtk_box_pack_start (GTK_BOX (hbox), label_add, false, false, 10); //box,child,expand,fill,padding

	gtk_entry_set_text( (GtkEntry*) name_dl, "Name" );
	gtk_entry_set_text( (GtkEntry*) command_dl, "Command" );

	GtkWidget *add_dl_button = gtk_button_new();
	gtk_button_set_label( (GtkButton*)add_dl_button, "Add   ");

	gtk_box_pack_start (GTK_BOX (hbox), name_dl,    false, false, 2);
	gtk_box_pack_start (GTK_BOX (hbox), command_dl, true, true, 2);
	gtk_box_pack_start (GTK_BOX (hbox), add_dl_button, false, false, 10);

	g_signal_connect (add_dl_button, "released", 
		G_CALLBACK (add_dlmngr_callback), &callbacks_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* List of Providers -- Mangareader (default) and Mangapark */
void 
gui::hpack2_4()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 10);
	GtkWidget *label_provider = gtk_label_new("Provider        ");
	gtk_box_pack_start (GTK_BOX (hbox), label_provider, false, false, 10); //box,child,expand,fill,padding


	std::vector<std::string> all_available_providers = factory.show_providers();
	for ( auto i: all_available_providers) {
		gtk_combo_box_append_text( (GtkComboBox*) provider_combo , i.c_str());
	}
	//set the first one as default
	//TODO : set the default in the config instead
	gtk_combo_box_set_active( (GtkComboBox*) provider_combo, 0);

	gtk_box_pack_start (GTK_BOX (hbox), provider_combo, true, true, 10); //box,child,expand,fill,padding
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the Save configs button */
void
gui::hpack2_5()
{
	hbox = gtk_hbox_new (true,3);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, true, true, 0);
	GtkWidget *save_button = gtk_button_new();
	gtk_button_set_label( (GtkButton*)save_button, "Save Configs");
	gtk_box_pack_start (GTK_BOX (hbox), save_button, true, true, 0);

	g_signal_connect (save_button, "released", 
		G_CALLBACK (save_cfg_callback), &callbacks_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Queue part */
void
gui::hpack3_1()
{
	hbox = gtk_hbox_new (false,0);
	gtk_box_pack_start(GTK_BOX (vbox3), hbox, false, false, 10);
	GtkWidget *queue_label = gtk_label_new("Queue ");
	gtk_box_pack_start (GTK_BOX  (hbox), queue_label, false, false, 10);
	gtk_box_pack_start (GTK_BOX (hbox), queue_combo, true, true, 10);



	/* callback from hpack_3() -- addtoqueue */
	g_signal_connect (add_queue_buton, "released", 
		G_CALLBACK (add_to_q_callback), &callbacks_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Queue part */
void
gui::hpack3_2()
{
	hbox = gtk_hbox_new (false,0);
	gtk_box_pack_start(GTK_BOX (vbox3), hbox, false, false, 10);
	GtkWidget *change_q_name_label = gtk_label_new("Manga ");
	gtk_entry_set_text( (GtkEntry*) q_name_entry, "manga name");
	gtk_entry_set_editable( (GtkEntry*) q_name_entry, false);
	GtkWidget *change_q_start_label = gtk_label_new("Start ");
	GtkWidget *change_q_end_label = gtk_label_new("End ");

	gtk_box_pack_start (GTK_BOX (hbox), change_q_name_label, false, false, 10);
	gtk_box_pack_start ( GTK_BOX(hbox), q_name_entry, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), change_q_start_label, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), q_start_entry, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), change_q_end_label, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), q_end_entry, true, true, 10);

//	g_signal_connect (select_button, "released", G_CALLBACK (dl_manager_callback), &dl_mngr_callback_data);
//

	g_signal_connect (queue_combo, "changed", 
		G_CALLBACK (select_q_callback), &callbacks_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Change queue */
void
gui::hpack3_3()
{
	hbox = gtk_hbox_new (false,0);
	gtk_box_pack_start(GTK_BOX (vbox3), hbox, false, false, 10);
	GtkWidget *delete_q = gtk_button_new();
	gtk_button_set_label( (GtkButton*) delete_q, "Delete");
	GtkWidget *change_q = gtk_button_new();
	gtk_button_set_label( (GtkButton*) change_q, "Change");

	gtk_box_pack_start (GTK_BOX (hbox), change_q, true, true, 10);
	gtk_box_pack_start ( GTK_BOX(hbox), delete_q, true, true, 10);


	g_signal_connect( change_q, "released", 
		G_CALLBACK (change_q_callback), &callbacks_data);
	g_signal_connect (delete_q, "released", 
		G_CALLBACK (delete_q_callback), &callbacks_data);

	g_signal_connect (revert_item, "activate", G_CALLBACK(rever_callback), &callbacks_data);
	g_signal_connect (image_buton, "released", G_CALLBACK (download_callback), &callbacks_data);
	g_signal_connect (stop_button, "released", G_CALLBACK (stop_callback), &callbacks_data);


}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the main window of the program */
void
gui::draw_main_win()
{
	/* Start */
	/* When the window is given the "delete-event" signal (this is given
	* by the window manager, usually by the "close" option, or on the
	* titlebar), we ask it to call the delete_event () function
	* as defined above. The data passed to the callback
	* function is NULL and is ignored in the callback function. */
	g_signal_connect (win, "delete-event", G_CALLBACK (delete_event), NULL);
	/* when receiving the destroy signal quit the app*/
	g_signal_connect (win, "destroy", G_CALLBACK (delete_event), NULL);
	/* set default stuffs inside the window */
	gtk_container_set_border_width (GTK_CONTAINER (win), 0);
//	gtk_window_set_default_size( GTK_WINDOW(win), 900,500);
	gtk_widget_set_size_request ( (GtkWidget *)win, 600,380);
	gtk_window_set_resizable ( GTK_WINDOW (win), true);
//	gtk_window_set_policy (GTK_WINDOW(win), FALSE, FALSE, FALSE);
	gtk_window_set_title (GTK_WINDOW (win), "Manga Downloader");
	gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
	gtk_widget_realize (win);
	/* Attach the new accelerator group to the window. */
	gtk_window_add_accel_group (GTK_WINDOW (win), accel_group);
	/* Start */
	gtk_widget_show_all (win);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* put stuff inside the notebook */
void
gui::pack_in_notebook()
{
	GtkWidget *notebook        = gtk_notebook_new ();
	GtkWidget *tab_labl_main   = gtk_label_new("Main");
	GtkWidget *tab_labl_config = gtk_label_new("Configs");
	GtkWidget *tab_labl_queue  = gtk_label_new("Queue");

	gtk_notebook_append_page ( (GtkNotebook*)notebook, vbox, tab_labl_main);
	gtk_notebook_append_page ( (GtkNotebook*)notebook, vbox2, tab_labl_config);
	gtk_notebook_append_page ( (GtkNotebook*)notebook, vbox3, tab_labl_queue);

	gtk_container_add( GTK_CONTAINER (mainvbox), notebook);
	gtk_container_add( GTK_CONTAINER (win), mainvbox);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Public function called to Display the whole GUI and start the procedures */
void
gui::Display()
{
	/* draw main window */
	draw_main_win();
	/* insert it into the win */
	/* top menu */
	hpack_1();
	/* program label */
	hpack_2();
	/* manga name/list */
	hpack_3();
	/* the wheels */
	hpack_4();
	/* chapter progress bar */
	hpack_5();
	/* page progress bar */
	hpack_6();
	///Separator
	GtkWidget *separ = gtk_hseparator_new ();
	gtk_box_pack_start (GTK_BOX (vbox), separ, FALSE, FALSE, 5);
	/* buttons */
	hpack_7();
	/* second tab start */
	/* text box with the location name */
	hpack2_1();
	/* combo box + buttons */
	hpack2_2();
	/* add command */
	hpack2_3();
	/* providers */
	hpack2_4();
	/* Save Configs */
	hpack2_5();
	/* third tab start */
	/* combo queue */
	hpack3_1();
	/* change/delete */
	hpack3_2();
	hpack3_3();

	pack_in_notebook();
	/* Enter the main loop */
	gtk_widget_show_all (win);
	gtk_main ();
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

