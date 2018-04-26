#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define LOCK_OK 0

typedef struct {
	int fd;
	int modoLectura;
} LockArchivo;

int LockArchivo_inicializar(LockArchivo *lock, char *filename, int modoLectura);

int LockArchivo_tomar(LockArchivo *lock);
int LockArchivo_liberar(LockArchivo *lock);

int LockArchivo_eliminar(LockArchivo *lock);
