#include "PedidosCaptura.h"

static int cant_capturas = 0;

int PedidosCaptura_crear(PedidosCaptura *PC, const char *filename){
    if(!cant_capturas)
        cant_capturas = random() % 100;

    int r = ShareMem_crear(&(PC->shm_pcaptura), sizeof(int) * cant_capturas, filename, PCAPTURA_DEFAULT_NUM);
    if(r != SHM_OK) return r;

    PC->cant_pedidos = cant_capturas;
    return PCAPTURA_OK;
}

int PedidosCaptura_inicializar(PedidosCaptura *PC){
    //genero los dnis con pedidos de captura
    int i = 0;
    int dni = DNI_START;
    for (i = 0; i < PC->cant_pedidos; i++){
        dni = dni + random() % 55;
        ShareMem_escribir(&(PC->shm_pcaptura), &dni, i, sizeof(int));
    }
    return PCAPTURA_OK;
}

int PedidosCaptura_check_persona(PedidosCaptura *PC, Person *P){
    int i = 0;
    int dni = 0;

    //recorro la mem compartida, leo cada entrada y comparo con la id de la persona
    for (i = 0; i < PC->cant_pedidos; i++){
        ShareMem_leer(&(PC->shm_pcaptura), &dni, i, sizeof(int));
        if(P->id == dni) {
            return 1;
        }
    }

    return 0;
}

int PedidosCaptura_eliminar(PedidosCaptura *PC){
    ShareMem_liberar(&(PC->shm_pcaptura));
    return PCAPTURA_OK;
}
