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
 *  error handling in the dlder (check if the download manager command works fine
 *  check for 404 errors )
 *  Add more providers
 *  Add the Updater to it
 *  Skip chapter button 
 *  when the add to queue is pressed the input fields are cleared
 *  Write a Makefile
 *
 */ 

#ifndef GUI_INCLUDED
#define GUI_INCLUDED

#include <iostream>
#include <stdlib.h>
#include <string>
#include <pthread.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms-compat.h>

#include "../cfg/config.hpp"
#include "../dlder/dl_facto.hpp"
#include "callbacks/callbacks.hpp"

/* GUI Class handling all the GUI and Callbacks */
class gui {
	public:
		gui(int argc, char *argv[]);
		~gui();
		void Display();

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
		GtkWidget         *revert_item;
		GtkAccelGroup     *accel_group;
		config*           gui_config;
		dlder*            downloader;
		int               downloader_state;
		callbacks         callbacks_data;
		dl_facto          factory;

		void init_config                ( void                                              ) ;
		void init_callbacks             ( void                                              ) ;
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
		static void rever_callback      ( GtkWidget *wid, gpointer user_data                ) ;
		static void add_to_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void select_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;   
		static void change_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void delete_q_callback   ( GtkWidget *wid, gpointer user_data                ) ;
		static void *start_download     ( void *user_data                                   ) ;
		static gint update_bars         ( gpointer user_data                                ) ;
};
/* ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- */

#endif

