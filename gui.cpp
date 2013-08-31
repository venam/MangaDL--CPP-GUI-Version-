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

 * Compile with:
 * clang++ gui.cpp `pkg-config gtk+-2.0 --cflags` `pkg-config gtk+-2.0 --libs` -lsqlite3 -lcurl -o TEST
 *
 * TODO:
 *  Clear queue button
 *  How to
 *  language for the external dl manager
 *  check if the download manager command works fine
 *  check for 404 errors
 *  Add more providers
 *  Add the Updater to it
 *  Write a Makefile
 *
 */ 
#include <stdlib.h>
#include <iostream>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms-compat.h>
#include <string>
#include "manager/config.hpp"
#include "manager/mgreader.hpp"
#include "manager/mgpark.hpp"
#include <pthread.h>

struct manga_queue_data {
	std::string manga_name;
	std::string start_chapter;
	std::string end_chapter;
};

typedef struct callback_data {
	GtkWidget     *D1;
	GtkWidget     *D2;
	GtkWidget     *D3;
	GtkWidget     *D4;
	GtkWidget     *D5;
	GtkWidget     *D6;
	GtkWidget     *D7;
	GtkWidget     *D8;
	std::string   *S1;
	std::vector   <dl_mngr>*DL;
	std::vector   <manga_queue_data>*Q;  /*the queue*/
	mgreader      *DLDER_reader;
	mgpark        *DLDER_park;
	GtkWidget     *D9;
	int           *CUR_PROV;
	int           *ISDL;
} callback_items;

typedef struct slct_dl_manager {
	GtkWidget   *D1;
	GtkWidget   *D2;
	GtkWidget   *D3;
	std::vector <dl_mngr>*DL;
	db_manager  DB;
	std::string *S1;
} select_dl_manager;

typedef struct q_data {
	GtkWidget        *D1; /*mgname*/
	GtkWidget        *D2; /*wheel1*/
	GtkWidget        *D3; /*wheel2*/
	GtkWidget        *D4; /*combo*/
	std::vector <manga_queue_data>*Q;  /*the queue*/
} callback_q_data;

/* GUI Class handling all the GUI and Callbacks */
class Manga_GUI {
	public:
		Manga_GUI();
		~Manga_GUI();
		void Display(int argc, char *argv[]);

	protected:
	private:
		GtkWidget         *win;
		GtkWidget         *vbox;
		GtkWidget         *vbox2;
		GtkWidget         *vbox3;
		GtkWidget         *mainvbox;
		GtkWidget         *hbox;
		GtkWidget         *stop_button;
		GtkWidget         *label;
		GtkWidget         *stat_label;
		GtkWidget         *image_buton;
		GtkWidget         *add_queue_buton;
		GtkWidget         *imbox;
		GtkWidget         *wheel;
		GtkWidget         *wheel2;
		GtkWidget         *texta;
		GtkWidget         *location_text;
		GtkWidget         *dl_manager_combo;
		GtkWidget         *name_dl;
		GtkWidget         *command_dl;
		GtkWidget         *queue_combo;
		GtkWidget         *q_name_entry;
		GtkWidget         *q_start_entry;
		GtkWidget         *q_end_entry;
		GtkWidget         *pbar1;
		GtkWidget         *pbar2;
		GtkWidget         *provider_combo;
		GtkAccelGroup     *accel_group;
		callback_items    wstruct;
		callback_items    wstruct_loc;
		db_manager        db;
		std::string       current_location;
		select_dl_manager dl_mngr_callback_data;
		std::vector       <dl_mngr>download_managers;
		std::vector       <manga_queue_data>queue;
		callback_q_data   q_c_data;
		callback_q_data   q_combo_data;
		mgreader          downloader_reader;
		mgpark            downloader_park;

		int               downloader_state;
		int               current_provider;

		void draw_main_win              ( void                                              ) ;
		void hpack_1                    ( void                                              ) ;
		void hpack_2                    ( void                                              ) ;
		void hpack_3                    ( void                                              ) ;
		void hpack_4                    ( void                                              ) ;
		void hpack_5                    ( void                                              ) ;
		void hpack_6                    ( void                                              ) ;
		void hpack_7                    ( void                                              ) ;
		void hpack2_1                   ( void                                              ) ;
		void hpack2_2                   ( void                                              ) ;
		void hpack2_3                   ( void                                              ) ;
		void hpack2_4                   ( void                                              ) ;
		void hpack2_5                   ( void                                              ) ;
		void hpack3_1                   ( void                                              ) ;
		void hpack3_2                   ( void                                              ) ;
		void hpack3_3                   ( void                                              ) ;
		void pack_in_notebook           ( void                                              ) ;
		void   file_menu_fill           ( GtkWidget *menubar                                ) ;
		void   config_menu_fill         ( GtkWidget *menubar                                ) ;
		void   updater_menu_fill        ( GtkWidget *menubar                                ) ;
		void   help_menu_fill           ( GtkWidget *menubar                                ) ;
		static void wpopup              ( std::string message                               ) ;
		static void about_this          ( GtkWidget *wid, GtkWidget *win                    ) ;
		static gboolean delete_event    ( GtkWidget *widget, GdkEvent *event, gpointer data ) ;
		static GtkWidget *xpm_label_box ( gchar *xpm_filename, gchar *label_text            ) ;
		static void download_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void stop_callback       ( GtkWidget *wid, gpointer user_data                ) ;
		static void location_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void dl_manager_callback ( GtkWidget *wid, gpointer user_data                ) ;
		static void add_dlmngr_callback ( GtkWidget *wid, gpointer user_data                ) ;
		static void del_dlmngr_callback ( GtkWidget *wid, gpointer user_data                ) ;
		static void save_cfg_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void add_to_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void select_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;   
		static void change_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void delete_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void *start_download     ( void *user_data                                   ) ;
		static gint update_bars         ( gpointer user_data                                ) ;
};
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* MENU  -- This doesn't work as expected
GtkItemFactoryEntry Manga_GUI::menu_items[] = {
	{ (gchar *)"/_File"                  , NULL                  , NULL          , 0 , (gchar *)"<Branch>" }     ,
	{ (gchar *)"/File/_Save Configs"     , (gchar *)"<control>S" , print_hello   , 2 , NULL }                    ,
	{ (gchar *)"/File/_Revert Configs"   , (gchar *)"<control>R" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/File/_Hide in sys-tray" , (gchar *)"<control>I" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/File/sep1"              , (gchar *)NULL         , NULL          , 0 , (gchar *)"<Separator>" }  ,
	{ (gchar *)"/File/_Quit"             , (gchar *)"<control>Q" , gtk_main_quit , 0 , NULL }                    ,
	{ (gchar *)"/_Configs"               , NULL                  , NULL          , 0 , (gchar *)"<Branch>" }     ,
	{ (gchar *)"/Configs/Mg _Location"   , (gchar *)"<control>L" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/Configs/_DL Manager"    , (gchar *)"<control>D" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/Configs/_Provider"      , (gchar *)"<control>P" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/Configs/Mg _List"       , (gchar *)"<control>M" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/Configs/_Save Configs"  , NULL                  , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/_Updater"               , (gchar *)"<control>U" , NULL          , 0 , (gchar *)"<Branch>" }     ,
	{ (gchar *)"/_Help"                  , NULL                  , NULL          , 0 , (gchar *)"<LastBranch>" } ,
	{ (gchar *)"/Help/How to"            , (gchar *)"<control>h" , NULL          , 0 , NULL }                    ,
	{ (gchar *)"/_Help/About"            , NULL                  , about_this    , 2 , NULL }                    ,
};*/

/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */
Manga_GUI::~Manga_GUI()
{
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Initialize the GtkWidget Elements */
Manga_GUI::Manga_GUI() 
{
	stop_button       = NULL;
	win               = NULL;
	vbox              = NULL;
	vbox2             = NULL;
	hbox              = NULL;
	label             = NULL;
	image_buton       = NULL;
	add_queue_buton   = NULL;
	imbox             = NULL;
	wheel             = NULL;
	wheel2            = NULL;
	texta             = NULL;
	location_text     = NULL;
	dl_manager_combo  = NULL;
	name_dl           = NULL;
	stat_label        = NULL;
	vbox3             = NULL;
	command_dl        = NULL;
	queue_combo       = NULL;
	q_end_entry       = NULL;
	q_start_entry     = NULL;
	q_name_entry      = NULL;
	pbar1             = NULL;
	pbar2             = NULL;
	provider_combo    = NULL;
	downloader_state  = 0;
	current_provider  = 0;
	current_location  = db.get_mg_location();
	download_managers = db.get_dl_managers();
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* About Pop Up */
void 
Manga_GUI::about_this (GtkWidget *wid, GtkWidget *win)
{
	GtkWidget *dialog = NULL;
	dialog            = gtk_message_dialog_new (GTK_WINDOW (win), 
			GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, 
"MangaDL (alpha) v0.1 \n\
Coded By Venam \n\
<patrick@unixhub.net>");
	gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy(dialog);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

void 
Manga_GUI::wpopup( std::string message )
{
	GtkWidget *dialog1 = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
		GTK_MESSAGE_WARNING, GTK_BUTTONS_CLOSE, message.c_str());
	gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dialog1));
	gtk_widget_destroy (dialog1);
}

/* Delete Event to quit the GUI */
gboolean 
Manga_GUI::delete_event( GtkWidget *widget, GdkEvent *event, gpointer data )
{
	/* If you return FALSE in the "delete-event" signal handler,
	 * GTK will emit the "destroy" signal. Returning TRUE means
	 * you don't want the window to be destroyed.
	 * This is useful for popping up 'are you sure you want to quit?'
	 * type dialogs. */
	g_print ("delete event occurred\n");
	/* Change TRUE to FALSE and the main window will be destroyed with
	 * a "delete-event". */
	gtk_main_quit();
	return FALSE;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Set the image in a box */
GtkWidget *
Manga_GUI::xpm_label_box( gchar *xpm_filename, gchar *label_text )
{
	GtkWidget *box;
	GtkWidget *label;
	GtkWidget *image;
	/* Create box for image and label */
	box = gtk_hbox_new (true, 0);
	gtk_container_set_border_width (GTK_CONTAINER (box), 2);
	/* Now on to the image stuff */
	image = gtk_image_new_from_file (xpm_filename);
	/* Create a label for the button */
	label = gtk_label_new (label_text);
	/* Pack the image and label into the box */
	gtk_box_pack_start (GTK_BOX (box), image, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (box), label, FALSE, FALSE, 0);
	gtk_widget_show (image);
	//gtk_widget_show (label);
	return box;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* call back used when the download button is pressed */
void 
Manga_GUI::download_callback(GtkWidget *wid, gpointer user_data)
{
	/* contains
	 * the current location S1
	 * download managers    DL
	 * the queue            Q
	 * the queue combo box  D1
	 * the q name entry     D2
	 * the q start entry    D3
	 * the q end entry      D4
	 * The progress bar1    D5
	 * The progress bar2    D6
	 * the download button  D7
	 * the stat label       D8
	 * combo with providers D9
	 * the current provider CUR_PROV
	 * state of the dowldr  ISDL 0-nothing 1-downloading 2-paused
	 * downloader instance  DLDER_*
	 */ 
	callback_items *w = (callback_items*) user_data;

	/* nothing state */
	if( *w->ISDL == 0) {
		
		/* Check if the queue is empty */
		if ( (*w->Q).size() == 0 ) {
			wpopup("The queue is empty");
			return;
		}

		gtk_button_set_label( (GtkButton*)w->D7, "Pause");
		/* download state */
		*w->ISDL = 1;
		/* set the provider */
		*w->CUR_PROV = gtk_combo_box_get_active( (GtkComboBox*) w->D9 );

		/* start the thread that manages the download */
		pthread_t th_1;
		pthread_create(&th_1,NULL,&start_download, w);
		return;
	}
	/* download state */
	else if( *w->ISDL == 1) {
		gtk_button_set_label( (GtkButton*)w->D7, "Unpause");
		/* pause state */
		*w->ISDL = 2;
		/* pause the current class instance */
		if ( *w->CUR_PROV == 0 )
			(*w->DLDER_reader).pause_unpause();
		else if (*w->CUR_PROV == 1 )
			(*w->DLDER_park).pause_unpause();

		return;
	}
	/* pause state */
	else if ( *w->ISDL ==2) {
		gtk_button_set_label( (GtkButton*)w->D7, "Pause");
		/* download state */
		*w->ISDL = 1;

		if ( *w->CUR_PROV == 0 )
			(*w->DLDER_reader).pause_unpause();
		else if ( *w->CUR_PROV ==1 )
			(*w->DLDER_park).pause_unpause();

		return;
	}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* function called as a new thread */
void*
Manga_GUI::start_download( void *user_data)
{
	/* contains
	 * the current location S1
	 * download managers    DL
	 * the queue            Q
	 * the queue combo box  D1
	 * the q name entry     D2
	 * the q start entry    D3
	 * the q end entry      D4
	 * The progress bar1    D5
	 * The progress bar2    D6
	 * the download button  D7
	 * the stat label       D8
	 * state of the dowldr  ISDL 0-nothing 1-downloading 2-paused
	 * downloader instance  DLDER
	 */ 
	callback_items *w = (callback_items*) user_data;
	std::string download_command = "default";
	std::string download_location = "";
	for (unsigned int i=0; i< (*w->DL).size() ; i++)
		if ( (*w->DL)[i].selected) 
			download_command = (*w->DL)[i].command ;
	download_location = *w->S1;

	g_timeout_add_seconds (3, update_bars, (gpointer) w );
	/* download until the queue is empty */
	while ( (*w->Q).size() != 0 ) {
		gtk_entry_set_text( (GtkEntry*)w->D2, "");
		gtk_spin_button_set_value( (GtkSpinButton*)w->D3, 1);
		gtk_spin_button_set_value( (GtkSpinButton*)w->D4, 1);
		/* initialized the progress bars */
		gtk_progress_bar_set_fraction( (GtkProgressBar*) w->D5, 0.0);
		gtk_progress_bar_set_fraction( (GtkProgressBar*) w->D6, 0.0);
		/* put some status text */
		std::string stat_text = 
			(*w->Q)[0].manga_name +": "+(*w->Q)[0].start_chapter+"->"+ (*w->Q)[0].end_chapter;
		gtk_label_set_text( (GtkLabel*) w->D8, stat_text.c_str() );

		/* init the downloader */
		if ( *w->CUR_PROV == 0 ) {
			(*w->DLDER_reader).init( 
					(*w->Q)[0].manga_name,
					download_location,
					download_command,
					(*w->Q)[0].start_chapter,
					(*w->Q)[0].end_chapter
			);
		}
		else if ( *w->CUR_PROV ==1 ) {
			(*w->DLDER_park).init( 
				(*w->Q)[0].manga_name,
				download_location,
				download_command,
				(*w->Q)[0].start_chapter,
				(*w->Q)[0].end_chapter
			);
		}
		/* remove the first value from the queue and the combo box */
		if ((*w->Q).size()!=0)
			(*w->Q).erase( (*w->Q).begin() );
		gtk_combo_box_remove_text( (GtkComboBox*)w->D1, 0);

		/* start the download */
		if ( *w->CUR_PROV == 0)
			(*w->DLDER_reader).run();
		else if ( *w->CUR_PROV ==1)
			(*w->DLDER_park).run();
	}
	/* reinit the stuff */
	gtk_entry_set_text( (GtkEntry*)w->D2, " ");
	gtk_spin_button_set_value( (GtkSpinButton*)w->D3, 1);
	gtk_spin_button_set_value( (GtkSpinButton*)w->D4, 1);
	gtk_progress_bar_set_fraction( (GtkProgressBar*)w->D5, 0.001);
	gtk_progress_bar_set_fraction( (GtkProgressBar*)w->D6, 0.001);
	gtk_button_set_label( (GtkButton*)w->D7, "Download");
	gtk_label_set_text( (GtkLabel*) w->D8, " " );
	*w->ISDL = 0;

	/* here, the timeout thing that updates the progress bars should have stopped */
	return NULL;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* update the bars during the download */
gint
Manga_GUI::update_bars( gpointer user_data )
{
	callback_items *w = (callback_items*) user_data;

	/* stop updating when the queue is empty */
	//if ( (*w->Q).size() == 0 || (*w->DLDER).finished() ||
	if ( *w->ISDL == 0 ) return false;
	mg_status stat;
	if (*w->CUR_PROV ==0 )
		stat =  (*w->DLDER_reader).get_status();
	else /* if(*w->CUR_PROV ==1) */
		stat =  (*w->DLDER_park).get_status();
	/*
	 * nb of chapters downloading = stat.end_chapter-stat.start_chapter+1;
	 * nb of already dlded        = stat.cur_chapter - stat.start_chapter+1;
	 *
	 */ 
	if (stat.end_chapter == 0 ) return true;
	gdouble chapter_progress = 
		(gdouble)(stat.cur_chapter-stat.start_chapter+1)
		/
		(gdouble)(stat.end_chapter-stat.start_chapter+1);
	gdouble page_progress = 
		(gdouble)(stat.cur_page)/
		(gdouble)(stat.end_page);
	if ( chapter_progress < 0 || chapter_progress > 1.0 ) return true;
	if ( page_progress < 0 || page_progress > 1.0 ) return true;

	gtk_progress_bar_set_fraction( (GtkProgressBar*) w->D5, chapter_progress);
	gtk_progress_bar_set_fraction( (GtkProgressBar*) w->D6, page_progress);
	return true;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* call back used when the download button is pressed */
void 
Manga_GUI::stop_callback(GtkWidget *wid, gpointer user_data)
{
	/* contains
	 * the current location S1 --not used
	 * download managers    DL --not used
	 * the queue            Q  --not used
	 * the queue combo box  D1 --not used
	 * the q name entry     D2
	 * the q start entry    D3
	 * the q end entry      D4
	 * The progress bar1    D5
	 * The progress bar2    D6
	 * the download button  D7
	 * the stat label       D8
	 * the combo providers  D9
	 * current provider     CUR_PROV
	 * is downloading atm   ISDL
	 * downloader instance  DLDER_*
	 */ 
	callback_items *w = (callback_items*) user_data;
	
	/* if we are not in the "nothing state" */
	if(*w->ISDL!=0) {
		GtkWidget *dialog1 = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
			GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Stoping download");
		gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
		gtk_dialog_run (GTK_DIALOG (dialog1));
		gtk_widget_destroy (dialog1);
		/* reset stuffs */
		gtk_entry_set_text( (GtkEntry*)w->D2, "  ");
		gtk_spin_button_set_value( (GtkSpinButton*)w->D3, 1);
		gtk_spin_button_set_value( (GtkSpinButton*)w->D4, 1);
		gtk_progress_bar_set_fraction( (GtkProgressBar*)w->D5, 0.001);
		gtk_progress_bar_set_fraction( (GtkProgressBar*)w->D6, 0.001);
		gtk_button_set_label( (GtkButton*)w->D7, "Download");
		gtk_label_set_text( (GtkLabel*) w->D8, "  " );
		*w->ISDL = 0;
		if ( *w->CUR_PROV == 0 ) 
			(*w->DLDER_reader).stop();
		else if (*w->CUR_PROV ==1)
			(*w->DLDER_park).stop();
		/* set the current provider to the one that is selected atm */
		*w->CUR_PROV = gtk_combo_box_get_active( (GtkComboBox*) w->D9);
	}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* pop up to select the location where the manga will be drop into */
void 
Manga_GUI::location_callback(GtkWidget *wid, gpointer user_data)
{
	std::string dirpath = "";
	callback_items *w = (callback_items*) user_data;

	GtkWidget *select_dir = gtk_file_chooser_dialog_new("Select a Dir", NULL,
		GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
		GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL );

	if (gtk_dialog_run(GTK_DIALOG (select_dir) ) == GTK_RESPONSE_ACCEPT)
		dirpath = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER (select_dir) );
	gtk_widget_destroy(select_dir);

	*((std::string *)w->S1) = dirpath;
	dirpath = "Download Location "+dirpath;
	gtk_label_set_text( (GtkLabel*) w->D1, dirpath.c_str() );
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* choose the download manager */
void 
Manga_GUI::dl_manager_callback(GtkWidget *wid, gpointer user_data)
{
	select_dl_manager* data = (select_dl_manager*) user_data;

	int position_to_choose  = gtk_combo_box_get_active ( (GtkComboBox*) data->D1 );
	if (position_to_choose == -1) {
		wpopup("Cannot select the command");
		return;
	}

	std::string selected_dl   = gtk_combo_box_get_active_text ( (GtkComboBox*)data->D1 );
	for (unsigned int i=0; i< (*data->DL).size() ; i++)
		if ( (*data->DL)[i].name==selected_dl){
			(*data->DL)[i].selected = true;
			std::cout<< (*data->DL)[i].name<<" Selected "<<"\n";
		}
		else {
			(*data->DL)[i].selected = false;
			std::cout<< (*data->DL)[i].name<<"\n";
		}
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Add a Download manager */
void 
Manga_GUI::add_dlmngr_callback(GtkWidget *wid, gpointer user_data) 
{
	select_dl_manager* data = (select_dl_manager*) user_data;


	std::string dl_name = gtk_entry_get_text( (GtkEntry*) data->D2 );
	std::string command = gtk_entry_get_text( (GtkEntry*) data->D3 );
	bool problem = false;
	for (unsigned int i=0; i< (*data->DL).size() ; i++)
		if ( (*data->DL)[i].name==dl_name)
			problem = true;
		else if ( (*data->DL)[i].command== command )
			problem = true;
	if (problem || dl_name=="Name" || command =="Command"|| dl_name==""||command =="") {
		wpopup("Cannot add the command");
		return;
	}
	/* Add to the combo box */
	gtk_combo_box_append_text( (GtkComboBox*) data->D1 , dl_name.c_str());
	/* Clear the Text boxes */
	gtk_entry_set_text( (GtkEntry*) data->D2, "");
	gtk_entry_set_text( (GtkEntry*) data->D3, "");
	/* Add to the list */
	dl_mngr new_dl_mngr;
	new_dl_mngr.name     = dl_name;
	new_dl_mngr.command  = command;
	new_dl_mngr.selected = false;
	(*data->DL).push_back(new_dl_mngr);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Remove a Download manager */
void 
Manga_GUI::del_dlmngr_callback(GtkWidget *wid, gpointer user_data) 
{
	select_dl_manager* data = (select_dl_manager*) user_data;
	int position_to_delete  = gtk_combo_box_get_active ( (GtkComboBox*) data->D1 );

	if (position_to_delete == -1) {
		wpopup("Cannot delete the command");
		return;
	}

	std::string selected_dl = gtk_combo_box_get_active_text ( (GtkComboBox*)data->D1 );
	/* remove from list */
	for (unsigned int i=0; i< (*data->DL).size() ; i++)
		if ( (*data->DL)[i].name==selected_dl)
			(*data->DL).erase( (*data->DL).begin()+i );
	/* Remove from the combo box */
	gtk_combo_box_remove_text ( (GtkComboBox*) data->D1, position_to_delete);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Save the configs in the database */
void 
Manga_GUI::save_cfg_callback(GtkWidget *wid, gpointer user_data)
{
	select_dl_manager* data = (select_dl_manager*) user_data;
	data->DB.update_mg_location( (*data->S1) );
	std::cout<< (*data->S1)<<"\n";
	data->DB.update_dl_manager( (*data->DL) );

	GtkWidget *dialog1 = gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL, 
		GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Database has been updated");
	gtk_window_set_position (GTK_WINDOW (dialog1), GTK_WIN_POS_CENTER);
	gtk_dialog_run (GTK_DIALOG (dialog1));
	gtk_widget_destroy (dialog1);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Add the manga selected to the queue */
void 
Manga_GUI::add_to_q_callback(GtkWidget *wid, gpointer user_data)
{
	callback_q_data *data = (callback_q_data*) user_data;

	/* check if the start chapter is less than the end chapter */
	int start_int = gtk_spin_button_get_value_as_int( (GtkSpinButton*) data->D2 );
	int end_int = gtk_spin_button_get_value_as_int( (GtkSpinButton*) data->D3 );
	if (start_int > end_int) {
		wpopup("can't download in reverse order");
		return;
	}

	/* get the manga name */
	std::string mgname_to_add = gtk_entry_get_text( (GtkEntry*) data->D1 );
	if (mgname_to_add == "Manga Name" || mgname_to_add == "") {
		wpopup("You didn't specify a correct manga name");
		return;
	}

	replaceAll(mgname_to_add, " ","");

	std::string to_add_to_combo = mgname_to_add+" | "+to_string(start_int)+" -> "+to_string(end_int);
	/* Add to the combo box */
	gtk_combo_box_append_text( (GtkComboBox*) data->D4 , to_add_to_combo.c_str());

	manga_queue_data new_to_queue;
	new_to_queue.manga_name = mgname_to_add;
	new_to_queue.start_chapter = to_string(start_int);
	new_to_queue.end_chapter = to_string(end_int);
	(*data->Q).push_back(new_to_queue);

	for(unsigned int i=0; i< (*data->Q).size() ; i++)
		std::cout<< (*data->Q)[i].manga_name <<"\n";

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* change the widget according to the selected manga in the queue */
void 
Manga_GUI::select_q_callback(GtkWidget *wid, gpointer user_data )
{
	callback_q_data *data = (callback_q_data*) user_data;
	/* starting from 0 */
	int active = gtk_combo_box_get_active( (GtkComboBox*) data->D4);
	if (active==-1) return;
	std::string active_name  = (*data->Q)[active].manga_name;
	std::string active_start = (*data->Q)[active].start_chapter;
	std::string active_end   = (*data->Q)[active].end_chapter;

	gtk_entry_set_text( (GtkEntry*) data->D1, active_name.c_str() );
	gtk_spin_button_set_value( (GtkSpinButton*) data->D2, atoi(active_start.c_str()));
	gtk_spin_button_set_value( (GtkSpinButton*) data->D3, atoi(active_end.c_str()));

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* change the queue */
void 
Manga_GUI::change_q_callback(GtkWidget *wid, gpointer user_data )
{
	callback_q_data *data = (callback_q_data*) user_data;
	/* starting from 0 */
	int active = gtk_combo_box_get_active( (GtkComboBox*) data->D4);
	if (active ==-1) return;
	int st = gtk_spin_button_get_value_as_int( (GtkSpinButton*) data->D2);
	int en = gtk_spin_button_get_value_as_int( (GtkSpinButton*) data->D3);

	if (st>en) {
		wpopup("Can't download in reverse order");
		return;
	}

	std::string active_start = to_string(st);
	std::string active_end   = to_string(en);

	(*data->Q)[active].start_chapter = active_start;
	(*data->Q)[active].end_chapter   = active_end;
	std::cout<<"here\n"
		<<active
		<<"\n";

	std::string to_add_to_combo = 
		(*data->Q)[active].manga_name+" | "+active_start+" -> "+active_end;
	std::cout<<to_add_to_combo <<"\n";
	/* Add to the combo box */
	gtk_combo_box_text_insert_text( (GtkComboBoxText*) data->D4 , active,to_add_to_combo.c_str());
	gtk_combo_box_set_active ( (GtkComboBox*) data->D4, active);

	gtk_combo_box_text_remove( (GtkComboBoxText*) data->D4, active+1);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* delete from the queue */
void 
Manga_GUI::delete_q_callback(GtkWidget *wid, gpointer user_data )
{
	callback_q_data *data = (callback_q_data*) user_data;
	/* starting from 0 */
	int active = gtk_combo_box_get_active( (GtkComboBox*) data->D4);
	if (active ==-1) return;


	/* clear the text boxes */
	gtk_entry_set_text( (GtkEntry*) data->D1, "");
	gtk_spin_button_set_value( (GtkSpinButton*) data->D2, 1);
	gtk_spin_button_set_value( (GtkSpinButton*) data->D3, 1);
	/* remove from the combo box */
	std::cout<<active<<"\n";
	gtk_combo_box_remove_text ( (GtkComboBox*) data->D4, active);
	/* remove from the queue */
	(*data->Q).erase( (*data->Q).begin()+active );
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* fill the file menu */
void
Manga_GUI::file_menu_fill(GtkWidget *menubar)
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
Manga_GUI::config_menu_fill(GtkWidget *menubar)
{
	/* location */
	GtkWidget *config_menu = gtk_menu_item_new_with_mnemonic ("_Configs");
	GtkWidget *item_container = gtk_menu_new ();

	GtkWidget *location_item = gtk_menu_item_new_with_mnemonic ("Mg _Location");
	wstruct_loc.D1 = location_text;
	wstruct_loc.S1 = &current_location;
	g_signal_connect (location_item, "activate", G_CALLBACK(location_callback), &wstruct_loc);

	gtk_menu_append (GTK_MENU (item_container), location_item);

	gtk_widget_add_accelerator (location_item, "activate", accel_group, 
			GDK_l, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	GtkWidget *save_item = gtk_menu_item_new_with_mnemonic ("_Save Configs");
	dl_mngr_callback_data.DL = &download_managers;
	dl_mngr_callback_data.DB = db;
	dl_mngr_callback_data.S1 = &current_location;
	g_signal_connect (save_item, "activate", G_CALLBACK (save_cfg_callback), &dl_mngr_callback_data);
	gtk_menu_append (GTK_MENU (item_container), save_item);
	gtk_widget_add_accelerator (save_item, "activate", accel_group, 
		GDK_s, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);


	GtkWidget *revert_item = gtk_menu_item_new_with_mnemonic ("_Revert Configs");
	//	g_signal_connect (revert_item, "activate", G_CALLBACK(about_this), (gpointer) win);
	gtk_menu_append (GTK_MENU (item_container), revert_item);
	gtk_widget_add_accelerator (revert_item, "activate", accel_group, 
		GDK_r, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);

	gtk_menu_item_set_submenu (GTK_MENU_ITEM (config_menu), item_container);
	gtk_menu_bar_append (GTK_MENU_BAR (menubar), config_menu);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* fill the updater menu */
void
Manga_GUI::updater_menu_fill(GtkWidget *menubar)
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
Manga_GUI::help_menu_fill(GtkWidget *menubar)
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
Manga_GUI::hpack_1()
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
Manga_GUI::hpack_2()
{
	/* create a new label. */
	label = gtk_label_new ("MangaDL");
	gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
	gtk_misc_set_padding(GTK_MISC (label),50,20);
	gtk_label_set_markup( GTK_LABEL(label),
		"<span foreground='#007ef9' size='large'><b>MangaDL</b></span>");
	gtk_box_pack_start (GTK_BOX (mainvbox), label, FALSE, FALSE, 0);

	/* create a new label. */
	stat_label = gtk_label_new ("");
	gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
	gtk_box_pack_start (GTK_BOX (mainvbox), stat_label, FALSE, FALSE, 0);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the text box where you insert the manga name*/
void 
Manga_GUI::hpack_3()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, false, 0);

	texta =gtk_entry_new();
	gtk_entry_set_text((GtkEntry*)texta,"Manga Name");
	gtk_box_pack_start (GTK_BOX (hbox),texta , true, true, 10); //box,child,expand,fill,padding

	add_queue_buton = gtk_button_new();
	gtk_button_set_label( (GtkButton*)add_queue_buton, "Add to Queue");
	gtk_box_pack_start (GTK_BOX (hbox), add_queue_buton, false, false, 10);
	
	/* The callback is in hpack3_1() because the wheel doesn't exist before that */

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */


/* The wheels */
void
Manga_GUI::hpack_4()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, true, 5);

	label = gtk_label_new ("From");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
	//spin
	wheel = gtk_spin_button_new_with_range(1,2000,1);
	gtk_box_pack_start(GTK_BOX (hbox), wheel, true, true, 10);

	label = gtk_label_new ("to");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
	//spin
	wheel2 = gtk_spin_button_new_with_range(1,2000,1);
	gtk_box_pack_start(GTK_BOX (hbox), wheel2, true, true, 10);

}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* progress bar - chapter */
void
Manga_GUI::hpack_5()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, false, 10);
	
	pbar1 = gtk_progress_bar_new();
	gtk_box_pack_start( GTK_BOX(hbox), pbar1, true, true, 10);

	label = gtk_label_new ("Chapters");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* progress bar - page */
void
Manga_GUI::hpack_6()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox), hbox, true, false, 10);
	
	pbar2 = gtk_progress_bar_new();
	gtk_box_pack_start( GTK_BOX(hbox), pbar2, true, true, 10);

	label = gtk_label_new ("Pages   ");
	gtk_box_pack_start( GTK_BOX(hbox), label, false, false, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Buttons at the bottom */
void 
Manga_GUI::hpack_7()
{
	///hbox
	hbox = gtk_hbox_new (TRUE,4);
	//gtk_container_add (GTK_CONTAINER (vbox), hbox);
	gtk_box_pack_end(GTK_BOX (vbox), hbox,TRUE, TRUE, 0);
	///image button
	image_buton = gtk_button_new();
	stop_button = gtk_button_new_with_label("Stop");
	/* the callback is at the end of the packs */
	
	/* This calls our box creating function , the box is inserted inside the button*/
	gtk_button_set_label((GtkButton*)image_buton, "Download");
	//	imbox = xpm_label_box ((gchar *)"data/button.png",(gchar *)"");
	//	gtk_container_add (GTK_CONTAINER (image_buton), imbox);
	//	gtk_container_border_width (GTK_CONTAINER(image_buton), 0);
	gtk_box_pack_start (GTK_BOX (hbox), image_buton, true, true, 10);
	gtk_box_pack_start (GTK_BOX (hbox), stop_button, true,true, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the text box with the location */
void 
Manga_GUI::hpack2_1()
{
	hbox = gtk_hbox_new (false,20);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 20);

	std::string label_text = "Download Location "+current_location;
	location_text = gtk_label_new( label_text.c_str() );
	gtk_box_pack_start (GTK_BOX (hbox),location_text, true, true, 10); //box,child,expand,fill,padding

	GtkWidget *location_buton = gtk_button_new();
	wstruct_loc.D1 = location_text;
	wstruct_loc.S1 = &current_location;
	/* Connect the "clicked" signal of the button to our callback */
	g_signal_connect (location_buton, "released", G_CALLBACK (location_callback), &wstruct_loc);
	gtk_button_set_label( (GtkButton*)location_buton, "Choose");
	gtk_box_pack_start (GTK_BOX (hbox), location_buton, false, false, 10);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw a combo box with all the dl_managers name + 2 buttons select and delete */
void 
Manga_GUI::hpack2_2()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 10);
	GtkWidget *dl_label = gtk_label_new("Download Manager ");
	gtk_box_pack_start (GTK_BOX (hbox), dl_label, false, false, 10); //box,child,expand,fill,padding
	dl_manager_combo = gtk_combo_box_new_text ();
	
	for(unsigned int i=0;i<download_managers.size();i++) {
		gtk_combo_box_append_text( (GtkComboBox*) dl_manager_combo , 
			download_managers[i].name.c_str());
		if ( download_managers[i].selected )
			gtk_combo_box_set_active( (GtkComboBox*)  dl_manager_combo, i );
	}

	gtk_box_pack_start (GTK_BOX (hbox), dl_manager_combo, true, true, 3); //box,child,expand,fill,padding

	GtkWidget *select_button = gtk_button_new();
	GtkWidget *delete_button = gtk_button_new();

	gtk_button_set_label( (GtkButton*)select_button, "Select");
	gtk_button_set_label( (GtkButton*)delete_button, "Delete");

	gtk_box_pack_start (GTK_BOX (hbox), select_button, false, false, 10);
	gtk_box_pack_start (GTK_BOX (hbox), delete_button, false, false, 10);

	dl_mngr_callback_data.DL = &download_managers;
	dl_mngr_callback_data.D1 = dl_manager_combo;

	g_signal_connect (select_button, "released", 
		G_CALLBACK (dl_manager_callback), &dl_mngr_callback_data);
	g_signal_connect (delete_button, "released", 
		G_CALLBACK (del_dlmngr_callback), &dl_mngr_callback_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw 2 text box, name of dl, dl command + a button to add */
void 
Manga_GUI::hpack2_3()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 10);

	GtkWidget *label_add = gtk_label_new("Add a DL Manager");
	gtk_box_pack_start (GTK_BOX (hbox), label_add, false, false, 10); //box,child,expand,fill,padding

	name_dl     =  gtk_entry_new();
	gtk_entry_set_text( (GtkEntry*) name_dl, "Name" );
	command_dl  =  gtk_entry_new();
	gtk_entry_set_text( (GtkEntry*) command_dl, "Command" );

	GtkWidget *add_dl_button = gtk_button_new();
	gtk_button_set_label( (GtkButton*)add_dl_button, "Add   ");

	gtk_box_pack_start (GTK_BOX (hbox), name_dl,    false, false, 2);
	gtk_box_pack_start (GTK_BOX (hbox), command_dl, true, true, 2);
	gtk_box_pack_start (GTK_BOX (hbox), add_dl_button, false, false, 10);

	dl_mngr_callback_data.DL = &download_managers;
	dl_mngr_callback_data.D1 = dl_manager_combo;
	dl_mngr_callback_data.D2 = name_dl;
	dl_mngr_callback_data.D3 = command_dl;
	dl_mngr_callback_data.DB = db;

	g_signal_connect (add_dl_button, "released", 
		G_CALLBACK (add_dlmngr_callback), &dl_mngr_callback_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* List of Providers -- Mangareader (default) and Mangapark */
void 
Manga_GUI::hpack2_4()
{
	hbox = gtk_hbox_new (false,10);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, false, false, 10);
	GtkWidget *label_provider = gtk_label_new("Provider        ");
	gtk_box_pack_start (GTK_BOX (hbox), label_provider, false, false, 10); //box,child,expand,fill,padding

	provider_combo = gtk_combo_box_new_text ();

	gtk_combo_box_append_text( (GtkComboBox*) provider_combo , "mangareader");
	gtk_combo_box_append_text( (GtkComboBox*) provider_combo, "mangapark");
	gtk_combo_box_set_active( (GtkComboBox*) provider_combo, 0);

	gtk_box_pack_start (GTK_BOX (hbox), provider_combo, true, true, 10); //box,child,expand,fill,padding
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the Save configs button */
void
Manga_GUI::hpack2_5()
{
	hbox = gtk_hbox_new (true,3);
	gtk_box_pack_start(GTK_BOX (vbox2), hbox, true, true, 0);
	GtkWidget *save_button = gtk_button_new();
	gtk_button_set_label( (GtkButton*)save_button, "Save Configs");
	gtk_box_pack_start (GTK_BOX (hbox), save_button, true, true, 0);

	dl_mngr_callback_data.DL = &download_managers;
	dl_mngr_callback_data.DB = db;
	dl_mngr_callback_data.S1 = &current_location;

	g_signal_connect (save_button, "released", 
		G_CALLBACK (save_cfg_callback), &dl_mngr_callback_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Queue part */
void
Manga_GUI::hpack3_1()
{
	hbox = gtk_hbox_new (false,0);
	gtk_box_pack_start(GTK_BOX (vbox3), hbox, false, false, 10);
	queue_combo = gtk_combo_box_text_new();
	GtkWidget *queue_label = gtk_label_new("Queue ");
	gtk_box_pack_start (GTK_BOX  (hbox), queue_label, false, false, 10);
	gtk_box_pack_start (GTK_BOX (hbox), queue_combo, true, true, 10);



	/* callback from hpack_3() -- addtoqueue */
	q_c_data.Q  = &queue;
	q_c_data.D1 = texta; /*mgname*/
	q_c_data.D2 = wheel; /*wheel1*/
	q_c_data.D3 = wheel2; /*wheel2*/
	q_c_data.D4 = queue_combo; /*combo*/
	g_signal_connect (add_queue_buton, "released", 
		G_CALLBACK (add_to_q_callback), &q_c_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Queue part */
void
Manga_GUI::hpack3_2()
{
	hbox = gtk_hbox_new (false,0);
	gtk_box_pack_start(GTK_BOX (vbox3), hbox, false, false, 10);
	GtkWidget *change_q_name_label = gtk_label_new("Manga ");
	q_name_entry = gtk_entry_new();
	gtk_entry_set_text( (GtkEntry*) q_name_entry, "manga name");
	gtk_entry_set_editable( (GtkEntry*) q_name_entry, false);
	GtkWidget *change_q_start_label = gtk_label_new("Start ");
	q_start_entry = gtk_spin_button_new_with_range(1,2000,1);
	GtkWidget *change_q_end_label = gtk_label_new("End ");
	q_end_entry = gtk_spin_button_new_with_range(1,2000,1);

	gtk_box_pack_start (GTK_BOX (hbox), change_q_name_label, false, false, 10);
	gtk_box_pack_start ( GTK_BOX(hbox), q_name_entry, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), change_q_start_label, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), q_start_entry, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), change_q_end_label, true, true, 3);
	gtk_box_pack_start ( GTK_BOX(hbox), q_end_entry, true, true, 10);

//	g_signal_connect (select_button, "released", G_CALLBACK (dl_manager_callback), &dl_mngr_callback_data);
//

	q_combo_data.Q  = &queue;
	q_combo_data.D1 = q_name_entry;
	q_combo_data.D2 = q_start_entry;
	q_combo_data.D3 = q_end_entry;
	q_combo_data.D4 = queue_combo;
	g_signal_connect (queue_combo, "changed", 
		G_CALLBACK (select_q_callback), &q_combo_data);
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Change queue */
void
Manga_GUI::hpack3_3()
{
	hbox = gtk_hbox_new (false,0);
	gtk_box_pack_start(GTK_BOX (vbox3), hbox, false, false, 10);
	GtkWidget *delete_q = gtk_button_new();
	gtk_button_set_label( (GtkButton*) delete_q, "Delete");
	GtkWidget *change_q = gtk_button_new();
	gtk_button_set_label( (GtkButton*) change_q, "Change");

	gtk_box_pack_start (GTK_BOX (hbox), change_q, true, true, 10);
	gtk_box_pack_start ( GTK_BOX(hbox), delete_q, true, true, 10);


	q_combo_data.Q  = &queue;
	q_combo_data.D1 = q_name_entry;
	q_combo_data.D2 = q_start_entry;
	q_combo_data.D3 = q_end_entry;
	q_combo_data.D4 = queue_combo;
	g_signal_connect( change_q, "released", 
		G_CALLBACK (change_q_callback), &q_combo_data);
	g_signal_connect (delete_q, "released", 
		G_CALLBACK (delete_q_callback), &q_combo_data);


	/* download and stop buttons callbacks */
	wstruct.S1    = &current_location;
	wstruct.DL    = &download_managers;
	wstruct.Q     = &queue;
	wstruct.D1    = queue_combo;
	wstruct.D2    = q_name_entry;
	wstruct.D3    = q_start_entry;
	wstruct.D4    = q_end_entry;
	wstruct.D5    = pbar1;
	wstruct.D6    = pbar2;
	wstruct.D7    = image_buton;
	wstruct.D8    = stat_label;
	wstruct.D9    = provider_combo;
	wstruct.ISDL  = &downloader_state;
	wstruct.DLDER_reader = &downloader_reader;
	wstruct.DLDER_park   = &downloader_park;
	wstruct.CUR_PROV = &current_provider;

	g_signal_connect (image_buton, "released", G_CALLBACK (download_callback), &wstruct);
	g_signal_connect (stop_button, "released", G_CALLBACK (stop_callback), &wstruct);


}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* Draw the main window of the program */
void
Manga_GUI::draw_main_win()
{
	/* Start */
	/* Create the main window */
	win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
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
	gtk_widget_set_size_request ( (GtkWidget *)win, 600,350);
	gtk_window_set_resizable ( GTK_WINDOW (win), false);
//	gtk_window_set_policy (GTK_WINDOW(win), FALSE, FALSE, FALSE);
	gtk_window_set_title (GTK_WINDOW (win), "Manga Downloader");
	gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
	gtk_widget_realize (win);
	accel_group            =gtk_accel_group_new ();
	/* Attach the new accelerator group to the window. */
	gtk_window_add_accel_group (GTK_WINDOW (win), accel_group);
	/* Start */
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

/* put stuff inside the notebook */
void
Manga_GUI::pack_in_notebook()
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
Manga_GUI::Display(int argc, char *argv[])
{
	/* Initialize GTK+ */
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
	gtk_init (&argc, &argv);
	g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);
	/* draw main window */
	draw_main_win();
	/* vbox -- contains 7 hbox */
	vbox = gtk_vbox_new (false,0);
	vbox2 = gtk_vbox_new(false,0);
	vbox3 = gtk_vbox_new(false,0);
	mainvbox = gtk_vbox_new(false,0);
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

/* Testing in the main */
int main (int argc, char *argv[])
{
	Manga_GUI MG_DL;
	MG_DL.Display(argc, argv);
	return 0;
}
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */
