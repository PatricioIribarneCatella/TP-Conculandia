#ifndef MIGRACIONES_H
#define MIGRACIONES_H

#include <errno.h>
#include <stdio.h>

#include "Contador.h"
#include "Person.h"
#include "Queue.h"
#include "Sellos.h"

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla);

#endif  //MIGRACIONES_H
