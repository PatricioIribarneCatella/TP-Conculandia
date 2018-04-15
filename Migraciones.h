#ifndef MIGRACIONES_H
#define MIGRACIONES_H

#include "Contador.h"
#include "Person.h"
#include "Queue.h"
#include "Sellos.h"
#include <errno.h>
#include <stdio.h>

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla);

#endif  //MIGRACIONES_H
