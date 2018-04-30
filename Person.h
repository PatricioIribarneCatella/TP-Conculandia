#ifndef PERSON_H
#define PERSON_H

#include <stdlib.h>

#include "CaracteristicasPersona.h"

//Puntos de partida para los dni/pasaport de las personas
#define DNI_START 12045660
#define PASAPORT_START 1050360

enum nacionalidades { NATIVO, EXTRANJERO };

typedef struct {
	int nacionalidad;
	int id;  //dni o pasaporte
	CaracteristicasPersona caracteristicas;
} Person;

void Person_random_generate(Person *p);

int Person_es_extranjero(Person *p);

#endif  //PERSON_H
