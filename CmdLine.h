#ifndef CMDLINE_H
#define CMDLINE_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LOG_DEFAULT_FILENAME "log.txt"


typedef struct {
	int sellos;
	int ventanillas;
	char log_filename[40];
} CmdLine;

void CmdLine_parse(int argc, char *argv[], CmdLine *cl);

#endif  //CMDLINE_H
