// clang-format off
#include "RasgosDeRiesgoCompartidos.h"

// Creacion
int RasgosCompartidos_crear(RasgosDeRiesgoCompartidos *rasgos,
							int modoLectura) {
	int error = LockArchivo_inicializar(
		&(rasgos->lock), RASGOS_DE_RIESGO_SHM_FILENAME, modoLectura);

	if (!error)
		error = ShareMem_crear(&(rasgos->shm), sizeof(RasgosDeRiesgo),
							   RASGOS_DE_RIESGO_SHM_FILENAME,
							   RASGOS_DE_RIESGO_SHM_NUMBER);
	if (!error)
		Rasgos_inicializar(rasgos->shm.mem_ptr);

	return error < 0 ? error : 0;
}

// Chequeo (Locks de lectura)
int RasgosCompartidos_Persona_es_de_riesgo(RasgosDeRiesgoCompartidos *rasgos,
										   Person *persona) {
	if (!rasgos->lock.modoLectura)
		return -1;

	int return_value, error;

	// Adquirir lock
	error = LockArchivo_tomar(&(rasgos->lock));

	if (error) {
		perror("ERROR: adquirir lock - rasgos de riesgo ");
		return error;
	}

	return_value = Rasgos_Persona_es_de_riesgo(RASGOS_PTR(rasgos), persona);

	// Liberar lock
	error = LockArchivo_liberar(&(rasgos->lock));

	if (error) {
		perror("ERROR: liberar lock - rasgos de riesgo ");
		return error;
	}

	return return_value;
}

int RasgosCompartidos_tomar_lock(RasgosDeRiesgoCompartidos *rasgos, int modo) {
	if (rasgos->lock.modoLectura != modo)
		return -1;

	return LockArchivo_tomar(&(rasgos->lock));
}

int RasgosCompartidos_liberar_lock(RasgosDeRiesgoCompartidos *rasgos,
								   int modo) {
	if (rasgos->lock.modoLectura != modo)
		return -1;

	return LockArchivo_liberar(&(rasgos->lock));
}

// Modificación (Locks de escritura)
int RasgosCompartidos_Aniadir_sexo(RasgosDeRiesgoCompartidos *rasgos,
								   enum sexos nuevo_sexo) {
	return Rasgos_Aniadir_sexo(RASGOS_PTR(rasgos), nuevo_sexo);
}

int RasgosCompartidos_Remover_sexo(RasgosDeRiesgoCompartidos *rasgos,
								   enum sexos sexo) {
	return Rasgos_Remover_sexo(RASGOS_PTR(rasgos), sexo);
}

int RasgosCompartidos_Aniadir_pelo(RasgosDeRiesgoCompartidos *rasgos,
								   enum tipos_pelos nuevo_pelo) {
	return Rasgos_Aniadir_pelo(RASGOS_PTR(rasgos), nuevo_pelo);
}

int RasgosCompartidos_Remover_pelo(RasgosDeRiesgoCompartidos *rasgos,
								   enum tipos_pelos pelo) {
	return Rasgos_Remover_pelo(RASGOS_PTR(rasgos), pelo);
}

int RasgosCompartidos_Aniadir_ojos(RasgosDeRiesgoCompartidos *rasgos,
								   enum colores_ojos nuevos_ojos) {
	return Rasgos_Aniadir_ojos(RASGOS_PTR(rasgos), nuevos_ojos);
}

int RasgosCompartidos_Remover_ojos(RasgosDeRiesgoCompartidos *rasgos,
								   enum colores_ojos ojos) {
	return Rasgos_Remover_ojos(RASGOS_PTR(rasgos), ojos);
}

int RasgosCompartidos_Aniadir_caracteristica_especial(
	RasgosDeRiesgoCompartidos *rasgos,
	enum caracteristicas_especiales nueva_caracteristica) {

	return Rasgos_Aniadir_caracteristica_especial(
		RASGOS_PTR(rasgos), nueva_caracteristica);
}

int RasgosCompartidos_Remover_caracteristica_especial(
	RasgosDeRiesgoCompartidos *rasgos,
	enum caracteristicas_especiales caracteristica) {
	
	return Rasgos_Remover_caracteristica_especial(
		RASGOS_PTR(rasgos), caracteristica);
}

// Getters
int RasgosCompartidos_get_ojos(RasgosDeRiesgoCompartidos* rasgos) {

	return Rasgos_get_ojos(RASGOS_PTR(rasgos));
}

int RasgosCompartidos_get_pelo(RasgosDeRiesgoCompartidos* rasgos) {

	return Rasgos_get_pelo(RASGOS_PTR(rasgos));
}

int RasgosCompartidos_get_sexo(RasgosDeRiesgoCompartidos* rasgos) {

	return Rasgos_get_sexo(RASGOS_PTR(rasgos));
}

char* RasgosCompartidos_get_caracteristica_especial(RasgosDeRiesgoCompartidos* rasgos) {

	return Rasgos_get_caracteristica_especial(RASGOS_PTR(rasgos));
}

// Eliminación
int RasgosCompartidos_eliminar(RasgosDeRiesgoCompartidos *rasgos) {
	int error_lock = LockArchivo_eliminar(&(rasgos->lock));
	int error_memoria_compartida = ShareMem_liberar(&(rasgos->shm));

	return error_lock ? error_lock : error_memoria_compartida;
}
