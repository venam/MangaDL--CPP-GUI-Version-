/*
Copyright (c) 2013, Patrick Louis <patrick-louis at unixhub.net>
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    1.  The author is informed of the use of his/her code. The author does not have to consent to the use; however he/she must be informed.
    2.  If the author wishes to know when his/her code is being used, it the duty of the author to provide a current email address at the top of his/her code, above or included in the copyright statement.
    3.  The author can opt out of being contacted, by not providing a form of contact in the copyright statement.
    4.  If any portion of the author’s code is used, credit must be given.
            a. For example, if the author’s code is being modified and/or redistributed in the form of a closed-source binary program, then the end user must still be made somehow aware that the author’s work has contributed to that program.
            b. If the code is being modified and/or redistributed in the form of code to be compiled, then the author’s name in the copyright statement is sufficient.
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
* clang++ mgreader.cpp -Wall -lcurl -lpthread -o mgreader
* g++ mgreader.cpp Browser/Browser.cpp -IBrowser -g -Wall -lcurl -o mgreader
*
* TODO: If it's 404 return something that specify it doesn't exist.
*       start downloading from a certain page
*
*/

#ifndef INCLUDED_MGFOX
#define INCLUDED_MGFOX

#include "../../browser/browser.hpp"
#include "../dlder.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


/*
 		browser      dlder_br;
		std::string  dlder_manganame;
		std::string  dlder_mg_location;
		std::string  dlder_external_dl_mgr;
		std::string  dlder_start;
		std::string  dlder_end;
		std::string  dlder_url_img;
		std::string  dlder_img;

		unsigned int dlder_nb_of_chapter;
		unsigned int dlder_current_chapter;
		unsigned int dlder_nb_of_pages;
		unsigned int dlder_current_page;
		bool         dlder_paused_flag;
		bool         dlder_stop_flag;

		void create_browser();
		void manage_chapters();
		void download_image();
 */

class mgfox : public dlder {
	private:
		//all the chapter links 
		//mgfox_all_chapters["11"] might contain link/11, link/11.1/, and link/11.2
		std::vector < std::vector<std::string> > mgfox_all_chapters;
		//current url
		std::string mgfox_current_page_url;
		//the next link
		std::string mgfox_next_link_url;
		//check if reached the end of current chapter
		bool chapter_end();
		//scrap the page for the image + number of page er chapter + next link
		void scrap_page();
		//get all the chapters
		void scrap_first_page();
		//set-up the string for the page number
		void increase_current();
		//increase chapter count
		void increase_chapter();
		//open the next page
		void go_to_next_page();
		//open the first page to get all the chapter links and arrange them
		void open_first_page();
		//start dl
		void start_downloading();
		//download and place images for a chapter with sub chapters
		void download_chapter(int chapter);
		std::string get_next_link(std::string sub_chapter);

	public:
		/*needs to be implemented */
		mgfox(
				std::string manganame, std::string mg_location,
				std::string external_dl_mgr,std::string start, std::string end);
		mgfox();
		~mgfox();

		void run();
		mg_status get_status();
		void pause_unpause();
		bool finished();
		void stop();
		void init(std::string manganame, std::string mg_location,
			std::string external_dl_mgr,std::string start, std::string end);
};


#endif

