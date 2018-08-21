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
*
*/


#include "dl_facto.hpp"


///=================================================================================///
dl_facto::dl_facto()
{
	dl_facto_dlder = NULL;
	/* all the available providers */
	dl_facto_all_providers.push_back("mgreader");
	dl_facto_all_providers.push_back("mgpark");
	dl_facto_all_providers.push_back("mgfox");
	dl_facto_all_providers.push_back("webtoons");
}
///=================================================================================///


///=================================================================================///
dl_facto::~dl_facto()
{
	if (dl_facto_dlder != NULL) {
		delete dl_facto_dlder;
	}
	else {
		// dl_facto_dlder is already empty
	}

}
///=================================================================================///


///=================================================================================///
dlder* 
dl_facto::get_provider(std::string provider)
{
	if (dl_facto_dlder != NULL) {
		delete dl_facto_dlder;
	}
	else {
		// dl_facto_dlder is already empty
	}

	if (provider == "mgreader") {
		dl_facto_dlder = new mgreader();
	}
	else if (provider == "mgpark" ) {
		dl_facto_dlder = new mgpark();
	}
	else if (provider == "mgfox") {
		dl_facto_dlder = new mgfox();
	}
	else if (provider == "webtoons") {
		dl_facto_dlder = new webtoons();
	}
	else {
		std::cerr<<"Provider not found, using mgreader instead";
	}

	return dl_facto_dlder;
}
///=================================================================================///


///=================================================================================///
std::vector<std::string>
dl_facto::show_providers()
{
	return dl_facto_all_providers;
}
///=================================================================================///


