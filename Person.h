#ifndef PERSON_H
#define PERSON_H

#include <stdlib.h>

#include "CaracteristicasPersona.h"
#include "utils.h"

// Puntos de partida para los dni/pasaport de las personas
#define DNI_START 12045660
#define PASAPORT_START 1050360

#define NAT_U_LIM 35
// Regular el porcentage de nativos/extranjeros
#define NAT(X) (X > NAT_U_LIM ? 1 : 0)

enum nacionalidades { NATIVO, EXTRANJERO };

typedef struct {
	int nacionalidad;
	int id;  //dni o pasaporte
	CaracteristicasPersona caracteristicas;
} Person;

void Person_random_generate(Person *p);

int Person_es_extranjero(Person *p);

int Person_get_ojos(Person *p);
int Person_get_pelo(Person *p);
int Person_get_sexo(Person *p);
char *Person_get_caracteristica_especial(Person *p);

#endif  //PERSON_H
