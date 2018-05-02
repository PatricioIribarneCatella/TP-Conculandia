#ifndef SHAREDMEMORY_H
#define SHAREDMEMORY_H

#include <stddef.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#define SHM_OK 0
#define ERROR_FTOK -1
#define ERROR_SHMGET -2
#define ERROR_SHMAT -3

typedef struct {
	void *mem_ptr;  // puntero a la memoria
	int mem_id;	// id del segmento de memoria
	size_t size;	// tamaño de la memoria (bytes)
} SharedMemory;

int ShareMem_crear(SharedMemory *SHM, size_t size, const char *filename,
				   int num);

int ShareMem_cantProcesosAdosados(SharedMemory *SM);

int ShareMem_liberar(SharedMemory *SHM);

int ShareMem_leer(SharedMemory *SHM, void *ptr, int offset, size_t s);

int ShareMem_escribir(SharedMemory *SHM, const void *ptr, int offset, size_t s);

#endif  //SHAREDMEMORY_H
