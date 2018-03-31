#include "Queue.h"

int Queue_crear(Queue *Q, char *file_name) {
    if(0 == mknod(file_name, S_IFIFO |0666 , 0)){
        strcpy(Q->f_name,file_name);
        Q->fd = -1;
        return QUEUE_OK;
    }
    return ERROR_MKNOD;
}

int Queue_abrir(Queue *Q, char *file_name, int mode){
    Q->fd = open(file_name, mode);
    strcpy(Q->f_name,file_name);
    return Q->fd;
}

int Queue_leer(Queue *Q, void *ptr, size_t s){
    return read(Q->fd, ptr, s);
}

int Queue_escribir(Queue *Q, void *ptr, size_t s){
    return write(Q->fd, ptr, s);
}

int Queue_cerrar(Queue *Q){
    close(Q->fd);
    Q->fd = -1;
    return QUEUE_OK;
}

int Queue_eliminar(Queue *Q){
    unlink(Q->f_name);
    return QUEUE_OK;
}

