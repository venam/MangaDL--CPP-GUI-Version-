/*
 * Webtoons downloader
 *
 * https://www.webtoons.com/search?keyword=tower
 * find in page:
 * card_item N=a:scl.list
 * <a href="/episodeList?titleNo=95"
 *
 * add referer
 *
 */

#include "webtoons.hpp"

webtoons::webtoons(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end)
	: dlder( manganame, mg_location, external_dl_mgr, start, end)
{
	//mgreader_current_page_url     ="http://www.mangareader.net/"+ dlder_manganame+"/"+dlder_start +"/";
	//mgreader_next_link        = "";
}

webtoons::webtoons() : dlder()
{
}

void
webtoons::init(std::string manganame, std::string mg_location,
	std::string external_dl_mgr,std::string start, std::string end) 
{
	// initialize the variable and browser
	dlder::init(manganame,mg_location,external_dl_mgr, start, end);
	dlder_external_dl_mgr = "default";

// curl 'https://webtoon-phinf.pstatic.net/20170612_128/1497237379108N3y8L_JPEG/1497237379079953356.jpg?type=q90' -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: https://www.webtoons.com/en/fantasy/tower-of-god/season-2-ep-254/viewer?title_no=95&episode_no=335' -H 'DNT: 1' -H 'Connection: keep-alive' -H 'Pragma: no-cache' -H 'Cache-Control: no-cache' --output test2.jpg

	dlder_manganame           = manganame;
	dlder_mg_location         = mg_location;
	dlder_external_dl_mgr     = "curl '[INPUT]' -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:58.0) Gecko/20100101 Firefox/58.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: [REFERER]' -H 'DNT: 1' -H 'Connection: keep-alive' -H 'Pragma: no-cache' -H 'Cache-Control: no-cache' --output '[OUTPUT]'";
	dlder_url_img             = "";
	dlder_img                 = "000";
	dlder_nb_of_chapter       = 0;
	dlder_nb_of_pages         = 0;
	dlder_paused_flag         = false;
	dlder_stop_flag           = false;

	dlder_start               = start;
	dlder_end                 = end;
	dlder_current_page        = 1;
	dlder_current_chapter     = atoi(start.c_str());

	this->search_url = "https://www.webtoons.com/search?keyword=";
	this->webtoon_series_url = "";
	this->title_no = -1;
}

// destructor
webtoons::~webtoons()
{
}

void
webtoons::initial_toon_search()
{
	std::string search_query = this->search_url + dlder_br.escape(dlder_manganame);
	std::cout<< "doing initial search on : "<< search_query <<"\n";
	do {
		dlder_br.open(search_query, 100);
	} while(dlder_br.error());
	std::vector<std::string> response_lines;
	response_lines = split(dlder_br.response(), '\n');

	int nb_results = 0;
	if (dlder_stop_flag) {
		return;
	}

	for (unsigned int i=0; i < response_lines.size(); i++) {
		if (word_in(response_lines[i], "class=\"search_result\">Webtoon (")) {
			this->webtoon_series_url = "";
			std::cout<< "FOUND RESULTS LINE: "<< response_lines[i] << "\n";

			std::string str_nb_results = response_lines[i];
			/* <h3 class="search_result">Webtoon (1 results)</h3> */
			replaceAll(str_nb_results,"<h3 class=\"search_result\">Webtoon (", "");
			replaceAll(str_nb_results," results)</h3>", "");
			replaceAll(str_nb_results," ", "");
			std::cout << "str nb results: "<< str_nb_results << "\n";
			nb_results = atoi(str_nb_results.c_str());
			std::cout << "nb results: "<< nb_results << "\n";
			// if the results are 0 exits directly
			if (nb_results == 0) {
				dlder_stop_flag = true;
				break;
			}
		} else if (word_in(response_lines[i], "card_item N=a:scl.list")
				&& nb_results > 0) {
			std::cout<< "THE LINK SHOULD BE HERE: "<<response_lines[i] << "\n";

			std::string series_url = response_lines[i];
			/* <a href="/episodeList?titleNo=95" class="card_item N=a:scl.list,i:95,g:en_en" */
			replaceAll(series_url, "<a href=\"", "");
			unsigned int index = series_url.find("\"", 0);
			series_url = series_url.substr(0, index);
			replaceAll(series_url," ", "");
			replaceAll(series_url,"\t", "");

			if (this->webtoon_series_url == "") {
				this->webtoon_series_url = "https://www.webtoons.com" + series_url;
				std::cout<<this->webtoon_series_url<<"\n";
				break;
			}
		}
	}

	// no results were matching
	if (this->webtoon_series_url == "") {
		dlder_stop_flag = true;
	}
}

void
webtoons::scrap_chapters_list()
{
	// from newest to oldest
	if (dlder_stop_flag) {
		return;
	}

	// open the toon url
	dlder_br.open(this->webtoon_series_url,100);
	// this will redirect to the real url
	this->webtoon_series_url = dlder_br.get_url();
	unsigned int index = this->webtoon_series_url.find("title_no=", 0);

	std::string title_no_str = this->webtoon_series_url.substr(index+9, this->webtoon_series_url.length());
	this->title_no = atoi(title_no_str.c_str());
	std::cout<< "title_no=" << title_no<< "\n";
	std::vector<std::string> response_lines;

	// search until we find the chapters we were looking for

	// fill in this std::map <int, std::string> chapters_url;
	int chapter_list_page = 1;
	int latest_chapter = -1;
	int chapter_number = -1;
	while (true) {
		if (dlder_stop_flag) {
			return;
		}

		response_lines = split(dlder_br.response(), '\n');
		for (unsigned int i=0; i < response_lines.size(); i++) {
			if (word_in(response_lines[i], "title_no=" + to_string(title_no) + "&episode_no")
				&& !word_in(response_lines[i], "First episode") ) {
				// <a href="https://www.webtoons.com/en/fantasy/tower-of-god/season-2-ep-315/viewer?title_no=95&episode_no=396" ...
				index = response_lines[i].find("episode_no=", 0);
				std::string episode_no_str = response_lines[i].substr(index+11, response_lines[i].length());
				index = episode_no_str.find("\"", 0);
				episode_no_str = episode_no_str.substr(0, index);
				chapter_number = atoi(episode_no_str.c_str());

				index = response_lines[i].find("a href=\"", 0);
				std::string chapter_url = response_lines[i].substr(index+8, response_lines[i].length());
				index = chapter_url.find("\"", 0);
				chapter_url = chapter_url.substr(0, index);

				if (latest_chapter == -1) {
					latest_chapter = chapter_number;
					if (latest_chapter < atoi(dlder_start.c_str())) {
						dlder_start = to_string(latest_chapter);
					}
					if (atoi(dlder_end.c_str()) > latest_chapter) {
						dlder_end = to_string(latest_chapter);
					}
				}
				chapters_url[chapter_number] = chapter_url;
				std::cout<< chapter_number << chapters_url[chapter_number]<<"\n";
			}
		}
		// go down in the page list
		if (chapter_number == 1) {
			break;
		}
		// also stop if the start chapter is bigger than the chapter_number
		if (chapter_number <= atoi(dlder_start.c_str())) {
			break;
		}
		chapter_list_page++;
		std::cout<< "increasing page number in list " << chapter_list_page << "\n";
		dlder_br.open(this->webtoon_series_url+"&page="+to_string(chapter_list_page),100);
	}
	// if latest
	// if the latest chapter is bigger than 1 then we need to move to the next page = this->webtoon_series_url + &page=11 
}

void 
webtoons::increase_page()
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

void
webtoons::increase_chapter()
{
	dlder_nb_of_pages          = 0;
	dlder_current_page         = 1;
	dlder_img                  = "000";
}

void
webtoons::start_downloading()
{
	if (dlder_stop_flag) {
		return;
	}

	for (int i = atoi(dlder_start.c_str()); i <= atoi(dlder_end.c_str()); i++) {
		dlder_current_chapter = i;
		dlder_referer = chapters_url[i];
		manage_chapters();
		std::cout<< "opening: "<< chapters_url[i] << "\n";
		do {
			dlder_br.open(chapters_url[i], 100);
		} while(dlder_br.error());

		std::vector<std::string> image_list;
		std::vector<std::string> response_lines;
		response_lines = split(dlder_br.response(), '\n');

		// page is open -> count the number of images push them in a list and download

		// getting all images in the page
		/* <img src="https://webtoon-phinf.pstatic.net/20161010_172/1476072595930DXO7P_JPEG/1476072595899953007.jpg?type=q90" alt="image" class="_images" data-url="https://webtoon-phinf.pstatic.net/20161010_172/1476072595930DXO7P_JPEG/1476072595899953007.jpg?type=q90" rel="nofollow" ondragstart="return false;" onselectstart="return false;" oncontextmenu="return false;" width="700" height="1240.0"> */
		for (unsigned int i=0; i < response_lines.size(); i++) {
			if (word_in(response_lines[i], "alt=\"image\" class=\"_images\" data-url=\"")) {
				unsigned int index = response_lines[i].find("data-url=\"", 0);
				std::string img_src = response_lines[i].substr(index+10, response_lines[i].length());
				index = img_src.find("\"", 0);
				img_src = img_src.substr(0, index);
				std::cout<< img_src << "\n";
				image_list.push_back(img_src);
			}
		}

		dlder_nb_of_pages = image_list.size();
		for (unsigned int i=0; i< image_list.size(); i++) {
			dlder_url_img = image_list[i];
			std::cout<< "downloading image at url = " << image_list[i] << "\n";
			download_image();
			increase_page();
		}
		// reset chapter related vars
		increase_chapter();
	}
}

void
webtoons::run()
{
	initial_toon_search();
	if (!dlder_stop_flag) {
		scrap_chapters_list();
		start_downloading();
	} else {
		std::cout<< "couldn't do initial search\n";
	}

	std::cout<< "[*] Finished all the downloads\nEnjoy Your Reading!";
}

