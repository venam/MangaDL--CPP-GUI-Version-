#include "manager/dlder/provider/mgpark.hpp"

int
main (int argc, char *argv[])
{
	dlder *d = new mgpark();
	d->init(
		"tenkuu-shinpan-miura-tsuina",
		"/home/raptor/mg",
		"wget [INPUT] -O [OUTPUT]",
		"1", 
		"2"
	);
	d->run();
	delete d;
	return 0;
}
