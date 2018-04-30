#include "LockArchivo.h"

int LockArchivo_inicializar(LockArchivo *lock, char *filename,
							int modoLectura) {
	int flags;

	if (modoLectura)
		flags = O_RDONLY;
	else
		flags = O_WRONLY;

	lock->fd = open(filename, flags | O_CREAT, 0644);
	lock->modoLectura = modoLectura;

	lock->fl.l_len = 0;
	lock->fl.l_start = 0;
	lock->fl.l_whence = SEEK_SET;
	lock->fl.l_type = modoLectura ? F_RDLCK : F_WRLCK;

	return lock->fd < 0 ? lock->fd : LOCK_OK;
}

int LockArchivo_tomar(LockArchivo *lock) {
	int return_value;

	return_value = fcntl(lock->fd, F_SETLKW, &(lock->fl));

	return return_value < 0 ? return_value : 0;
}

int LockArchivo_liberar(LockArchivo *lock) {
	int return_value;

	lock->fl.l_type = F_UNLCK;

	return_value = fcntl(lock->fd, F_SETLK, &(lock->fl));

	return return_value < 0 ? return_value : 0;
}

int LockArchivo_eliminar(LockArchivo *lock) {
	int return_value = close(lock->fd);
	lock->fd = -1;
	return return_value;
}
