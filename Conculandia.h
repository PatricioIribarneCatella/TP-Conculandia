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
#include "Migraciones.h"
#include "PedidosCaptura.h"
#include "RasgosDeRiesgoCompartidos.h"
#include "Person.h"
#include "Queue.h"
#include "Shell.h"
#include "Log.h"

int Conculandia_init(CmdLine *cl, Log *log, Queue *q, Sellos *sellos,
			Contador *personas, Contador *pers_arrestadas, Contador* pers_deportadas,
			PedidosCaptura *p_captura, RasgosDeRiesgoCompartidos* r_riesgos,
			pid_t *frontera);

void Liberar_recursos(Log *log, Queue *q, Sellos *sellos,
			Contador *personas, Contador *pers_arrestadas, Contador* pers_deportadas,
			PedidosCaptura *p_captura, RasgosDeRiesgoCompartidos* r_riesgos);

#endif  //CONCULANDIA_H
