/*
Copyright (c) 2013, Patrick Louis <patrick at unixhub.net>
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

Mini version (clean without forms an links) of the browser (Curl Wrapper)
*/

#include "browser.hpp"
#include "regex.hpp"
#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>
#include <sstream>
#include <map>
#include <errno.h>

///==================================CONSTRUCTOR====================================///
browser::browser()
{
	br_header                = "";
	br_html_response         = "";
	br_writing_bytes         = false;
	br_timeout               = 30;
	br_res                   = CURLE_OK;
	br_filepipe              = NULL;
	handle_first_initialization();
}
///=================================================================================///


///====================================DESTRUCTOR===================================///
browser::~browser()
{
	curl_easy_cleanup(br_curl);
	br_history.clear();
	init();
	curl_global_cleanup();
}
///=================================================================================///


///===========initialize the curl handle the very first time========================///
void 
browser::handle_first_initialization()
{
	//get a handler
	curl_global_cleanup();
	br_curl = curl_easy_init();
	//to init all
	curl_global_init(CURL_GLOBAL_ALL);
	//for ssl
	curl_global_init(CURL_GLOBAL_SSL);
	curl_easy_setopt(br_curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
	curl_easy_setopt(br_curl, CURLOPT_NOPROGRESS, 1L);
	//for authentification
	curl_easy_setopt(br_curl, CURLOPT_HTTPAUTH,  CURLAUTH_DIGEST|CURLAUTH_BASIC|CURLAUTH_ANYSAFE);
	curl_easy_setopt(br_curl, CURLOPT_PROXYAUTH,  CURLAUTH_ANYSAFE);
	curl_easy_setopt(br_curl, CURLOPT_COOKIEFILE, "");
	curl_easy_setopt(br_curl, CURLOPT_POSTREDIR, CURL_REDIR_POST_ALL);
#ifdef windows
	//For W$ -- init winsock
	curl_global_init(CURL_GLOBAL_WIN32);
#endif
}
///=================================================================================///


///======================INITIALIZE THE BROWSER AND VARIABLES=======================///
void 
browser::init()
{
	br_header         = "";
	br_html_response  = "";
	br_writing_bytes  = false;
	br_timeout        = 20;
	//maybe we'll loose the cookies if we do that
	//curl            = curl_easy_init();

	struct curl_slist *headers  = NULL;
	headers                     = curl_slist_append(headers, "Accept:");
	curl_slist_free_all(headers);
	curl_easy_reset(br_curl);
}
///=================================================================================///


///============================CLEAN BEFORE CLOSING==================================///
void 
browser::clean()
{
	struct curl_slist *headers	= NULL;
	headers = curl_slist_append(headers, "Accept:");
	curl_slist_free_all(headers);
	br_html_response = "";
	br_history.clear();
	init();
	curl_easy_cleanup(br_curl);
	br_curl   = curl_easy_init();
}
///=================================================================================///


///=================================Close the browser===============================///
void 
browser::close()
{
	br_history.clear();
	init();
}
///=================================================================================///


///==========================SET A USER-AGENT FOR THE BROWSER========================///
void 
browser::add_useragent(std::string user_useragent)
{
	curl_easy_setopt(br_curl, CURLOPT_USERAGENT, user_useragent.c_str());
}
///=================================================================================///


///========================ADD HEADERS WITH OVERLOADING==============================///
void 
browser::add_headers(std::string user_headers[2])
{
	std::string string_head    = user_headers[0]+":"+user_headers[1];
	struct curl_slist *pushed_headers = NULL;
	pushed_headers = curl_slist_append(pushed_headers, string_head.c_str());
	curl_slist_free_all(pushed_headers);
}
void 
browser::add_headers(std::vector<std::string> user_headers)
{
	std::string arr_head[2];
	for(unsigned int i=0; i < user_headers.size(); i+=2) {
		arr_head[0] = user_headers[i];
		arr_head[1] = user_headers[i+1];
		this->add_headers(arr_head);
	}
}
void 
browser::add_headers(std::string left_header,std::string right_header)
{
	std::string arr_head[2] = { left_header,right_header};
	this->add_headers(arr_head);
}
void 
browser::add_headers(std::map<std::string, std::string> user_headers)
{
	std::string arr_head[2];
	for( std::map<std::string,std::string>::iterator ii=user_headers.begin(); ii!=user_headers.end(); ++ii) {
		arr_head[0] = (*ii).first;
		arr_head[1] = (*ii).second;
		this->add_headers(arr_head);
	}
}
///=================================================================================///


///==================THE OPEN PAGE WITH THE OVERLOADING=============================///
void 
browser::open(
	std::string user_url, 
	int user_timeout,
	bool user_save_history, 
	std::string user_post_data="")
{
	init();
	br_timeout = user_timeout;
	//assert(br_timeout>0);
	//set the url in the options
	curl_easy_setopt(br_curl, CURLOPT_URL, user_url.c_str() );
	//set the post if present
	if (user_post_data!="") {
		curl_easy_setopt(br_curl, CURLOPT_POSTFIELDS, user_post_data.c_str());
	}
	else {
	}
	//Handle the response
	if (br_writing_bytes) {
		//assert(br_filepipe!=NULL);
		curl_easy_setopt(br_curl, CURLOPT_WRITEFUNCTION, &browser::write_data);
		curl_easy_setopt(br_curl, CURLOPT_WRITEDATA, br_filepipe);
	}
	else {
		add_headers("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
		add_headers("Connection" ,"keep-alive");
		curl_easy_setopt(br_curl, CURLOPT_WRITEFUNCTION, &browser::write_to_string );
		curl_easy_setopt(br_curl, CURLOPT_WRITEDATA, &br_html_response);
		curl_easy_setopt(br_curl, CURLOPT_WRITEHEADER, &br_header);
		curl_easy_setopt(br_curl, CURLOPT_TIMEOUT,   br_timeout );
	}
	//perform the request
	br_res = curl_easy_perform(br_curl);
	
	if (this->error()) {
		std::cerr<<"\n";
	}
	else {
	}

	if (br_writing_bytes) {
		fclose(br_filepipe);
	}
	else {
	}

	//add current url to the history
	if(user_save_history) {
		br_history.push_back(this->get_url());
	}
	else {
	}
}
void 
browser::open(std::string user_url, int user_timeout)
{
	this->open(user_url, user_timeout, true, "");
}
void 
browser::open(
	std::string user_url, 
	std::string user_post_data, 
	int user_timeout=20)
{
	this->open(user_url, user_timeout, true, user_post_data);
}
void 
browser::open(
	std::string user_url, 
	int user_timeout,
	std::string user_post_data)
{
	this->open(user_url, user_timeout, true, user_post_data);
}
void 
browser::open_no_visit(std::string user_url, int user_timeout=20)
{
	//open without history
	this->open(user_url, user_timeout, false);
}
///=================================================================================///


///===========================HANDLE THE ERROR OUTPUT================================///
bool 
browser::error()
{
	//Check the return code for errors
	if(br_res != CURLE_OK) {
		std::cerr<< "\n[!] There was an error while opening the page : "
			<< curl_easy_strerror(br_res) <<"\n";
		return true;
	}
	else {
		return false;
	}
}
///=================================================================================///


///========================get the status response code=============================///
std::string 
browser::status()
{
	int response_int;
	curl_easy_getinfo(br_curl, CURLINFO_RESPONSE_CODE, &response_int);
	std::stringstream response_stream;
	std::string response_string;
	response_stream<<response_int;
	response_stream>>response_string;
	return response_string;
}
///=================================================================================///


///=======================return the headers info===================================///
std::string 
browser::info()
{
	return br_header;
}
///=================================================================================///


///===========return true if html response is not empty=============================///
bool 
browser::viewing_html()
{
	if (this->response().length()>5) {
		return true;
	}
	else {
		return false;
	}
}
///=================================================================================///


///================return the title of the page <title>TITLE</title>=================///
std::string 
browser::title()
{
	std::vector <std::string> title_container;
	get_after_delimiter(br_html_response,"title",title_container);
	if(title_container.size()>0) {
		int start = title_container[0].find(">")+1;
		int stop  = title_container[0].find("<");
		return title_container[0].substr(start,stop-start);
	}
	else {
		return "";
	}
}
///=================================================================================///


///======================return true if a string is in the title=====================///
bool 
browser::in_title(std::string user_str)
{
	std::string current_title = title();
	if (current_title.find (user_str) != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}
///=================================================================================///


///==============================return the current url=============================///
std::string 
browser::get_url()
{
	char * current_url;
	curl_easy_getinfo(br_curl,CURLINFO_EFFECTIVE_URL, &current_url);
	return current_url;
}
///=================================================================================///


///==============================check if in url====================================///
bool 
browser::in_url(std::string user_str)
{
	std::string current_url = this->get_url();
	if(current_url=="") {
		std::cerr<<"\n[!] No page in history\n";
	}
	else {
	}

	if (current_url.find (user_str) != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}
///=================================================================================///


///=========================LET US SEE THAT RESPONSE=================================///
std::string 
browser::response()
{
	return br_html_response;
}
///=================================================================================///


///====================RETURN TRUE IF THE STRING IS IN REPSONSE=====================///
bool 
browser::in_response(std::string user_str)
{
	if (br_html_response.find (user_str) != std::string::npos) {
		return true;
	}
	else {
		return false;
	}
}
///=================================================================================///


///=======================get all the cookies received in a string===================///
std::string 
browser::get_cookies()
{
	std::string all_cookies = "";
	//set to 0 and not to NULL
	curl_slist * cookies   = 0 ;
	curl_easy_getinfo(br_curl,CURLINFO_COOKIELIST, &cookies);
	curl_slist * cur = cookies;
	while (cur) {
		all_cookies+= cur->data;
		cur        = cur->next;
	}
	curl_slist_free_all(cookies);
	curl_slist_free_all(cur);
	return all_cookies;
}
///=================================================================================///


///========================simply reload the page===================================///
void 
browser::reload()
{
	if (this->get_url().length()>4) {
		std::string current_page = this->get_url();
		//don't reload a page that isn't a page
		//assert(current_page.length()>7);
		this->open(current_page,20);
	}
	else {
		std::cerr<<"\n[!] No page has been open yet\n";
	}
}
///=================================================================================///


///===========================PREPARE FOR WRITING BYTES==============================///
void 
browser::write_bytes(std::string user_filename)
{
	br_filepipe  = fopen(user_filename.c_str(),"wb");
	//assert(br_filepipe!=NULL);
	//an error occured when opening the file for writting
	if(br_filepipe==NULL) {
		std::cerr<< "[!] error writting the file: "
			<< strerror (errno) <<"\n";
	}
	else {
	}

	br_writing_bytes = true;
}
///=================================================================================///


///==========================allow redirection======================================///
void 
browser::set_handle_redirect(bool user_allow)
{
	curl_easy_setopt(br_curl, CURLOPT_FOLLOWLOCATION, user_allow);
	curl_easy_setopt(br_curl, CURLOPT_AUTOREFERER, user_allow);
}
///=================================================================================///


///============================set gzip encoding to true=============================///
void 
browser::set_handle_gzip(bool user_allow)
{
	if(user_allow) {
		curl_easy_setopt(br_curl, CURLOPT_ACCEPT_ENCODING, "gzip");
	}
	else {
		//remember that this one needs curl to be compiled with zlib
		curl_easy_setopt(br_curl, CURLOPT_ACCEPT_ENCODING, "deflate");
	}
	curl_easy_setopt(br_curl, CURLOPT_TRANSFER_ENCODING, user_allow);
}
///=================================================================================///


///=========================to set the verify peer to false=========================///
void 
browser::set_handle_ssl(bool user_allow)
{
	if (user_allow) {
		curl_easy_setopt(br_curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
	}
	else {
		curl_easy_setopt(br_curl, CURLOPT_USE_SSL, CURLUSESSL_NONE);
	}
	curl_easy_setopt(br_curl, CURLOPT_SSL_VERIFYPEER, user_allow);
	curl_easy_setopt(br_curl, CURLOPT_SSL_VERIFYHOST, user_allow);
}
///=================================================================================///


///============================set verbose to true==================================///
void 
browser::set_verbose(bool user_allow)
{
	curl_easy_setopt(br_curl, CURLOPT_VERBOSE, user_allow);
}
///=================================================================================///


///==========================add cookies to the browser==============================///
void 
browser::set_cookie(std::string user_cookies)
{
	curl_easy_setopt(br_curl, CURLOPT_COOKIE, user_cookies.c_str());
}
///=================================================================================///


///========================set the output of the cookies=============================///
void 
browser::set_cookiejar(std::string user_cookiejar)
{
	curl_easy_setopt(br_curl, CURLOPT_COOKIEJAR, user_cookiejar.c_str());
}
void 
browser::set_cookiejar()
{
	curl_easy_setopt(br_curl, CURLOPT_COOKIEJAR, "-");
}
///=================================================================================///


///============================Set a custom dns server===============================///
void 
browser::set_dns(std::string user_dns_server)
{
	curl_easy_setopt(br_curl, CURLOPT_DNS_SERVERS, user_dns_server.c_str());
}
///=================================================================================///


///================Set a proxy (proxy:port) or unset it with the bool false=========///
void 
browser::set_proxy(std::string user_proxy, std::string user_type="http")
{
	int prox_type = CURLPROXY_HTTP;

	if(user_type=="socks4") {
		prox_type = CURLPROXY_SOCKS4;
	}
	else if(user_type=="socks5") {
		prox_type = CURLPROXY_SOCKS5;
	}
	else if(user_type=="socks4a") {
		prox_type = CURLPROXY_SOCKS4A;
	}
	else {
	}

	curl_easy_setopt(br_curl, CURLOPT_PROXY, user_proxy.c_str());
	curl_easy_setopt(br_curl, CURLOPT_PROXYTYPE, prox_type );
}
void 
browser::set_proxy(bool user_allow)
{
	if(user_allow) {
		std::cerr<<"\n[!] Need to pass the arguments \n";
	}
	else {
		curl_easy_setopt(br_curl, CURLOPT_PROXY, "" );
	}
}
///=================================================================================///


///======================proxy Login identification=================================///
void 
browser::set_proxy_login(std::string user_username, std::string user_passwd)
{
	std::string concat = user_username+":"+user_passwd;
	curl_easy_setopt(br_curl, CURLOPT_PROXYUSERPWD, concat.c_str());
}
///=================================================================================///


///======================HTTP tunneling through http proxy===========================///
void 
browser::set_http_tunel(bool user_allow)
{
	curl_easy_setopt(br_curl, CURLOPT_HTTPPROXYTUNNEL, user_allow);
}
///=================================================================================///


///===========Set the interface to use with a port or maybe a port range============///
void 
browser::set_interface(
	std::string user_interface_name, 
	long int user_port=80, 
	long int user_max_port=80)
{
		curl_easy_setopt(br_curl, CURLOPT_INTERFACE, user_interface_name.c_str() );
		curl_easy_setopt(br_curl, CURLOPT_LOCALPORT, user_port );
		if(user_port<80) {
			std::cerr<<"\n[!] Remember: With great power comes great responsabilities\n";
		}
		else {
			user_max_port = user_port + user_max_port-user_port+1;
			curl_easy_setopt(br_curl, CURLOPT_LOCALPORTRANGE, user_max_port );
		}
}
///=================================================================================///


///===========================Use HTTP 1.0 instead of 1.1===========================///
void 
browser::set_http_version_1_0(bool user_set_it)
{
	if(user_set_it) {
		curl_easy_setopt(br_curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
	}
	else {
		curl_easy_setopt(br_curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
	}
}
///=================================================================================///


///===========================do a header only request===============================///
void 
browser::head_request(bool user_allow)
{
	curl_easy_setopt(br_curl, CURLOPT_NOBODY, user_allow);
}
///=================================================================================///


///===========================Show the current history===============================///
void 
browser::history()
{
	for(unsigned int ii=0;ii<br_history.size();ii++) {
		std::cout<<br_history[ii]<<"\n";
	}
}
///=================================================================================///


///=========================Clear the history accumulated over time==================///
void 
browser::clear_history()
{
	br_history.clear();
}
///=================================================================================///


///============================Go back in history===================================///
void 
browser::back(int user_timeout=20)
{
	//assert(br_history.size()>0);
	//remove the last page
	//visit the page before the last page we opened
	//but don't save that we visited it
	br_history.pop_back();
	open(br_history[br_history.size()-1],user_timeout,false);
}
///=================================================================================///


///======================OPTIONS RELATED TO THE CONGESTION==========================///
//in bytes normally, now * 1000 so in kbs
void 
browser::limit_speed(int user_limit)
{
	//assert(user_limit>0 && user_limit<30000);
	user_limit = user_limit *1000;
	if(user_limit>0 && user_limit<30000) {
		curl_easy_setopt(br_curl, CURLOPT_LOW_SPEED_LIMIT, user_limit);
		curl_easy_setopt(br_curl, CURLOPT_MAX_SEND_SPEED_LARGE, user_limit);
		curl_easy_setopt(br_curl, CURLOPT_MAX_RECV_SPEED_LARGE, user_limit);
	}
	else {
		std::cerr<<"\n[!] Can't set the time limit \n";
	}
}
//relevant only if limit_speed is set up
//in seconds
void 
browser::limit_time(int user_limit)
{
	//assert(user_limit>0);
	if(user_limit>0) {
		curl_easy_setopt(br_curl, CURLOPT_LOW_SPEED_TIME, user_limit);
	}
	else {
		std::cerr<<"\n[!] Can't set the limit \n";
	}
}
///=================================================================================///


///=============================Escape/Unescape strings==============================///
std::string 
browser::escape(std::string user_the_string)
{
	std::string output;
	output = curl_easy_escape( br_curl, user_the_string.c_str(), 0 );
	return output;
}
std::string 
browser::unescape(std::string user_the_string)
{
	std::string output;
	output = curl_easy_escape( br_curl, user_the_string.c_str(), 0 );
	return output;
}
///=================================================================================///


///======Get the first root of the current  url starting from right or from left====///
std::string 
browser::get_first_root(bool user_start_from_host)
{
	std::string temp_url   ="";
	int backward_it        = 1;
	temp_url               = this->get_url();
	bool https             = false;
	// meaning we just need to add the docuement to the hostname.
	//remove the http:// to not confuse the slashes
	replaceAll(temp_url,"http://","");
	if( word_in(temp_url,"https://") ) {
		https = true;
		replaceAll(temp_url,"https://","");
	}
	else {
	}

	//now test if we are in a directory
	//meaning something like:
	//www.something.com/   or
	//www.somthing.com/blah.php or
	//www.something.com/else/somthing.php
	if( word_in(temp_url,"/")) {
		//just find the first / in the temp url
		if (user_start_from_host) {
			backward_it = temp_url.find("/");
			if (!https) {
				temp_url = "http://"+ temp_url.substr(0,backward_it+1);
			}
			else {
				temp_url = "https://"+temp_url.substr(0,backward_it+1);
			}
			if (temp_url[temp_url.size()-1]=='/') {
				return temp_url.substr(0,temp_url.size()-1);
			}
			else {
				return temp_url;
			}
		}
		else {
			while(temp_url[temp_url.size()-backward_it]!='/') {
				backward_it++;
			}
			//here we are on the last slash
			//prepare the first root URL for the form
			if(!https) {
				temp_url = "http://" + temp_url.substr(0,temp_url.size()-backward_it);
			}
			else {
				temp_url = "https://" + temp_url.substr(0,temp_url.size()-backward_it);
			}
		}
	}
	//meaning we don't have any slash, we are in the top
	//dir , so something like:
	//www.blahblah.com
	else {
		//here we concatenate all we need in this way:
		//http://www.blahblah.com/formurl.php
		if(!https) {
			temp_url = "http://" + temp_url + "/";
		}
		else {
			temp_url = "https://" + temp_url + "/";
		}
	}
	return temp_url;
}
///=================================================================================///


///===========return a curl handle to use outside of the class======================///
CURL*
browser::get_handle()
{
	return &br_curl;
}
///=================================================================================///


//===============SAVE THE HTML OUTPUT INTO THE RESPONSE STRING=====================///
size_t 
browser::write_to_string(
	void* curl, 
	size_t size, 
	size_t count, 
	void* response)
{
  ((std::string*)response)->append((char*)curl, 0, size*count);
  return size*count;
}
///=================================================================================///


///====================SAVE THE OUTPUT IN A BIN FILE================================///
size_t 
browser::write_data(
	void* ptr, 
	size_t size, 
	size_t nmemb, 
	FILE* stream)
{
	size_t written;
	written = fwrite(ptr, size, nmemb, stream);
	return written;
}
///=================================================================================///


