#ifndef CONCULANDIA_H
#define CONCULANDIA_H

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "CmdLine.h"
#include "Contador.h"
#include "Frontera.h"
#include "Log.h"
#include "Migraciones.h"
#include "PedidosCaptura.h"
#include "MinisterioSeguridad.h"
#include "Person.h"
#include "Queue.h"
#include "Shell.h"

void Conculandia_run(CmdLine *cl);

#endif  //CONCULANDIA_H
