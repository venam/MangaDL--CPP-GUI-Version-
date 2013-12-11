#include "manager/dlder/provider/mgreader.hpp"

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


///------------------------TEST CLASS IN MAIN HERE-------------------------///
int main()
{
	std::string man;
	std::string  loc;
	std::string ext;
	std::string  st;
	std::string  en;
	std::cout << "Enter the manga name        => ";
	std::cin  >> man;
	std::cout << "Enter the Starting chapter  => ";
	std::cin  >> st;
	std::cout << "Enter the Finishing chapter => ";
	std::cin  >> en;

	dlder *my_downloader = new mgreader(man,"/home/stick","axel [INPUT] -o [OUTPUT] ",st,en);
	pthread_t th_1;
	pthread_create(&th_1,NULL,&print_stat,my_downloader);
	my_downloader->run();
	pthread_join(th_1,NULL);

	delete my_downloader;
	return 0;
}
///------------------------------------------------------------------------///

