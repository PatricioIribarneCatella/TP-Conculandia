#ifndef SELLOS_H
#define SELLOS_H

#include "Semaphore.h"

#define SELLOS_FILE_NAME "Sellos.h"

//SOLO PUEDE HABER UNA VARIABLE SELLOS

typedef struct {
    Semaphore sem;
} Sellos;

int Sellos_crear(Sellos *S, int cant_s);

int Sellos_tomar_sello(Sellos *S);

int Sellos_liberar_sello(Sellos *S);

int Sellos_eliminar(Sellos *S);

#endif //SELLOS_H
