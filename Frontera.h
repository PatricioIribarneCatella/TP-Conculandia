#ifndef FRONTERA_H
#define FRONTERA_H

#include "Person.h"
#include "Queue.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>


void Frontera_sigint_handler();

int Frontera_run();


#endif  //FRONTERA_H
