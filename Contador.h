#ifndef CONTADOR_H
#define CONTADOR_H

#include "Semaphore.h"
#include "SharedMemory.h"

#define CONT_FILE_1 "Contador.h"
#define CONT_FILE_2 "Contador.c"
#define CONT_FILE_3 "CmdLine.h"
#define CONT_DEFAULT_NUM 33

//CONTADOR COMPARTIDO

typedef struct {
    SharedMemory shm_cont;
    Semaphore sem; //semaforo binario
    int crear_sem;
} Contador;

int Contador_crear(Contador *C, const char *filename);

int Contador_init_to_zero(Contador *C);

int Contador_incrementar(Contador *C);

int Contador_get(Contador *C);

int Contador_eliminar(Contador *C);

#endif //CONTADOR_H
