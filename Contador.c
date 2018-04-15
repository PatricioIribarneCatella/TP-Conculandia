#include "Contador.h"
#include "SharedMemory.h"

int Contador_crear(Contador *C, const char *filename){
    ShareMem_crear(&(C->shm_cont), sizeof(int), filename, CONT_DEFAULT_NUM);
    Semaphore_init(&(C->sem), filename, 1);
    return 0;
}

int Contador_init_to_zero(Contador *C){
    int init = 0;
    Semaphore_p(&(C->sem));
    ShareMem_escribir(&(C->shm_cont), &init, 0, sizeof(int));
    Semaphore_v(&(C->sem));
    return 0;
}

int Contador_get(Contador *C){
    int c = 0;
    Semaphore_p(&(C->sem));
    ShareMem_leer(&(C->shm_cont), &c, 0, sizeof(int));
    Semaphore_v(&(C->sem));
    return c;
}

int Contador_incrementar(Contador *C){
    Semaphore_p(&(C->sem));
    *((int *) C->shm_cont.mem_ptr) = *((int *) C->shm_cont.mem_ptr) + 1;
    Semaphore_v(&(C->sem));
    return 0;
}

int Contador_eliminar(Contador *C){
    Semaphore_eliminar(&(C->sem));
    ShareMem_liberar(&(C->shm_cont));
    return 0;
}
