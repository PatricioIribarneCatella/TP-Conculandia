#define LOCK_OK 0
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>



typedef struct {
    int fd;
    int modoLectura;
} LockArchivo;



int LockArchivo_inicializar(LockArchivo* lock, char* filename, int modoLectura);

int LockArchivo_tomar(LockArchivo* lock);
int LockArchivo_liberar(LockArchivo* lock);

int LockArchivo_eliminar(LockArchivo* lock);