#ifndef CONCULANDIA_H
#define CONCULANDIA_H

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "CmdLine.h"
#include "Contador.h"
#include "Frontera.h"
#include "Migraciones.h"
#include "Person.h"
#include "Queue.h"

void Conculandia_init(CmdLine *cl);

#endif  //CONCULANDIA_H
