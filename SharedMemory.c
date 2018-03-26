#include "SharedMemory.h"

int ShareMem_crear(SharedMemory *SHM,
				   size_t size,
				   const char *filename,
				   int num) {
	//Creo la key
	key_t key = ftok(filename, num);
	if (key == ERROR_FTOK) {
		return ERROR_FTOK;
	}

	//Reservo el segmento de memoria
	SHM->mem_id = shmget(key, size, 0644 | IPC_CREAT | IPC_EXCL);
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
	//Hago el detach
	shmdt((void *) SHM->mem_ptr);

	//Si no hay procesos adosados libero
	int procAdosados = ShareMem_cantProcesosAdosados(SHM);
	if (procAdosados == 0) {
		shmctl(SHM->mem_id, IPC_RMID, NULL);
	}

	return SHM_OK;
}

int ShareMem_leer(SharedMemory *SHM, void *ptr, int offset, size_t s) {
	*((char *) ptr) = *(((char *) SHM->mem_ptr) + offset * s);
	return SHM_OK;
}

int ShareMem_escribir(SharedMemory *SHM,
					  const void *ptr,
					  int offset,
					  size_t s) {
	*(((char *) SHM->mem_ptr) + offset * s) = *((char *) ptr);
	return SHM_OK;
}
