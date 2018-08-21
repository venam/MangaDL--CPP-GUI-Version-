/*
 *
 * https://www.webtoons.com/search?keyword=tower
 * find in page:
 * card_item N=a:scl.list
 * <a href="/episodeList?titleNo=95"
 *
 * add referer
 *
 */

#ifndef INCLUDED_WEBTOONS
#define INCLUDED_WEBTOONS

#include "../../browser/browser.hpp"
#include "../dlder.hpp"
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

class webtoons: public dlder {
	private:
		// map containing the chapter number and the url related to it
		std::map <int, std::string> chapters_url;
		std::string search_url;
		std::string webtoon_series_url;
		int title_no;
		void initial_toon_search();
		void scrap_chapters_list();
		void start_downloading();
		void increase_page();
		void increase_chapter();

	public:
		/* abstract methods that need to be implemented */
		webtoons(
				std::string manganame, std::string mg_location,
				std::string external_dl_mgr,std::string start, std::string end);
		webtoons();
		~webtoons();

		void run();
		void init(std::string manganame, std::string mg_location,
			std::string external_dl_mgr,std::string start, std::string end);
};


#endif
