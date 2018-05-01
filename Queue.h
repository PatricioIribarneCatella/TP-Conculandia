#ifndef QUEUE_H
#define QUEUE_H

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_FILE_NAME 128
#define QUEUE_OK 0
#define ERROR_MKNOD -1

//FIFO default file
#define FIFO_FILE "/tmp/fifo_f"

//FIFO usado como cola

typedef struct {
	char f_name[MAX_FILE_NAME];
	struct flock fl;
	int fd;
} Queue;


//Para el mismo archivo fifo un solo proceso debe llamar a crear
int Queue_crear(Queue *Q, char *file_name);

//Modos: O_WRONLY, O_RDONLY
int Queue_abrir(Queue *Q, char *file_name, int mode);

int Queue_leer(Queue *Q, void *ptr, size_t s);

int Queue_escribir(Queue *Q, void *ptr, size_t s);

int Queue_cerrar(Queue *Q);

//El proceso que llamo a crear deberia llamar a eliminiar
//aunque si otro lo llama no deberia haber problema
int Queue_eliminar(Queue *Q);


#endif  //QUEUE_H
