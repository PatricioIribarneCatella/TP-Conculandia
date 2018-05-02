#ifndef PEDIDOSCAPTURA_H
#define PEDIDOSCAPTURA_H

#include "Person.h"
#include "SharedMemory.h"

#define PCAPTURA_DEFAULT_NUM 56
#define PCAPTURA_FILE "PedidosCaptura.h"
#define PCAPTURA_OK 0

typedef struct {
    SharedMemory shm_pcaptura;
    int cant_pedidos;
} PedidosCaptura;

int PedidosCaptura_crear(PedidosCaptura *PC, const char *filename);

int PedidosCaptura_inicializar(PedidosCaptura *PC);

//retorna 1 si el dni de la persona esta en la lista
//0 caso contrario
int PedidosCaptura_check_persona(PedidosCaptura *PC, Person *P);

int PedidosCaptura_eliminar(PedidosCaptura *PC);

#endif //PEDIDOSCAPTURA_H
