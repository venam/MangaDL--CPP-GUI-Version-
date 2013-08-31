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

#include <iostream>
#include "Browser/Browser.hpp"
#include <vector>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

class mgpark {
	public:
		mgpark(std::string manganame, std::string mg_location,
				std::string external_dl_mgr,std::string start, std::string end);
		mgpark();
		~mgpark();
		void run();
		mg_status get_status();
		void pause_unpause();
		bool finished();
		void stop();
		void init(std::string manganame, std::string mg_location,
			std::string external_dl_mgr,std::string start, std::string end);
	protected:
	private:
		//vars
		Browser br;
		std::vector  <std::string>_imgs;
		std::vector  <std::string>_chapters;
		std::string  _manganame;
		std::string  _mg_location;
		std::string  _external_dl_mgr;
		std::string  _img;
		std::string  _next_link;
		std::string  _current_image;
		std::string  _start;
		std::string  _end;
		bool         _flag;
		bool         _paused;
		unsigned int _START_CHAPT;
		unsigned int _nb_of_pages;
		unsigned int _page_counter;
		//foos
		void scrap_page();
		void scrap_images();
		void create_browser();
		void increase_current();
		void increase_chapter();
		void manage_chapters();
		void download_image();
		void download_each_imgs();
		void start_downloading();
		void open_new_chapter(int chapt);
		void open_first_page();
};

mgpark::mgpark(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
{
	_manganame       = manganame;
	_mg_location     = mg_location;
	_external_dl_mgr = external_dl_mgr;
	_start           = start;
	_START_CHAPT     = atoi(_start.c_str());
	_end             = end;
	_current_image   = "000";
	_img             = "";
	_next_link       = "";
	_nb_of_pages     = 0;
	_page_counter    = 2;
	_flag            = false;
	_paused          = false;
}


mgpark::mgpark()
{
	_manganame       = "";
	_mg_location     = "";
	_external_dl_mgr = "";
	_start           = "";
	_START_CHAPT     = atoi(_start.c_str());
	_end             = "";
	_current_image   = "000";
	_img             = "";
	_next_link       = "";
	_nb_of_pages     = 0;
	_page_counter    = 2;
	_flag            = false;
	_paused          = false;
}

void
mgpark::init(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
{
	_manganame       = manganame;
	_mg_location     = mg_location;
	_external_dl_mgr = external_dl_mgr;
	_start           = start;
	_START_CHAPT     = atoi(_start.c_str());
	_end             = end;
	_current_image   = "000";
	_img             = "";
	_next_link       = "";
	_nb_of_pages     = 0;
	_page_counter    = 2;
	_flag            = false;
	_paused          = false;
	_imgs.clear();
	_chapters.clear();
}


mgpark::~mgpark()
{
}

mg_status 
mgpark::get_status()
{
	mg_status stat;
	stat.start_chapter = _START_CHAPT;
	stat.cur_chapter   = atoi(_start.c_str());
	stat.end_chapter   = atoi(_end.c_str());
	stat.cur_page      = _page_counter-1;
	stat.end_page      = _nb_of_pages+1;
	stat.finished      = _flag;
	return stat;
}

void
mgpark::stop()
{
	_flag = true;
}

void
mgpark::pause_unpause()
{
	if (_paused) _paused = false;
	else         _paused = true;
}

bool
mgpark::finished()
{
	return _flag;
}

void
mgpark::create_browser()
{
	br.adduseragent("Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008071615 Fedora/3.0.1-1.fc9 Firefox/3.0.1");
	std::string header[2] = {"Referer","http://www.mangareader.net"};
	br.addheaders(header);
	br.set_handle_redirect(false);
	br.set_handle_gzip(false);
	br.set_handle_ssl(false);
	br.set_verbose(false);
	br.fetch_links(false);
	br.fetch_forms(false);
}

void 
mgpark::increase_current()
{
	_current_image = to_string(atoi(_current_image.c_str())+1);
	if (_current_image.size() == 1)
		_current_image = "00"+_current_image;
	else if (_current_image.size() == 2)
		_current_image = "0"+_current_image;
	_page_counter++;
}

void
mgpark::increase_chapter()
{
	_nb_of_pages   = 0;
	_page_counter  = 1;
	_current_image = "000";
	std::ostringstream o;
	_start         = to_string( atoi(_start.c_str())+1);
	_page_counter++;
}

void
mgpark::scrap_page()
{
	std::vector<std::string> response_lines = split(br.response(), '\n');
	//loop backward so there will be no need to reverse the vector later on
	for (int i=response_lines.size()-1; i>=0;i--) {
		//a new chapter has been found
		if (word_in(response_lines[i],">All</a>")) {
			std::vector<std::string> url_split = split(response_lines[i], '\"');
			std::string url = url_split[1];
			int manga_name_place = url.find(_manganame);
			std::string tmp = url.substr(manga_name_place+_manganame.size(),url.size()-1);
			if ( !word_in( tmp,"-" ) || !word_in( tmp, "+") || !word_in( tmp, ".") )
				_chapters.push_back( "http://www.mangapark.com"+ url);
		}
	}
	if ( _chapters.size() == 0 )
		std::cout<<"No Chapter for this manga\n";
}

void
mgpark::scrap_images()
{
	std::vector<std::string> response_lines;
	response_lines = split(br.response(), '\n');
	for (unsigned int i=0;i<response_lines.size();i++) {
		//this line contains an image
		if (word_in(response_lines[i], "<em><a target=\"_blank\"") ) {
			std::vector<std::string> splitter = split(response_lines[i], '\"');
			_imgs.push_back(splitter[3]);
		}
	}
	_nb_of_pages = _imgs.size() -1;
}

void 
mgpark::manage_chapters()
{
	if (0 != access( _mg_location.c_str() , W_OK & R_OK ))
		mkdir( _mg_location.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	chdir(_mg_location.c_str());
	if (0 != access( _manganame.c_str() , W_OK & R_OK ))
		mkdir( _manganame.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	chdir(_manganame.c_str());
	std::string chapter_dir = _manganame+"-"+_start;
	if (0 != access(chapter_dir.c_str(), W_OK & R_OK ))
		mkdir( chapter_dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	chdir(chapter_dir.c_str());
}

void
mgpark::download_each_imgs()
{
	manage_chapters();
	for (int i=0;i<_imgs.size();i++) {
		_img = _imgs[i];
		if ( _flag ) return;
		while (_paused) {
			sleep(3);
		}
		download_image();
		increase_current();
	}
}

void
mgpark::download_image()
{
	if (_flag) return;
	while (_paused) {
		sleep(3);
	}
	std::string where_to;
	if (_mg_location[_mg_location.size()-1]=='/')
		 where_to = _mg_location.substr(0,_mg_location.size()-2)+"/"+_manganame+"/" +_manganame+"-"+_start+"/"+_current_image+".jpg";
	else where_to = _mg_location+"/"+_manganame+"/" +_manganame+"-"+_start+"/"+_current_image+".jpg";
	if ( _external_dl_mgr == "default" ) {
		do {
			br.write_bytes(_current_image+".jpg");
			br.open( _img,400);
		}while (br.error());
	}
	else {
		int status = 1;
		std::string command = replaceAll2( _external_dl_mgr, "[INPUT]", _img);
		command = replaceAll2( command, "[OUTPUT]", _current_image+".jpg");
		while (status != 0) {
			status = system(command.c_str());
		}
	}
	std::cout<< where_to << "\n";
}

void
mgpark::open_new_chapter(int chapt)
{
	do {
		std::cout<<_chapters[chapt]<<"\n";
		br.open(_chapters[chapt]);
	} while( br.error() );
}

void
mgpark::start_downloading()
{
	if ( atoi(_end.c_str()) > _chapters.size() )
		_end = to_string(_chapters.size());

	if ( _end == _start ) {
		open_new_chapter( atoi(_start.c_str() ) );
		scrap_images();
		download_each_imgs();
		_current_image = "000";
		_imgs.clear();
		_flag = true;
		return;
	}

	for ( 
			int i= atoi(_start.c_str())-1; 
			i < atoi(_end.c_str()); 
			i++ 
		) {
		open_new_chapter(i);
		scrap_images();
		download_each_imgs();
		_current_image = "000";
		_imgs.clear();
		increase_chapter();
	}
	_flag = true;
}

void
mgpark::open_first_page()
{
	do {
		br.open("http://www.mangapark.com/manga/"+_manganame,100);
	} while(br.error());
}

void
mgpark::run()
{
	open_first_page();
	scrap_page();
	start_downloading();
	std::cout<< "[*] Finished all the downloads\nEnjoy Your Reading!\n";
}
