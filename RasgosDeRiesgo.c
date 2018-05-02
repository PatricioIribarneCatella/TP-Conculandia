#include "RasgosDeRiesgo.h"

// Chequeo de riesgos
static int esta_prendido_en_mascara(short int mascara, unsigned int posicion) {
	return mascara & (1 << posicion);
}

// Agregado y quitado de caracteristicas
static void remover_de_mascara(short int *mascara, unsigned int posicion) {
	(*mascara) &= ~(1 << posicion);
}

static void aniadir_a_mascara(short int *mascara, unsigned int posicion) {
	(*mascara) |= 1 << posicion;
}


// Creacion de lista
void Rasgos_inicializar(RasgosDeRiesgo *rasgos) {
	rasgos->mascaraCaracteristicasEspeciales = 0;
	rasgos->mascaraOjos = 0;
	rasgos->mascaraPelo = 0;
	rasgos->mascaraSexo = 0;
}

int Rasgos_Persona_es_de_riesgo(RasgosDeRiesgo *rasgos, Person *persona) {
	CaracteristicasPersona *car = &(persona->caracteristicas);
	return rasgos->mascaraCaracteristicasEspeciales & car->caracteristicasEspeciales ||
		   esta_prendido_en_mascara(rasgos->mascaraOjos, car->ojos) ||
		   esta_prendido_en_mascara(rasgos->mascaraPelo, car->pelo) ||
		   esta_prendido_en_mascara(rasgos->mascaraSexo, car->sexo);
}

int Rasgos_Aniadir_sexo(RasgosDeRiesgo *rasgos, enum sexos nuevo_sexo) {
	if (nuevo_sexo > MAX_SEXO)
		return ARGUMENTO_INVALIDO;
	aniadir_a_mascara(&(rasgos->mascaraSexo), nuevo_sexo);
	return CAMBIO_OK;
}

int Rasgos_Remover_sexo(RasgosDeRiesgo *rasgos, enum sexos sexo) {
	if (sexo > MAX_SEXO)
		return ARGUMENTO_INVALIDO;
	remover_de_mascara(&(rasgos->mascaraSexo), sexo);
	return CAMBIO_OK;
}

int Rasgos_Aniadir_pelo(RasgosDeRiesgo *rasgos, enum tipos_pelos nuevo_pelo) {
	if (nuevo_pelo > MAX_TIPO_PELO)
		return ARGUMENTO_INVALIDO;
	aniadir_a_mascara(&(rasgos->mascaraPelo), nuevo_pelo);
	return CAMBIO_OK;
}

int Rasgos_Remover_pelo(RasgosDeRiesgo *rasgos, enum tipos_pelos pelo) {
	if (pelo > MAX_TIPO_PELO)
		return ARGUMENTO_INVALIDO;
	remover_de_mascara(&(rasgos->mascaraPelo), pelo);
	return CAMBIO_OK;
}

int Rasgos_Aniadir_ojos(RasgosDeRiesgo *rasgos, enum colores_ojos nuevos_ojos) {
	if (nuevos_ojos > MAX_COLOR_OJO)
		return ARGUMENTO_INVALIDO;
	aniadir_a_mascara(&(rasgos->mascaraOjos), nuevos_ojos);
	return CAMBIO_OK;
}

int Rasgos_Remover_ojos(RasgosDeRiesgo *rasgos, enum colores_ojos ojos) {
	if (ojos > MAX_COLOR_OJO)
		return ARGUMENTO_INVALIDO;
	remover_de_mascara(&(rasgos->mascaraOjos), ojos);
	return CAMBIO_OK;
}

int Rasgos_Aniadir_caracteristica_especial(
	RasgosDeRiesgo *rasgos,
	enum caracteristicas_especiales nueva_caracteristica) {
	if (nueva_caracteristica > MAX_CARACTERISTICA_ESPECIAL)
		return ARGUMENTO_INVALIDO;
	aniadir_a_mascara(&(rasgos->mascaraCaracteristicasEspeciales),
					  nueva_caracteristica);
	return CAMBIO_OK;
}

int Rasgos_Remover_caracteristica_especial(
	RasgosDeRiesgo *rasgos, enum caracteristicas_especiales caracteristica) {
	if (caracteristica > MAX_CARACTERISTICA_ESPECIAL)
		return ARGUMENTO_INVALIDO;
	remover_de_mascara(&(rasgos->mascaraCaracteristicasEspeciales),
					   caracteristica);
	return CAMBIO_OK;
}

int Rasgos_get_ojos(RasgosDeRiesgo *rasgos) {
	return rasgos->mascaraOjos;
}

int Rasgos_get_pelo(RasgosDeRiesgo *rasgos) {
	return rasgos->mascaraPelo;
}

int Rasgos_get_sexo(RasgosDeRiesgo *rasgos) {
	return rasgos->mascaraSexo;
}

int Rasgos_get_caracteristica_especial(RasgosDeRiesgo *rasgos) {
	return rasgos->mascaraCaracteristicasEspeciales;
}
