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

#ifndef INCLUDED_CALLBACKS
#define INCLUDED_CALLBACKS

#include "../../cfg/config.hpp"
#include "../../dlder/dlder.hpp"
#include "../gui.hpp"


class callbacks {
	private:
		//the widgets
		GtkWidget*                     callbacks_win;
		GtkWidget*                     callbacks_vbox;
		GtkWidget*                     callbacks_vbox2;
		GtkWidget*                     callbacks_vbox3;
		GtkWidget*                     callbacks_mainvbox;
		GtkWidget*                     callbacks_hbox;
		GtkWidget*                     callbacks_stop_button;
		GtkWidget*                     callbacks_label;
		GtkWidget*                     callbacks_stat_label;
		GtkWidget*                     callbacks_image_buton;
		GtkWidget*                     callbacks_add_queue_buton;
		GtkWidget*                     callbacks_imbox;
		GtkWidget*                     callbacks_wheel;
		GtkWidget*                     callbacks_wheel2;
		GtkWidget*                     callbacks_texta;
		GtkWidget*                     callbacks_location_text;
		GtkWidget*                     callbacks_dl_manager_combo;
		GtkWidget*                     callbacks_name_dl;
		GtkWidget*                     callbacks_command_dl;
		GtkWidget*                     callbacks_queue_combo;
		GtkWidget*                     callbacks_q_name_entry;
		GtkWidget*                     callbacks_q_start_entry;
		GtkWidget*                     callbacks_q_end_entry;
		GtkWidget*                     callbacks_pbar1;
		GtkWidget*                     callbacks_pbar2;
		GtkWidget*                     callbacks_provider_combo;
		GtkWidget*                     callbacks_revert_item;
		GtkAccelGroup*                 callbacks_accel_group;


		//the state of the download
		int*                           callbacks_downloader_state;

		//the downloader
		dlder*                         callbacks_downloader;

		dl_facto*                     callbacks_factory;

		//config which contains the temporary data and the revert data
		/*
		std::string                   cfg_current_location;
		std::vector<dl_mngr>          cfg_download_managers;
		std::vector<manga_queue_data> cfg_queue;

		std::string                   cfg_revert_current_location;
		std::vector<dl_mngr>          cfg_revert_download_managers;
		*/
		config*                        callbacks_config;
	public:
		friend class gui;

};


#endif

