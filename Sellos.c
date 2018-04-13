#include "Sellos.h"

int Sellos_crear(Sellos *S, int cant_s){
    if (cant_s < 1) return -1;
    Semaphore_init(&(S->sem), SELLOS_FILE_NAME, cant_s);
    return 0;
}

int Sellos_tomar_sello(Sellos *S){
    Semaphore_p(&(S->sem));
    return 0;
}

int Sellos_liberar_sello(Sellos *S){
    Semaphore_v(&(S->sem));
    return 0;
}

int Sellos_eliminar(Sellos *S){
    Semaphore_eliminar(&(S->sem));
    return 0;
}