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

#ifndef INCLUDED_MGREADER
#define INCLUDED_MGREADER

#include "../../browser/browser.hpp"
#include "../dlder.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>


class mgreader: public dlder {
	private:
		//vars
		std::map <std::string,std::string> mgreader_resolver;
		std::string mgreader_current_page_url;
		std::string mgreader_next_link;
		//foos
		bool not_published();
		bool chapter_end();
		void scrap_page();
		void fill_in_resolver();
		void increase_current();
		void increase_chapter();
		void go_to_next_page();
		void autoresolve();
		void start_downloading();
		void download_image();

	public:
		/*needs to be implemented */
		mgreader(
				std::string manganame, std::string mg_location,
				std::string external_dl_mgr,std::string start, std::string end);
		mgreader();
		~mgreader();

		void run();
		mg_status get_status();
		void pause_unpause();
		bool finished();
		void stop();
		void init(std::string manganame, std::string mg_location,
			std::string external_dl_mgr,std::string start, std::string end);
};

#endif
