#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Contador.h"
#include "Log.h"

#define BUFLEN 1024
#define END_LINE '\n'
#define END_STRING '\0'

void Shell_run(int f, Log *log, Contador *p, Contador *p_arrest, Contador* p_deport);

#endif  // SHELL_H
