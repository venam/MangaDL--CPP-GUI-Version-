#include "manager/dlder/provider/mgfox.hpp"


///------------------------TEST CLASS IN MAIN HERE-------------------------///
int main()
{

	std::string mg;
	std::cout<<"Enter manga to test> ";
	std::cin>>mg;
	mgfox *my_downloader = new mgfox(mg,"/home/stick","axel [INPUT] -o [OUTPUT] ","1","2");
	my_downloader->debug();

	delete my_downloader;
	return 0;
}
///------------------------------------------------------------------------///

