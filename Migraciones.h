#ifndef MIGRACIONES_H
#define MIGRACIONES_H

#include <errno.h>
#include <stdio.h>

#include "Contador.h"
#include "Log.h"
#include "PedidosCaptura.h"
#include "Person.h"
#include "Queue.h"
#include "RasgosDeRiesgoCompartidos.h"
#include "Sellos.h"

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla, Log *log);

#endif  //MIGRACIONES_H
