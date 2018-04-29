#include "Conculandia.h"

int main(int argc, char *argv[]) {

	CmdLine cl;
	
	CmdLine_parse(argc, argv, &cl);

	Conculandia_run(&cl);
	
	return 0;
}
