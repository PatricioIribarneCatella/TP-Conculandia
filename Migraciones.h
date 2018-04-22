#ifndef MIGRACIONES_H
#define MIGRACIONES_H

#include <errno.h>
#include <stdio.h>

#include "Contador.h"
#include "Log.h"
#include "Person.h"
#include "Queue.h"
#include "Sellos.h"
#include "PedidosCaptura.h"

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla, Log *log);

#endif  //MIGRACIONES_H
