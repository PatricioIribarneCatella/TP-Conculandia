#include "CmdLine.h"

static void print_usage() {
	printf("USAGE: ./main.out -v numV -s numS\n");
	printf("Tener en cuenta que: numV > numS y (numS > 1 && numV > 1)\n");
}

void CmdLine_parse(int argc, char *argv[], CmdLine *cl) {
	int option;

	while ((option = getopt(argc, argv, "v:s:")) != -1) {
		switch (option) {
			case 'v':
				cl->ventanillas = atoi(optarg);
				break;
			case 's':
				cl->sellos = atoi(optarg);
				break;
			default:
				print_usage();
				_exit(EXIT_FAILURE);
		}
	}

	if (cl->ventanillas < 2 || cl->sellos < 2 ||
		(cl->sellos >= cl->ventanillas)) {
		print_usage();
		_exit(EXIT_FAILURE);
	}
}
