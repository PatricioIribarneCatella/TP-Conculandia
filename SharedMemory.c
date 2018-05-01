#include "SharedMemory.h"

int ShareMem_crear(SharedMemory *SHM, size_t size, const char *filename,
				   int num) {
	//Creo la key
	key_t key = ftok(filename, num);
	if (key == ERROR_FTOK) {
		return ERROR_FTOK;
	}

	//Reservo el segmento de memoria (no usar IPC_EXCL, si ya fue creado hay que seguir de la misma forma)
	SHM->mem_id = shmget(key, size, 0644 | IPC_CREAT);
	if (SHM->mem_id == ERROR_SHMGET) {
		return ERROR_SHMGET;
	}

	//Hago el attach
	SHM->mem_ptr = shmat(SHM->mem_id, NULL, 0);
	if (SHM->mem_ptr == (void *) -1) {
		return ERROR_SHMAT;
	}

	SHM->size = size;

	return SHM_OK;
}

int ShareMem_cantProcesosAdosados(SharedMemory *SHM) {
	struct shmid_ds estado;
	shmctl(SHM->mem_id, IPC_STAT, &estado);

	return estado.shm_nattch;
}

int ShareMem_liberar(SharedMemory *SHM) {
	int return_value = SHM_OK;
	//Hago el detach
	return_value = shmdt((void *) SHM->mem_ptr);

	//Si no hay procesos adosados libero
	int procAdosados = ShareMem_cantProcesosAdosados(SHM);

	// Puede haber dos procesos que traten de liberar esta memoria, no hay ningun problema, a uno
	// se le devolvera error diciendo que la memoria borrada
	if (procAdosados == 0) {
		return_value = shmctl(SHM->mem_id, IPC_RMID, NULL);
	}

	return return_value;
}

int ShareMem_leer(SharedMemory *SHM, void *ptr, int offset, size_t s) {
	memcpy(ptr, ((char *) SHM->mem_ptr) + offset * s, s);
	return SHM_OK;
}

int ShareMem_escribir(SharedMemory *SHM, const void *ptr, int offset,
					  size_t s) {
	memcpy(((char *) SHM->mem_ptr) + offset * s, ptr, s);
	return SHM_OK;
}
