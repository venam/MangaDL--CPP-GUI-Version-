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

*/

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

#include "mgfox.hpp"

///=================================================================================///
mgfox::mgfox(
		std::string manganame, std::string mg_location,
		std::string external_dl_mgr,std::string start, std::string end) 
	: dlder( manganame, mg_location, external_dl_mgr, start, end)
{
	mgfox_current_page_url    ="http://www.mangafox.me/manga"+ dlder_manganame+"/"+dlder_start +"/";
	dlder_current_page        = 2;
	mgfox_next_link_url       = "";
}
///=================================================================================///


///=================================================================================///
mgfox::mgfox() : dlder()
{
	mgfox_current_page_url    ="http://www.mangafox.me/manga"+ dlder_manganame+"/"+dlder_start +"/";
	dlder_current_page        = 2;
	mgfox_next_link_url       = "";
}
///=================================================================================///


///=================================================================================///
mgfox::~mgfox()
{
}
///=================================================================================///


///=================================================================================///
void mgfox::init(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
{
	dlder::init(manganame,mg_location,external_dl_mgr, start, end);

	mgfox_current_page_url    ="http://www.mangafox.me/manga"+ dlder_manganame+"/"+dlder_start +"/";
	dlder_current_page        = 2;
	mgfox_next_link_url       = "";
}
///=================================================================================///


///=================================================================================///
mg_status mgfox::get_status()
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
///=================================================================================///


///=================================================================================///
void mgfox::pause_unpause()
{
	dlder_paused_flag = dlder_paused_flag? false: true;
}
///=================================================================================///


///=================================================================================///
bool mgfox::finished()
{
	return dlder_stop_flag;
}
///=================================================================================///


///=================================================================================///
void mgfox::stop()
{
	dlder_stop_flag = true;
}
///=================================================================================///


///=================================================================================///
void mgfox::scrap_first_page() 
{
	//get all lines in the html separately
	std::vector<std::string> response_lines = split(dlder_br.response(), '\n');
	//loop backward
	int chapter = -1;
	for (int i = response_lines.size()-1; i>-1; i--) {
		if( (word_in(response_lines[i],"http://mangafox.me/manga/"+dlder_manganame+"/c") or
		word_in(response_lines[i],"http://mangafox.me/manga/"+dlder_manganame+"/v")) and
		word_in(response_lines[i],"\"") and not
		word_in(response_lines[i],"is coming next") and not
		word_in(response_lines[i],"...") ) {
			//split by "
			std::vector<std::string> splitter = split(response_lines[i], '\"');
			//needs to find out if it's a sub chapter, if there's a . in the chapter
			std::vector<std::string> splitter2 = split(splitter[1], '/');

			//if there's a volume then there's one more /
			int where = 5;
			if (word_in(response_lines[i],"http://mangafox.me/manga/"+dlder_manganame+"/v")) {
				where = 6;
			}

			//save the last chapt
			std::string last_chapt = " ";
			if (chapter!=-1) {
				std::string last_chapt_pre = mgfox_all_chapters[chapter][0];
				int where_2 = 5;
				if (word_in(last_chapt_pre,"http://mangafox.me/manga/"+dlder_manganame+"/v")) {
					where = 6;
				}
				std::vector<std::string> splitter3 = split(last_chapt_pre, '/');
				if (word_in(splitter3[where],".")) {
					std::vector<std::string> splitter4 = split(splitter3[where],'.');
					last_chapt = splitter4[0];
				}
				else {
					last_chapt = splitter3[where];
				}
			}
			
			//if there's a dot and it has the same chapter name
			if ( word_in(splitter2[where], "." ) and word_in(splitter2[where], last_chapt) ){
				//it's a subchapter so we just add it to the same chapter list
				mgfox_all_chapters[chapter].push_back(splitter[1]);
			}
			else {
				//it's a new chapter
				chapter++;
				std::vector<std::string> new_chapt;
				new_chapt.push_back(splitter[1]);
				mgfox_all_chapters.push_back(new_chapt);
			}
		}
	}

#ifdef DEBUG
	for (auto i : mgfox_all_chapters) {
		for(auto j: i) {
			std::cout<<j<<"--";
		}
		std::cout<<"\n";
	}
#endif
}
///=================================================================================///


///=================================================================================///
bool mgfox::chapter_end() 
{
	return ( dlder_current_page-1 == dlder_nb_of_pages);
}
///=================================================================================///


///=================================================================================///
void mgfox::increase_current()
{
	dlder_img  = to_string(atoi(dlder_img.c_str())+1);
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
///=================================================================================///


///=================================================================================///
void mgfox::increase_chapter()
{
	dlder_nb_of_pages          = 0;
	dlder_current_page         = 1;
	dlder_img                  = "000";
	dlder_current_chapter++;
	dlder_current_page         = 2;
}
///=================================================================================///


///=================================================================================///
void mgfox::go_to_next_page()
{
	if (this->chapter_end()){
		this->increase_chapter();
	}
	else {
		this->increase_current();
	}
	mgfox_current_page_url = mgfox_next_link_url;
}
///=================================================================================///


///=================================================================================///
void mgfox::scrap_page() 
{
	//get all lines in the html separately
	std::vector<std::string> response_lines = split(dlder_br.response(), '\n');
	for (auto line: response_lines) {
		//get the image
		if (word_in(line, ".jpg") and word_in(line, "onerror")) {
			std::vector<std::string> splitter = split(line,'"');
			dlder_url_img = splitter[5];
			break;
		}
		else if ( word_in(line, "of ") and word_in(line,"</div")) {
			std::string nb_of_pages = replaceAll2(line, "of ","");
			nb_of_pages = replaceAll2(nb_of_pages, "</div>","");
			nb_of_pages = replaceAll2(nb_of_pages," ","");
			dlder_nb_of_pages = atoi(nb_of_pages.c_str());
		}
	}
#ifdef DEBUG
	std::cout<<dlder_url_img<<"\n";
	std::cout<<dlder_nb_of_pages<<"\n";
#endif
}
///=================================================================================///


///=================================================================================///
std::string mgfox::get_next_link(std::string sub_chapter)
{
	int i;
	for (  i=sub_chapter.length()-1; i>5; i--) {
		if (sub_chapter[i] == '/') {
			break;
		}
		else {
		}
	}
	return sub_chapter.substr(0,i+1) +to_string(dlder_current_page)+".htm";
}
///=================================================================================///


///=================================================================================///
void mgfox::download_chapter(int chapter)
{
	//download all sub-chapters
#ifdef DEBUG
	for (auto sub_chapter: mgfox_all_chapters[chapter]) {
		std::cout<<sub_chapter<<"\n";
	}
#endif
	for( auto sub_chapter: mgfox_all_chapters[chapter] ) {
		//until the whole chapter is downloaded
		mgfox_next_link_url = sub_chapter;
		if (dlder_stop_flag) {
			break;
		}
		else {
			while ( not this->chapter_end()  and not dlder_stop_flag) {
				//open chapter and scrap it for all necessary data
				do {
					dlder_br.open(mgfox_next_link_url,120);
				} while (dlder_br.error());
				scrap_page();
				mgfox_next_link_url = get_next_link(sub_chapter);
				dlder::manage_chapters();
				dlder::download_image();
				increase_current();
			}
		}
		dlder_current_page = 2;
	}
	//reinitialize variables
	dlder_img   = "000";
}
///=================================================================================///


///=================================================================================///
void mgfox::start_downloading()
{
	// if the end > the number of chapters
	if ( atoi(dlder_end.c_str()) > mgfox_all_chapters.size() ) {
		dlder_end = to_string(mgfox_all_chapters.size());
	}
	else {
		//it's ok the end is within the known chapters
	}

	// if the start is the end aka only dl 1 chapter
	if ( dlder_end == dlder_start ) {
		download_chapter( atoi(dlder_start.c_str())-1 );
	}
	else {
		for ( int i= atoi(dlder_start.c_str())-1; i < atoi(dlder_end.c_str()); i++ ) {
			download_chapter(i);
			increase_chapter();
		}
	}
	dlder_stop_flag = true;
}
///=================================================================================///


///=================================================================================///
void mgfox::open_first_page()
{
	do {
		dlder_br.open("http://www.mangafox.me/manga/"+dlder_manganame+"/",120);
	} while(dlder_br.error());
}
///=================================================================================///


///=================================================================================///
void mgfox::run() 
{
	open_first_page();
	scrap_first_page();
	start_downloading();
	std::cout<< "[*] Finished all the downloads\nEnjoy Your Reading!\n";
}
///=================================================================================///



