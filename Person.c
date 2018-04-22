#include "Person.h"

#define NAT_U_LIM 35
//1 si x > NAT_U_LIM, 0 caso contrario
//para regular el porcentage de nativos/extranjeros
#define NAT(X) (X > NAT_U_LIM ? 1 : 0)

//Son los id que se asigan a la persona
//se incrementan en uno cada vez que se genera una nueva persona
static int dni_n = DNI_START;
static int pasaport_n = PASAPORT_START;

void Person_random_generate(Person *p) {
	int r = random() % 100;
	p->nacionalidad = NAT(r);

	if (p->nacionalidad == NATIVO) {
		p->id = dni_n;
		dni_n = dni_n + 1;
	}
	else {
		p->id = pasaport_n;
		pasaport_n = pasaport_n + 1;
	}
}

int Person_es_extranjero(Person *p){
	return (p->nacionalidad == EXTRANJERO);
}
