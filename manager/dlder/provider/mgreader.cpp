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

#include "mgreader.hpp"


///=================================================================================///
mgreader::mgreader(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
	: dlder( manganame, mg_location, external_dl_mgr, start, end)
{
	fill_in_resolver();
	mgreader_current_page_url     ="http://www.mangareader.net/"+ dlder_manganame+"/"+dlder_start +"/";
	//vars
	mgreader_next_link        = "";
}
///=================================================================================///


///=================================================================================///
mgreader::mgreader() : dlder()
{
	fill_in_resolver();
	mgreader_current_page_url    ="http://www.mangareader.net/";
	//vars
	mgreader_next_link        = "";


}
///=================================================================================///


///=================================================================================///
void
mgreader::init(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end) 
{
	dlder::init(manganame,mg_location,external_dl_mgr, start, end);
	mgreader_current_page_url     ="http://www.mangareader.net/"+ dlder_manganame+"/"+dlder_start +"/";
	std::cout<<mgreader_current_page_url<<"\n";
}
///=================================================================================///


///=================================================================================///
mgreader::~mgreader()
{
}
///=================================================================================///


///=================================================================================///
mg_status 
mgreader::get_status()
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
void
mgreader::stop()
{
	dlder_stop_flag = true;
}
///=================================================================================///


///=================================================================================///
void
mgreader::pause_unpause()
{
	dlder_paused_flag = dlder_paused_flag? false: true;
}
///=================================================================================///


///=================================================================================///
//If manga is not published return True//
bool
mgreader::not_published()
{
	if (
		dlder_br.in_response("is not published yet. Once") || 
		dlder_current_chapter >= (atoi(dlder_end.c_str())+1) ) {

		std::cout<<"Reached the end\n"; 
		return true;
	}
	else {
		return false;
	}
}
///=================================================================================///


///=================================================================================///
//Reached the end of a chapter
bool
mgreader::chapter_end()
{
	return ( dlder_current_page-1 == dlder_nb_of_pages);
}
///=================================================================================///


///=================================================================================///
bool
mgreader::finished()
{
	return dlder_stop_flag;
}
///=================================================================================///


///=================================================================================///
//Fill in the resolver map//
void 
mgreader::fill_in_resolver()
{
	mgreader_resolver["http://i0"]    = "188.132.173.122";
	mgreader_resolver["http://i1"]    = "188.132.173.3";
	mgreader_resolver["http://i2"]    = "188.132.173.6";
	mgreader_resolver["http://i3"]    = "188.132.173.9";
	mgreader_resolver["http://i4"]    = "188.132.173.12";
	mgreader_resolver["http://i5"]    = "188.132.173.15";
	mgreader_resolver["http://i6"]    = "188.132.173.18";
	mgreader_resolver["http://i7"]    = "188.132.173.21";
	mgreader_resolver["http://i8"]    = "188.132.173.24";
	mgreader_resolver["http://i9"]    = "188.132.173.27";
	mgreader_resolver["http://i10"]   = "188.132.173.30";
	mgreader_resolver["http://i11"]   = "188.132.173.33";
	mgreader_resolver["http://i12"]   = "188.132.173.36";
	mgreader_resolver["http://i13"]   = "188.132.173.39";
	mgreader_resolver["http://i14"]   = "188.132.173.42";
	mgreader_resolver["http://i15"]   = "188.132.173.45";
	mgreader_resolver["http://i16"]   = "188.132.173.48";
	mgreader_resolver["http://i17"]   = "188.132.173.51";
	mgreader_resolver["http://i18"]   = "188.132.173.54";
	mgreader_resolver["http://i19"]   = "188.132.173.57";
	mgreader_resolver["http://i20"]   = "188.132.173.60";
	mgreader_resolver["http://i21"]   = "188.132.173.63";
	mgreader_resolver["http://i22"]   = "188.132.173.66";
	mgreader_resolver["http://i23"]   = "188.132.173.69";
	mgreader_resolver["http://i24"]   = "188.132.173.72";
	mgreader_resolver["http://i25"]   = "188.132.173.75";
	mgreader_resolver["http://i26"]   = "188.132.173.78";
	mgreader_resolver["http://i27"]   = "188.132.173.81";
	mgreader_resolver["http://i28"]   = "188.132.173.84";
	mgreader_resolver["http://i29"]   = "188.132.173.87";
	mgreader_resolver["http://i30"]   = "188.132.173.90";
	mgreader_resolver["http://i31"]   = "188.132.173.93";
	mgreader_resolver["http://i32"]   = "188.132.173.96";
	mgreader_resolver["http://i33"]   = "188.132.173.99";
	mgreader_resolver["http://i34"]   = "188.132.173.126";
	mgreader_resolver["http://i35"]   = "188.132.173.129";
	mgreader_resolver["http://i36"]   = "188.132.173.132";
	mgreader_resolver["http://i37"]   = "188.132.173.135";
	mgreader_resolver["http://i38"]   = "188.132.173.138";
	mgreader_resolver["http://i39"]   = "188.132.173.141";
	mgreader_resolver["http://i40"]   = "188.132.173.144";
	mgreader_resolver["http://i41"]   = "188.132.173.200";
	mgreader_resolver["http://i1000"] = "188.132.173.200";
	mgreader_resolver["http://i999"]  = "188.132.173.12";
	mgreader_resolver["http://i998"]  = "188.132.173.48";
	mgreader_resolver["http://i997"]  = "188.132.173.72";
	mgreader_resolver["http://i996"]  = "188.132.173.96";
	mgreader_resolver["http://i995"]  = "188.132.173.144";
	mgreader_resolver["http://i994"]  = "188.132.173.200";
}
///=================================================================================///


///=================================================================================///
void 
mgreader::increase_current()
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
void
mgreader::increase_chapter()
{
	dlder_nb_of_pages          = 0;
	dlder_current_page         = 1;
	dlder_img                  = "000";
	dlder_current_chapter      = dlder_current_chapter+1;
	mgreader_current_page_url  = "http://www.mangareader.net/"+ dlder_manganame +"/"+ to_string(dlder_current_chapter) +"/"+ to_string( dlder_current_page);
	dlder_current_page++;
}
///=================================================================================///


///=================================================================================///
void
mgreader::go_to_next_page()
{
	if (this->chapter_end()){
		this->increase_chapter();
	}
	else {
		this->increase_current();
	}
	mgreader_current_page_url = mgreader_next_link;
}
///=================================================================================///


///=================================================================================///
void
mgreader::scrap_page()
{
	std::vector<std::string> response_lines = split(dlder_br.response(), '\n');
	for (unsigned int i=0; i<response_lines.size();i++) {
		if (dlder_nb_of_pages == 0 && word_in(response_lines[i],"</select> of ")) {
			std::string str_nb_page = response_lines[i];
			/* </select> of 18</div> */
			replaceAll(str_nb_page,"</select> of ","");
			replaceAll(str_nb_page,"</div>","");
			dlder_nb_of_pages   = atoi(str_nb_page.c_str());
		}
		else {
		}

		if (word_in(response_lines[i],"><img id=\"img\"")) {
			unsigned int index  = response_lines[i].find("src=\"",0);
			unsigned int index2 = response_lines[i].find("\" alt",index+6);
			dlder_url_img = response_lines[i].substr(index+5,index2-index-5);
			std::cout<<"-----";
			std::cout<<dlder_url_img<<"\n";
			break;
		}
		else {
		}
	}
	mgreader_next_link = "http://www.mangareader.net/"+ dlder_manganame +"/"+ dlder_start +"/"+ to_string(dlder_current_page);
}
///=================================================================================///


///=================================================================================///
void
mgreader::autoresolve()
{
	int it  = dlder_url_img.find(".",0);
	std::map<std::string, std::string>::iterator map_int;
	map_int = mgreader_resolver.find(dlder_url_img.substr(0,it));
	if ( map_int != mgreader_resolver.end() )
		replaceAll(dlder_url_img,
			dlder_url_img.substr(0,it)+".mangareader.net",
			"http://"+map_int->second);
}
///=================================================================================///


///=================================================================================///
void
mgreader::download_image()
{
	autoresolve();
	dlder::download_image();
}
///=================================================================================///


///=================================================================================///
void
mgreader::start_downloading()
{
	do {
		dlder_br.open(mgreader_current_page_url,100);
	} while(dlder_br.error());
	if ( !not_published() ) {
		scrap_page();
		manage_chapters();
		download_image();
		go_to_next_page();
	}
	else {
		dlder_stop_flag = true;
	}
}
///=================================================================================///


///=================================================================================///
void
mgreader::run()
{
	while(!dlder_stop_flag) {
		start_downloading();
	}
	std::cout<< "[*] Finished all the downloads\nEnjoy Your Reading!";
}
///=================================================================================///


