#ifndef LOCK_ARCHIVO_H
#define LOCK_ARCHIVO_H

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LOCK_OK 0
#define WRITE 0
#define READ 1

typedef struct {
	int fd;
	int modoLectura;
	struct flock fl;
} LockArchivo;

int LockArchivo_inicializar(LockArchivo *lock, char *filename, int modoLectura);

int LockArchivo_tomar(LockArchivo *lock);
int LockArchivo_liberar(LockArchivo *lock);

int LockArchivo_eliminar(LockArchivo *lock);

#endif  // LOCK_ARCHIVO_H
