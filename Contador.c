#include "Contador.h"

int Contador_crear(Contador *C, const char *filename, int crear_sem){
    int error;

    error = ShareMem_crear(&(C->shm_cont), sizeof(int), filename, CONT_DEFAULT_NUM);

    if (!error)
        error = Semaphore_init(&(C->sem), filename, 1, crear_sem);
    C->crear_sem = crear_sem;
    return error;
}

int Contador_init_to_zero(Contador *C){
    int init = 0;
    int error = 0;
    error = Semaphore_p(&(C->sem));
    if (error)
        return error;

    ShareMem_escribir(&(C->shm_cont), &init, 0, sizeof(int));
    error = Semaphore_v(&(C->sem));
    return error;
}

int Contador_get(Contador *C){
    int c = 0;
    int error;
    error = Semaphore_p(&(C->sem));
    printf("Hola\n");
    if (error)
        return error;

    ShareMem_leer(&(C->shm_cont), &c, 0, sizeof(int));
    error = Semaphore_v(&(C->sem));

    printf("Hola\n");
    if (error)
        return error;

    return c;
}

int Contador_incrementar(Contador *C){

    int error = Semaphore_p(&(C->sem));

    if (!error) {
        *((int *) C->shm_cont.mem_ptr) = *((int *) C->shm_cont.mem_ptr) + 1;
        error = Semaphore_v(&(C->sem));
    }
    return error;
}

// Solo se va a borrar por el proceso que lo creo
int Contador_eliminar(Contador *C){
    int error_semaforo = 0, error_shm;
    if (C->crear_sem)
      error_semaforo = Semaphore_eliminar(&(C->sem));
    error_shm = ShareMem_liberar(&(C->shm_cont));
    return error_semaforo < 0 ? error_semaforo : error_shm;
}
