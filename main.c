#include <stdio.h>
#include <unistd.h>

#include "CmdLine.h"
#include "Conculandia.h"

int main(int argc, char* argv[]) {

	CmdLine cl;

	CmdLine_parse(argc, argv, &cl);

	Conculandia_init(&cl);

	return 0;
}
