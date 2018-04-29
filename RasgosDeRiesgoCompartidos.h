#include <stdio.h>

#include "LockArchivo.h"
#include "RasgosDeRiesgo.h"
#include "SharedMemory.h"

#define RASGOS_DE_RIESGO_SHM_NUMBER 27
#define RASGOS_DE_RIESGO_SHM_FILENAME "/tmp/rcomp"

#define RASGOS_PTR(X) X->shm.mem_ptr

typedef struct {
	RasgosDeRiesgo rasgos;
	SharedMemory shm;
	LockArchivo lock;
} RasgosDeRiesgoCompartidos;

// Ciclo de vida
int RasgosCompartidos_crear(RasgosDeRiesgoCompartidos *rasgos, int modoLectura);
int RasgosCompartidos_eliminar(RasgosDeRiesgoCompartidos *rasgos);

// Chequeo (Locks de lectura)
int RasgosCompartidos_Persona_es_de_riesgo(RasgosDeRiesgoCompartidos *rasgos,
										   Person *persona);
// Modificacion (Locks de escritura)
int RasgosCompartidos_Aniadir_sexo(RasgosDeRiesgoCompartidos *rasgos,
								   enum sexos nuevo_sexo);
int RasgosCompartidos_Remover_sexo(RasgosDeRiesgoCompartidos *rasgos,
								   enum sexos sexo);
int RasgosCompartidos_Aniadir_pelo(RasgosDeRiesgoCompartidos *rasgos,
								   enum tipos_pelos nuevo_pelo);
int RasgosCompartidos_Remover_pelo(RasgosDeRiesgoCompartidos *rasgos,
								   enum tipos_pelos pelo);
int RasgosCompartidos_Aniadir_ojos(RasgosDeRiesgoCompartidos *rasgos,
								   enum colores_ojos nuevos_ojos);
int RasgosCompartidos_Remover_ojos(RasgosDeRiesgoCompartidos *rasgos,
								   enum colores_ojos ojos);
int RasgosCompartidos_Aniadir_caracteristica_especial(
	RasgosDeRiesgoCompartidos *rasgos,
	enum caracteristicas_especiales nueva_caracteristica);

int RasgosCompartidos_Remover_caracteristica_especial(
	RasgosDeRiesgoCompartidos *rasgos,
	enum caracteristicas_especiales caracteristica);
