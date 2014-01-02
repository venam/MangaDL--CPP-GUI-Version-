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

Needs some error handling for the connection and download image function

*
*/


#include "dlder.hpp"


///=================================================================================///
dlder::dlder(
		std::string manganame, std::string mg_location,
		std::string external_dl_mgr,std::string start, std::string end)
{
	dlder_manganame       = manganame;
	dlder_mg_location     = mg_location;
	dlder_external_dl_mgr = external_dl_mgr;
	dlder_start           = start;
	dlder_end             = end;
	dlder_img             = "000";
	dlder_nb_of_chapter   = 0;
	dlder_current_chapter = atoi(start.c_str());
	dlder_nb_of_pages     = 0;
	dlder_current_page    = 1;
	dlder_paused_flag     = false;
	dlder_stop_flag       = false;
	create_browser();
}
dlder::dlder()
{
	dlder_manganame       = "";
	dlder_mg_location     = "";
	dlder_external_dl_mgr = "";
	dlder_start           = "";
	dlder_end             = "";
	dlder_img             = "000";
	dlder_nb_of_chapter   = 0;
	dlder_current_chapter = 1;
	dlder_nb_of_pages     = 0;
	dlder_current_page    = 1;
	dlder_paused_flag     = false;
	dlder_stop_flag       = false;
	create_browser();
}
dlder::~dlder()
{
}
///=================================================================================///


///=================================================================================///
void
dlder::init(std::string manganame, std::string mg_location,
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
	create_browser();
}
///=================================================================================///

///=================================================================================///
mg_status dlder::get_status()
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
void dlder::pause_unpause()
{
	dlder_paused_flag = dlder_paused_flag? false: true;
}
///=================================================================================///


///=================================================================================///
bool dlder::finished()
{
	return dlder_stop_flag;
}
///=================================================================================///


///=================================================================================///
void dlder::stop()
{
	dlder_stop_flag = true;
}
///=================================================================================///


///================================initialize the browser===========================///
void 
dlder::create_browser()
{
	dlder_br.add_useragent("Mozilla/5.0 (X11; U; Linux i686; en-US; rv:1.9.0.1) Gecko/2008071615 Fedora/3.0.1-1.fc9 Firefox/3.0.1");
	std::string header[2] = {"Referer","http://www.mangareader.net"};
	dlder_br.add_headers(header);
	dlder_br.set_handle_redirect(true);
	dlder_br.set_handle_gzip(true);
	dlder_br.set_handle_ssl(false);
	dlder_br.set_verbose(false);
}
///=================================================================================///


///===========Moves to the appropriate dir (creating it if needed)==================///
void 
dlder::manage_chapters()
{
	if (0 != access( dlder_mg_location.c_str() , W_OK & R_OK )) {
		mkdir( dlder_mg_location.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	else {
	}
	chdir(dlder_mg_location.c_str());
	if (0 != access( dlder_manganame.c_str() , W_OK & R_OK )) {
		mkdir( dlder_manganame.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	else {
	}
	chdir(dlder_manganame.c_str());
	std::string chapter_dir = dlder_manganame+"-"+to_string(dlder_current_chapter);
	if (0 != access(chapter_dir.c_str(), W_OK & R_OK )) {
		mkdir( chapter_dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	else {
	}
	chdir(chapter_dir.c_str());
}
///=================================================================================///


///=================================================================================///
void 
dlder::download_image()
{
	if (dlder_stop_flag) {
		return;
	}
	else {
		while (dlder_paused_flag) {
			sleep(3);
		}
		std::string where_to;
		if (dlder_mg_location[dlder_mg_location.size()-1]=='/') {
			where_to = 
				dlder_mg_location.substr(0,dlder_mg_location.size()-2) 
				+ "/"+dlder_manganame 
				+ "/" +dlder_manganame + "-"+to_string(dlder_current_chapter)
				+ "/"+dlder_img+ ".jpg";
		}
		else  { 
			where_to = 
				dlder_mg_location 
				+ "/"+dlder_manganame 
				+ "/"+dlder_manganame + "-"+to_string(dlder_current_chapter)  
				+ "/"+dlder_img + ".jpg";
		}
		if ( dlder_external_dl_mgr == "default" ) {
			do {
				dlder_br.write_bytes(dlder_img+".jpg");
				dlder_br.open( dlder_url_img,400);
			}while (dlder_br.error());
		}
		else {
			int status = 1;
			int times  = 1 ;
			int sleep_time = 1;
			std::string command =  command_converter();
			while (status != 0) {
				status = system(command.c_str());
				if (status !=0) {
					times++;
					//after 10 errors sleep 10 more seconds
					if (times%10 ==0) {
#ifdef DEBUG
						std::cerr<<"too many error adding 10s to sleep\n";
#endif
						sleep_time+=10;
					}
					else {
					}
					sleep(sleep_time);
				}
			}
		}
		std::cout<< where_to << "\n";
	}
}
///=================================================================================///


///=================================================================================///
std::string 
dlder::command_converter()
{
	std::string command = replaceAll2( dlder_external_dl_mgr, "[INPUT]", dlder_url_img);
	command             = replaceAll2( command, "[OUTPUT]", dlder_img+".jpg");
	std::cout<<command<<"\n";
	return command;
}
///=================================================================================///


