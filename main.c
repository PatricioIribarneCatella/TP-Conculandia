#include <stdio.h>
#include <unistd.h>

#include "CmdLine.h"

int main(int argc, char* argv[]) {

	CmdLine cl;

	CmdLine_parse(argc, argv, &cl);

	printf("Sellos: %d, ventanillas: %d\n", cl.sellos, cl.ventanillas);

	return 0;
}
