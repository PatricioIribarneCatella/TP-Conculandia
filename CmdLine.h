#ifndef CMDLINE_H
#define CMDLINE_H

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>

typedef struct {
	int sellos;
	int ventanillas;
} CmdLine;

void CmdLine_parse(int argc, char* argv[], CmdLine* cl);

#endif //CMDLINE_H
