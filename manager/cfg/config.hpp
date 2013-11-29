/*
 *
 * COPYRIGHT AND PERMISSION NOTICE

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

 * Manages the configs in the sqlite3 DB
 *
 * CREATE TABLE location ( mg_location VARCHAR);
 *
 * CREATE TABLE mg_list (mgname VARCHAR, mgchapter VARCHAR, mglastdate VARCHAR);
 *
 * CREATE TABLE dl_managers (name VARCHAR UNIQUE PRIMARY KEY,command VARCHAR UNIQUE, selected BOOL);
 *
 * CREATE TABLE provider ( name VARCHAR ); 
 * contains the current provider "mangareader"
 *
 * TODO:
 *     Language for the external download manager.
 *     replaces [INPUT] by the input file name and [OUTPUT] by the output name
 *
 */
#include "config.cpp"

struct mg_list {
	std::string mgname;
	std::string mgchapter;
	std::string mglastdate;
};

struct dl_mngr {
	std::string name;
	std::string command;
	bool selected;
};

struct manga_queue_data {
	std::string manga_name;
	std::string start_chapter;
	std::string end_chapter;
};

class db_manager {
	private:
		sqlite3 *db;
		char* zErrMsg;
		int rc;
		static int exec_callback(void* NotUsed, int argc, char** argv, char** azColName);
		/* needed from the GUI */
		std::string                   cfg_current_location;
		std::vector<dl_mngr>          cfg_download_managers;
		std::vector<manga_queue_data> cfg_queue;
		/* -- */

	public:
		db_manager();
		~db_manager();
		void exec(std::string command);
		std::string get_mg_location();
		void update_mg_location(const std::string new_location);
		std::string get_provider();
		void update_provider(const std::string new_provider);
		std::vector <dl_mngr> get_dl_managers();
		void add_dl_manager(const std::string name, const std::string command, const bool selected);
		void update_dl_manager(const std::vector <dl_mngr> new_managers);
		void remove_dl_manager(const std::string name);
		std::vector <mg_list> get_mg_list();
		
		void set_current_location(std::string user_current_location);
		std::string get_current_location();
		void set_download_managers(std::vector<dl_mngr> user_download_managers);
		std::vector<dl_mngr> get_download_managers();
		void set_queue(std::vector<manga_queue_data> user_queue);
		std::vector<manga_queue_data> get_queue();
};

