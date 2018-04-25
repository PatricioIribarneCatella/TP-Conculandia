#include "Sellos.h"

int Sellos_crear(Sellos *S, int cant_s){
    if (cant_s < 1)
        return -1;
    return Semaphore_init(&(S->sem), SELLOS_FILE_NAME, cant_s, 1);
}

int Sellos_tomar_sello(Sellos *S){
    return Semaphore_p(&(S->sem));
}

int Sellos_liberar_sello(Sellos *S){
    return Semaphore_v(&(S->sem));
}

int Sellos_eliminar(Sellos *S){
    return Semaphore_eliminar(&(S->sem));
}
