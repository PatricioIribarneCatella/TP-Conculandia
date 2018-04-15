#include "CmdLine.h"

static void print_usage() {
	printf("USAGE: ./main.out -v numV -s numS -l log_filename.txt\n");
	printf("Tener en cuenta que: numV > numS y (numS > 1 && numV > 1) y l es opcional con valor por defecto %s\n",
		LOG_DEFAULT_FILENAME);
}

void init_cl(CmdLine *cl) {
	strcpy(cl->log_filename, "log.txt");
	cl->sellos = 2;
	cl->ventanillas = 4;
};

void CmdLine_parse(int argc, char *argv[], CmdLine *cl) {
	int option;
	init_cl(cl);

	while ((option = getopt(argc, argv, "v:s:l:")) != -1) {
		switch (option) {
			case 'v':
				cl->ventanillas = atoi(optarg);
				break;
			case 's':
				cl->sellos = atoi(optarg);
				break;
			case 'l':
				strcpy(cl->log_filename, optarg);
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
