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
*/

#ifndef REGEX_HPP_INCLUDED
#define REGEX_HPP_INCLUDED

#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <sstream>

bool remove_html_comment(std::string & html_response);
void remove_html_comments(std::string & html_response);
void lower_it(const std::string &income, std::string & outcome);
void upper_it(const std::string &income, std::string & outcome);
bool word_in(const std::string &the_string, const std::string &to_search);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
std::string replaceAll2(std::string str, const std::string& from, const std::string& to);
void get_after_delimiter(std::string html_response, std::string seeking, std::vector <std::string> &form_container);
std::string  to_string(unsigned int integer);

///========================Conver a u integer to string==============================///
std::string 
to_string(unsigned int integer)
{
	std::ostringstream o;
	o << integer;
	return o.str();
}
///==================================================================================///

///===========================SPLIT A STRING FROM A DELIMITER========================///
std::vector<std::string>& 
split(
	const std::string& s, 
	char delim, 
	std::vector<std::string>& elems) 
{
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> 
split( const std::string& s, char delim) 
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}
///=================================================================================///

///==============================UPPER IT FROM STRING===============================///
void 
upper_it(const std::string& income, std::string& outcome)
{
	char c;
	int i=0;
	while (income[i]) {
		c        = income[i];
		outcome += toupper(c);
		i++;
	}
}
///=================================================================================///

///==============================LOWER IT FROM STRING===============================///
void 
lower_it(const std::string& income, std::string& outcome)
{
	char c;
	int i=0;
	while (income[i]) {
		c        = income[i];
		outcome += tolower(c);
		i++;
	}
}
///=================================================================================///

///===============================SEARCH FOR A WORD (any)===========================///
bool 
word_in(const std::string& the_string, const std::string& to_search)
{
	std::string temp;
	lower_it(the_string,temp);
	return (  temp.find(to_search)!=std::string::npos ||
			the_string.find(to_search)!=std::string::npos);
}
///=================================================================================///

///=================REPLACE SOMETHING IN A STRING WITH SOMETHING ELSE===============///
void 
replaceAll(
	std::string& str, 
	const std::string& from, 
	const std::string& to)
{
	if(from.empty()) {
		return;
	}
	else {
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
	}
}
///=================================================================================///

///=================REPLACE SOMETHING IN A STRING WITH SOMETHING ELSE===============///
std::string 
replaceAll2(
	std::string str, 
	const std::string& from, 
	const std::string& to)
{
	if(from.empty()) {
		return "";
	}
	else {
		size_t start_pos = 0;
		while((start_pos = str.find(from, start_pos)) != std::string::npos) {
			str.replace(start_pos, from.length(), to);
			start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
		}
		return str;
	}
}
///=================================================================================///


///=================================REMOVE COMMENTS=================================///
//Remove 1 comments -> return true
//No more comments  -> return false
bool 
remove_html_comment(std::string& html_response)
{
	//save the start position of the start of a comment
	unsigned int position_start		 = html_response.find ("<!--");
	unsigned int position_stop		  = html_response.find ("-->");
	if (position_start != std::string::npos) {
		//now we need to check for the end of it
		//... if no end
		//then we remove all after the start of the comment
		if (position_stop != std::string::npos) {
			html_response.erase (position_start, (position_stop-position_start)+3);
		}
		else {
			html_response.erase (position_start,html_response.length()-1);
		}
		return true;
	}
	//couldn't find any <!-- so no more comments in the html_response
	else {
		return false;
	}
}
void 
remove_html_comments(std::string & html_response)
{
	bool check_if_still_comments = true;
	while(check_if_still_comments) {
		check_if_still_comments  = remove_html_comment(html_response);
	}
}
///=================================================================================///

///====GET  "< *seeking(.*)< */or\ *seeking" and append it to the form_container====///
void 
get_after_delimiter(
	std::string html_response, 
	std::string seeking, 
	std::vector <std::string>& form_container)
{
	//0- Remove all comments because we don't want to search inside of them
	remove_html_comments(html_response);

	//1- lower it all but in a temporary string because we only want the indexes
	std::string html_lower;
	lower_it(html_response,html_lower);

	//2- start the search at 0 for the begining of a form
	//   until we reach stage 3 which will turn STOP
	//   to TRUE
	/* some variables we'll use */
	/*related to the start */
	bool STOP	  = false;
	int position	   = 0;
	unsigned int first_index;
	/*related to the end */
	bool found_end = false;
	int backward_ite;
	unsigned int last_index;

	while( STOP == false ) {
		// we didn't find an end yet
		found_end = false;
		//2- start the search at 0 for the begining of a form
		first_index = html_lower.find(seeking.c_str(),position);

		//3- if first_index shows that the word "form"
		//   is not present then there's no more forms
		//   in the html. WE STOP HERE
		if (first_index == std::string::npos) {
			STOP = true;
		}
		//4- we found a first instance of "form" BUT is it
		//   in the MIDDLE OF A WORD or at the BEGINING OF
		//   A FORM?
		else {
			// we go backward ignoring spaces until we reach a char
			backward_ite=1;
			while(html_lower[first_index-backward_ite]==' ') {
				backward_ite++;
			}
			// if the char after the spaces is not "<" then we go back to
			// stage 2 (searching for the word "form")

			// we restart the search from where we stoped
			// after the "form"
			position = first_index + seeking.length();
			//we found the start of a form at first_index
			if(html_lower[first_index-backward_ite] == '<') {
				//5- So we found the begining of a form "< *form"
				//   now we need to find its end
				//   we search for a "form" after it
				position = first_index + seeking.length();
				//we search increasingly, meaning the position variable
				//change after all "form" found until it's a real end
				while( found_end == false ) {
					last_index = html_lower.find(seeking.c_str(),position);
					//OMG MALFORMED HTML the <form> is not close
					//here this is to prevent infinity loop
					if (last_index == std::string::npos) {
						STOP = true;
						break;
					}
					else {
					}
					// we go backward ignoring spaces until we reach a char
					backward_ite=1;
					while(html_lower[last_index-backward_ite]==' ') {
						backward_ite++;
					}
					//The first char should be "/" or "\\"
					//if not we search again for the end of the form
					position = last_index + seeking.length();

					if(html_lower[last_index-backward_ite] == '/' ||
					   html_lower[last_index-backward_ite] == '\\') {
					//6- if there's a / or \\ then we go backward again ignoring spaces
						backward_ite++;
						while(html_lower[last_index-backward_ite]==' ') {
							backward_ite++;
						}
						//7- we found the start of a form and the end of it
						if(html_lower[last_index-backward_ite]=='<') {
							//let's add it to the container
							form_container.push_back(  html_response.substr(first_index,last_index-first_index)  );
							found_end=true;
						}
						else {
						}
					}
					else {
					}
				}
			}
		}
	}
}
///=================================================================================///


#endif 

