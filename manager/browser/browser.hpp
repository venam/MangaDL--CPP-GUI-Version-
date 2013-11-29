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

#ifndef MECHANIZE_HPP_INCLUDED
#define MECHANIZE_HPP_INCLUDED

#include <curl/curl.h>
#include <iostream>
#include <cstring>
#include <assert.h>
#include <vector>
#include <sstream>
#include <map>
#include <errno.h>

bool remove_html_comment(std::string & html_response);
void remove_html_comments(std::string & html_response);
void lower_it(const std::string &income, std::string & outcome);
void upper_it(const std::string &income, std::string & outcome);
bool word_in(const std::string &the_string, const std::string &to_search);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
std::string replaceAll2(std::string str, const std::string& from, const std::string& to);
void get_after_delimiter(std::string html_response, std::string seeking, std::vector <std::string> &form_container);
std::string  to_string(unsigned int integer);
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);


///==================================THE BROWSER CLASS==============================///
class browser {
	private:

		std::string               br_header;
		std::string               br_html_response;
		std::vector <std::string> br_history;
		bool                      br_writing_bytes;
		int                       br_timeout;
		CURLcode                  br_res;
		FILE*                     br_filepipe;
		CURL*                     br_curl;

		static size_t write_to_string(void *curl, size_t size, size_t count, void *response);
		static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);

	public:
		browser();
		~browser();
		void handle_first_initialization();
		void init();
		void clean();
		void close();
		void add_useragent(std::string user_useragent);
		void add_headers(std::string user_headers[2]);
		void add_headers(std::vector<std::string> user_headers);
		void add_headers(std::string left_header,std::string second_right_header);
		void add_headers(std::map<std::string, std::string> user_headers);
		
		void open(std::string user_url, int user_timeout,bool user_save_history, std::string user_post_data);
		void open(std::string user_url, int user_timeout);
		void open(std::string user_url, std::string user_post_data, int user_timeout);
		void open(std::string user_url, int user_timeout,std::string user_post_data);
		void open_no_visit(std::string user_url, int user_timeout);

		bool error();
		std::string status();
		std::string info();
		bool viewing_html();
		std::string title();
		bool in_title(std::string user_str);
		std::string get_url();
		bool in_url(std::string user_str);
		std::string response();
		bool in_response(std::string user_str);
		std::string get_cookies();
		void reload();
		void write_bytes(std::string user_filename);
		void set_handle_redirect(bool user_allow);
		void set_handle_gzip(bool user_allow);
		void set_handle_ssl(bool allow);
		void set_verbose(bool allow);
		void set_cookie(std::string cookies);
		void set_cookiejar(std::string user_cookiejar);
		void set_cookiejar();
		void set_dns(std::string user_dns_server);
		void set_proxy(std::string user_proxy, std::string user_type);
		void set_proxy(bool user_allow);
		void set_proxy_login(std::string user_username, std::string user_passwd);
		void set_http_tunel(bool user_allow);
		void set_interface(std::string user_interface_name, long int user_port, long int user_max_port);
		void set_http_version_1_0(bool user_set_it);
		void head_request(bool user_allow);
		void history();
		void clear_history();
		void back(int user_timeout);
		void limit_speed(int user_limit);
		void limit_time(int user_limit);
		std::string escape(std::string user_the_string);
		std::string unescape(std::string user_the_string);
		std::string get_first_root(bool user_start_from_host);
		CURL *get_handle();
};
///=================================================================================///



#endif // MECHANIZE_HPP_INCLUDED

