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
#include "Person.h"
#include "Queue.h"
#include "Shell.h"


int Conculandia_init(CmdLine* cl,
                      Log* log,
                      Queue* q,
                      Sellos* sellos,
                      Contador* personas,
                      Contador* pers_arrestadas,
                      PedidosCaptura* p_captura,
                      pid_t* frontera);

void Liberar_recursos(Log* log,
                     Queue* q,
                     Sellos* sellos,
                     Contador* personas,
                     Contador* pers_arrestadas,
                     PedidosCaptura* p_captura);

#endif  //CONCULANDIA_H
