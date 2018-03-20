#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

typedef struct {
	int sellos;
	int ventanillas;
} CmdLine;

static void print_usage() {
	printf("USAGE: ./main.out -v num -s num\n");
}

static void parse_cmd_line(int argc, char* argv[], CmdLine* cl) {
	
	int option;

	while ((option = getopt(argc, argv, "v:s:")) != -1) {
		
		switch(option) {
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
	
	if (cl->ventanillas == 0 || cl->sellos == 0) {
		print_usage();
		_exit(EXIT_FAILURE);
	}
}

int main(int argc, char* argv[]) {

	CmdLine cl = {0, 0};

	parse_cmd_line(argc, argv, &cl);

	printf("Sellos: %d, ventanillas: %d\n", cl.sellos, cl.ventanillas);

	return 0;
}
