#include "Person.h"

// Verifica si está en el rango [a, b)
#define IN_RANGE(X, a, b) (X >= a && X < b)

//Son los id que se asigan a la persona
//se incrementan en uno cada vez que se genera una nueva persona
static int dni_n = DNI_START;
static int pasaport_n = PASAPORT_START;

static enum colores_ojos get_color_ojos(int r) {
	if (IN_RANGE(r, 0, 25))
		return OJOS_VERDES;
	if (IN_RANGE(r, 25, 50))
		return OJOS_AZULES;
	if (IN_RANGE(r, 50, 75))
		return OJOS_MARRONES_OSCUROS;
	return OJOS_MARRONES_CLAROS;
}

static enum tipos_pelos get_color_pelo(int r) {
	if (IN_RANGE(r, 0, 17))
		return PELO_RUBIO;
	if (IN_RANGE(r, 17, 33))
		return PELO_MOROCHO;
	if (IN_RANGE(r, 33, 50))
		return PELO_CASTANIO;
	if (IN_RANGE(r, 50, 67))
		return PELO_COLORADO;
	if (IN_RANGE(r, 67, 83))
		return PELO_CANOSO;
	return PELADO;
}

static enum sexos get_tipo_sexo(int r) {
	return (r >= 50) ? MUJER : HOMBRE;
}

static enum caracteristicas_especiales get_carac_esp(int r) {
	if (IN_RANGE(r, 0, 17))
		return GORRA;
	if (IN_RANGE(r, 17, 33))
		return TATUAJE_EN_ROSTRO;
	if (IN_RANGE(r, 33, 50))
		return TATUAJE_EN_PIERNAS;
	if (IN_RANGE(r, 50, 67))
		return AROS;
	if (IN_RANGE(r, 67, 83))
		return PIERCING_EN_NARIZ;
	return PIERCING_EN_BOCA;
}

void Person_random_generate(Person *p) {
	
	int r_ojos, r_pelo, r_sexo,
	r_una_caracteristica_especial,
	r_otra_caracteristica_especial, r_nat;

	r_ojos = random() % 100;
	r_pelo = random() % 100;
	r_sexo = random() % 100;
	r_una_caracteristica_especial = random() % 100;
	r_otra_caracteristica_especial = random() % 100;
	r_nat = random() % 100;

	p->nacionalidad = NAT(r_nat);

	// Generar Nacionalidad
	if (p->nacionalidad == NATIVO) {
		p->id = dni_n;
		dni_n = dni_n + 1;
	}
	else {
		p->id = pasaport_n;
		pasaport_n = pasaport_n + 1;
	}

	// Generar caracteristicas
	CaracteristicasPersona_init(&(p->caracteristicas));
	Cambiar_ojos(&(p->caracteristicas), get_color_ojos(r_ojos));
	Cambiar_pelo(&(p->caracteristicas), get_color_pelo(r_pelo));
	Cambiar_sexo(&(p->caracteristicas), get_tipo_sexo(r_sexo));
	Aniadir_caracteristica_especial(&(p->caracteristicas),
		get_carac_esp(r_una_caracteristica_especial));
	Aniadir_caracteristica_especial(&(p->caracteristicas),
		get_carac_esp(r_otra_caracteristica_especial));
}

// Getters
int Person_es_extranjero(Person *p) {
	return (p->nacionalidad == EXTRANJERO);
}

int Person_get_ojos(Person *p) {
	return p->caracteristicas.ojos;
}

int Person_get_pelo(Person *p) {
	return p->caracteristicas.pelo;
}

int Person_get_sexo(Person *p) {
	return p->caracteristicas.sexo;
}

int Person_get_caracteristica_especial(Person *p) {
	return p->caracteristicas.caracteristicasEspeciales;
}
