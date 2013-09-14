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
*
* TODO: If it's 404 return something that specify it doesn't exist.
*       start downloading from a certain page
*
*/

#include <iostream>
#include "Browser/Browser.hpp"
#include "mgstatus.hpp"
#include <vector>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

class mgreader {
	public:
		mgreader(std::string manganame, std::string mg_location,
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
	protected:
	private:
		//vars
		Browser br;
		std::map <std::string,std::string> resolver;
		std::string  _manganame;
		std::string  _mg_location;
		std::string  _external_dl_mgr;
		std::string  _img;
		std::string  _current_page;
		std::string  _next_link;
		std::string  _current_image;
		std::string _start;
		std::string _end;
		bool         _flag;
		bool         _paused;
		unsigned int _START_CHAPT;
		unsigned int _nb_of_pages;
		unsigned int _page_counter;
		//foos
		bool not_published();
		bool chapter_end();
		void scrap_page();
		void fill_in_resolver();
		void create_browser();
		void increase_current();
		void increase_chapter();
		void go_to_next_page();
		void manage_chapters();
		void download_image();
		void autoresolve();
		void start_downloading();
};


mgreader::mgreader(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
{
	fill_in_resolver();
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
	_current_page    ="http://www.mangareader.net/"+ _manganame+"/"+_start +"/";
}


mgreader::mgreader()
{
	fill_in_resolver();
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
	_current_page    ="http://www.mangareader.net/";
}

void
mgreader::init(std::string manganame, std::string mg_location,
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
	_current_page    ="http://www.mangareader.net/"+ _manganame+"/"+_start +"/";
}


mgreader::~mgreader()
{
}

mg_status 
mgreader::get_status()
{
	mg_status stat;
	stat.start_chapter = _START_CHAPT;
	stat.cur_chapter   = atoi(_start.c_str());
	stat.end_chapter   = atoi(_end.c_str());
	stat.cur_page      = _page_counter-1;
	stat.end_page      = _nb_of_pages;
	stat.finished      = _flag;
	return stat;
}

void
mgreader::stop()
{
	_flag = true;
}

void
mgreader::pause_unpause()
{
	if (_paused) _paused = false;
	else         _paused = true;
}

//If manga is not published return True//
bool
mgreader::not_published()
{
	if (br.inresponse("is not published yet. Once") || atoi(_start.c_str()) >= (atoi(_end.c_str())+1) ) {
		std::cout<<"Reached the end\n"; 
		return true;
	}
	else return false;
}

//Reached the end of a chapter
bool
mgreader::chapter_end()
{
	return ( _page_counter-1 == _nb_of_pages);
}

bool
mgreader::finished()
{
	return _flag;
}

//Fill in the resolver map//
void 
mgreader::fill_in_resolver()
{
	resolver["http://i0"]    = "188.132.173.122";
	resolver["http://i1"]    = "188.132.173.3";
	resolver["http://i2"]    = "188.132.173.6";
	resolver["http://i3"]    = "188.132.173.9";
	resolver["http://i4"]    = "188.132.173.12";
	resolver["http://i5"]    = "188.132.173.15";
	resolver["http://i6"]    = "188.132.173.18";
	resolver["http://i7"]    = "188.132.173.21";
	resolver["http://i8"]    = "188.132.173.24";
	resolver["http://i9"]    = "188.132.173.27";
	resolver["http://i10"]   = "188.132.173.30";
	resolver["http://i11"]   = "188.132.173.33";
	resolver["http://i12"]   = "188.132.173.36";
	resolver["http://i13"]   = "188.132.173.39";
	resolver["http://i14"]   = "188.132.173.42";
	resolver["http://i15"]   = "188.132.173.45";
	resolver["http://i16"]   = "188.132.173.48";
	resolver["http://i17"]   = "188.132.173.51";
	resolver["http://i18"]   = "188.132.173.54";
	resolver["http://i19"]   = "188.132.173.57";
	resolver["http://i20"]   = "188.132.173.60";
	resolver["http://i21"]   = "188.132.173.63";
	resolver["http://i22"]   = "188.132.173.66";
	resolver["http://i23"]   = "188.132.173.69";
	resolver["http://i24"]   = "188.132.173.72";
	resolver["http://i25"]   = "188.132.173.75";
	resolver["http://i26"]   = "188.132.173.78";
	resolver["http://i27"]   = "188.132.173.81";
	resolver["http://i28"]   = "188.132.173.84";
	resolver["http://i29"]   = "188.132.173.87";
	resolver["http://i30"]   = "188.132.173.90";
	resolver["http://i31"]   = "188.132.173.93";
	resolver["http://i32"]   = "188.132.173.96";
	resolver["http://i33"]   = "188.132.173.99";
	resolver["http://i34"]   = "188.132.173.126";
	resolver["http://i35"]   = "188.132.173.129";
	resolver["http://i36"]   = "188.132.173.132";
	resolver["http://i37"]   = "188.132.173.135";
	resolver["http://i38"]   = "188.132.173.138";
	resolver["http://i39"]   = "188.132.173.141";
	resolver["http://i40"]   = "188.132.173.144";
	resolver["http://i41"]   = "188.132.173.200";
	resolver["http://i1000"] = "188.132.173.200";
	resolver["http://i999"]  = "188.132.173.12";
	resolver["http://i998"]  = "188.132.173.48";
	resolver["http://i997"]  = "188.132.173.72";
	resolver["http://i996"]  = "188.132.173.96";
	resolver["http://i995"]  = "188.132.173.144";
	resolver["http://i994"]  = "188.132.173.200";
}

void
mgreader::create_browser()
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
mgreader::increase_current()
{
	_current_image = to_string(atoi(_current_image.c_str())+1);
	if (_current_image.size() == 1)
		_current_image = "00"+_current_image;
	else if (_current_image.size() == 2)
		_current_image = "0"+_current_image;
	_page_counter++;
}

void
mgreader::increase_chapter()
{
	_nb_of_pages   = 0;
	_page_counter  = 1;
	_current_image = "000";
	std::ostringstream o;
	_start         = to_string( atoi(_start.c_str())+1);
	_next_link     = "http://www.mangareader.net/"+ _manganame +"/"+ _start +"/"+ to_string( _page_counter);
	_page_counter++;
}

void
mgreader::go_to_next_page()
{
	if (chapter_end())
		increase_chapter();
	else
		increase_current();
	_current_page = _next_link;
}

void
mgreader::scrap_page()
{
	std::vector<std::string> response_lines = split(br.response(), '\n');
	for (unsigned int i=0; i<response_lines.size();i++) {
		if (_nb_of_pages == 0 && word_in(response_lines[i],"</select> of ")) {
			std::string str_nb_page = response_lines[i];
			/* </select> of 18</div> */
			replaceAll(str_nb_page,"</select> of ","");
			replaceAll(str_nb_page,"</div>","");
			_nb_of_pages            = atoi(str_nb_page.c_str());
		}
		if (word_in(response_lines[i],"><img id=\"img\"")) {
			unsigned int index  = response_lines[i].find("src=\"",0);
			unsigned int index2 = response_lines[i].find("\" alt",index+6);
			_img = response_lines[i].substr(index+5,index2-index-5);
			break;
		}
	}
	_next_link = "http://www.mangareader.net/"+ _manganame +"/"+ _start +"/"+ to_string(_page_counter);
}

void 
mgreader::manage_chapters()
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
mgreader::autoresolve()
{
	int it  = _img.find(".",0);
	std::map<std::string, std::string>::iterator map_int;
	map_int = resolver.find(_img.substr(0,it));
	if ( map_int != resolver.end() )
		replaceAll(_img,
			_img.substr(0,it)+".mangareader.net",
			"http://"+map_int->second);
}

void
mgreader::download_image()
{
	autoresolve();
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
			if (_flag) return;
			while (_paused) {
				sleep(3);
			}
			br.write_bytes(_current_image+".jpg");
			br.open( _img,400);
		}while (br.error());
	}
	else {
		int status = 1;
		std::string command = replaceAll2( _external_dl_mgr, "[INPUT]", _img);
		command = replaceAll2( command, "[OUTPUT]", _current_image+".jpg");
		while (status != 0) {
			if (_flag) return;
			while (_paused) {
				sleep(3);
			}
			status = system(command.c_str());
		}
	}
	std::cout<< where_to << "\n";
}

void
mgreader::start_downloading()
{
	do {
		if (_flag) return;
		br.open(_current_page,100);
	} while(br.error());
	if ( !not_published() ) {
		scrap_page();
		manage_chapters();
		download_image();
		go_to_next_page();
	}
	else _flag = true;
}

void
mgreader::run()
{
	while(!_flag)
		start_downloading();
	std::cout<< "[*] Finished all the downloads\nEnjoy Your Reading!";
}

void *
print_stat(void *dl)
{
	mgreader *dl_ = (mgreader* )dl;
	while (!dl_->finished()) {
		mg_status stat =  dl_->get_status();
		std::cout << 
			"start:" << stat.start_chapter << "\n" <<
			"cur  :" << stat.cur_chapter   << "\n" <<
			"end  :" << stat.end_chapter   << "\n" <<
			"c_pag:" << stat.cur_page      << "\n" <<
			"e_pag:" << stat.end_page      << "\n" <<
			"OVER :" << stat.finished      << "\n" ;
		sleep(5);
	}
	return NULL;
}
