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
* clang++ mgrpark.cpp -Wall -lcurl -lpthread -o mgreader
*
* TODO: If it's 404 return something that specify it doesn't exist.
*       start downloading from a certain page
*
*/

#include "mgpark.hpp"

mgpark::mgpark(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
	: dlder(manganame, mg_location, external_dl_mgr, start, end)
{
}


mgpark::mgpark()
	: dlder()
{
}

void
mgpark::init(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
{
	dlder_manganame           = manganame;
	dlder_mg_location         = mg_location;
	dlder_external_dl_mgr     = external_dl_mgr;
	dlder_start                = start;
	dlder_end                 = end;
	dlder_url_img             = "";
	dlder_img                 = "000";
	dlder_nb_of_chapter       = 0;
	dlder_current_chapter     = atoi(start.c_str());
	dlder_nb_of_pages         = 0;
	dlder_current_page        = 1;
	dlder_paused_flag         = false;
	dlder_stop_flag           = false;
}


mgpark::~mgpark()
{
}

mg_status 
mgpark::get_status()
{
	mg_status stat;
	stat.start_chapter = atoi(dlder_start.c_str());
	stat.cur_chapter   = dlder_current_chapter;
	stat.end_chapter   = atoi(dlder_end.c_str());
	stat.cur_page      = dlder_current_page-1;
	stat.end_page      = dlder_nb_of_pages;
	stat.finished      = dlder_stop_flag;
	return stat;
}

void
mgpark::stop()
{
	dlder_stop_flag = true;
}

void
mgpark::pause_unpause()
{
	dlder_paused_flag = dlder_paused_flag? false: true;
}

bool
mgpark::finished()
{
	return dlder_stop_flag;
}

void 
mgpark::increase_current()
{
	dlder_img   = to_string(atoi(dlder_img.c_str())+1);
	if (dlder_img.size() == 1) {
		dlder_img = "00"+dlder_img;
	}
	else if (dlder_img.size() == 2) {
		dlder_img = "0"+dlder_img;
	}
	else {
	}
	dlder_current_page++;
}

void
mgpark::increase_chapter()
{
	dlder_nb_of_pages     = 0;
	dlder_current_page    = 1;
	dlder_img             = "000";
	dlder_current_chapter = dlder_current_chapter+1;
	dlder_current_page++;
}

void
mgpark::scrap_page()
{
	std::vector<std::string> response_lines = split(dlder_br.response(), '\n');
	//loop backward so there will be no need to reverse the vector later on
	for (int i=response_lines.size()-1; i>=0;i--) {
		//a new chapter has been found
		if (word_in(response_lines[i],">All</a>")) {
			std::vector<std::string> url_split = split(response_lines[i], '\"');
			std::string url = url_split[1];
			int manga_name_place = url.find(dlder_manganame);
			std::string tmp = url.substr(manga_name_place+dlder_manganame.size(),url.size()-1);
			if ( !word_in( tmp,"-" ) || !word_in( tmp, "+") || !word_in( tmp, ".") ) {
				mgpark_chapters.push_back( "http://www.mangapark.com"+ url);
			}
			else {
			}
		}
	}
	if ( mgpark_chapters.size() == 0 ) {
		std::cout<<"No Chapter for this manga\n";
	}
	else {
		//everything went well
	}
}

void
mgpark::scrap_images()
{
	std::vector<std::string> response_lines;
	response_lines = split(dlder_br.response(), '\n');
	for (unsigned int i=0;i<response_lines.size();i++) {
		//this line contains an image
		if (word_in(response_lines[i], "<em><a target=\"_blank\"") ) {
			std::vector<std::string> splitter = split(response_lines[i], '\"');
			mgpark_imgs.push_back(splitter[3]);
		}
		else {
			//not found ~~ weird
		}
	}
	dlder_nb_of_pages = mgpark_imgs.size() -1;
}

void
mgpark::download_each_imgs()
{
	manage_chapters();
	for (int i=0;i<mgpark_imgs.size();i++) {
		dlder_url_img = mgpark_imgs[i];
		if (dlder_stop_flag ) {
			return;
		}
		else {
			while (dlder_paused_flag) {
				sleep(3);
			}
			download_image();
			increase_current();
		}
	}
}

void
mgpark::open_new_chapter(int chapt)
{
	do {
		std::cout<<mgpark_chapters[chapt]<<"\n";
		dlder_br.open(mgpark_chapters[chapt],20);
	} while( dlder_br.error() );
}

void
mgpark::start_downloading()
{
	if ( atoi(dlder_end.c_str()) > mgpark_chapters.size() ) {
		dlder_end = to_string(mgpark_chapters.size()-1);
	}
	else {
	}

	if ( dlder_end == dlder_start ) {
		open_new_chapter( atoi(dlder_start.c_str() ) );
		scrap_images();
		download_each_imgs();
		dlder_img = "000";
		mgpark_imgs.clear();
		dlder_stop_flag = true;
	}

	else {
		for ( 
				int i= atoi(dlder_start.c_str())-1; 
				i < atoi(dlder_end.c_str()); 
				i++ 
			) {
			open_new_chapter(i);
			scrap_images();
			download_each_imgs();
			dlder_img = "000";
			mgpark_imgs.clear();
			increase_chapter();
		}
		dlder_stop_flag = true;
	}
}

void
mgpark::open_first_page()
{
	do {
		dlder_br.open("http://www.mangapark.com/manga/"+dlder_manganame,100);
	} while(dlder_br.error());
}

void
mgpark::run()
{
	open_first_page();
	scrap_page();
	start_downloading();
	std::cout<< "[*] Finished all the downloads\nEnjoy Your Reading!\n";
}

