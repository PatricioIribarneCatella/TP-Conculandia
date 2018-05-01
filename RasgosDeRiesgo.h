#ifndef RASGOS_DE_RIESGO_H
#define RASGOS_DE_RIESGO_H

#include "Person.h"
#include "utils.h"

typedef struct {
	short int mascaraOjos;
	short int mascaraPelo;
	short int mascaraCaracteristicasEspeciales;
	short int mascaraSexo;
} RasgosDeRiesgo;

// Creacion
void Rasgos_inicializar(RasgosDeRiesgo *rasgos);

// Chequeo
int Rasgos_Persona_es_de_riesgo(RasgosDeRiesgo *rasgos, Person *persona);

// Modificacion
int Rasgos_Aniadir_sexo(RasgosDeRiesgo *rasgos, enum sexos nuevo_sexo);
int Rasgos_Remover_sexo(RasgosDeRiesgo *rasgos, enum sexos sexo);
int Rasgos_Aniadir_pelo(RasgosDeRiesgo *rasgos, enum tipos_pelos nuevo_pelo);
int Rasgos_Remover_pelo(RasgosDeRiesgo *rasgos, enum tipos_pelos pelo);
int Rasgos_Aniadir_ojos(RasgosDeRiesgo *rasgos, enum colores_ojos nuevos_ojos);
int Rasgos_Remover_ojos(RasgosDeRiesgo *rasgos, enum colores_ojos ojos);
int Rasgos_Aniadir_caracteristica_especial(
	RasgosDeRiesgo *rasgos,
	enum caracteristicas_especiales nueva_caracteristica);
int Rasgos_Remover_caracteristica_especial(
	RasgosDeRiesgo *rasgos, enum caracteristicas_especiales caracteristica);

// Getters
int Rasgos_get_ojos(RasgosDeRiesgo *rasgos);
int Rasgos_get_pelo(RasgosDeRiesgo *rasgos);
int Rasgos_get_sexo(RasgosDeRiesgo *rasgos);
char *Rasgos_get_caracteristica_especial(RasgosDeRiesgo *rasgos);

#endif  // RASGOS_DE_RIESGO_H
