#include "LockArchivo.h"

int LockArchivo_inicializar(LockArchivo *lock,
							char *filename,
							int modoLectura) {
	int flags;
	if (modoLectura)
		flags = O_RDONLY;
	else
		flags = O_WRONLY;
	lock->fd = open(filename, flags | O_CREAT);
	lock->modoLectura = modoLectura;
	return lock->fd < 0 ? lock->fd : LOCK_OK;
}

int LockArchivo_tomar(LockArchivo *lock) {
	int return_value;
	struct flock fl;
	fl.l_len = 0;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	if (lock->modoLectura)
		fl.l_type = F_RDLCK;
	else
		fl.l_type = F_WRLCK;
	return_value = fcntl(lock->fd, F_SETLK, &fl);
	return return_value < 0 ? return_value : 0;
}

int LockArchivo_liberar(LockArchivo *lock) {
	struct flock fl;
	int return_value;
	fl.l_type = F_UNLCK;

	return_value = fcntl(lock->fd, F_SETLK, &fl);
	return return_value < 0 ? return_value : 0;
}

int LockArchivo_eliminar(LockArchivo *lock) {
	int return_value = close(lock->fd);
	lock->fd = -1;
	return return_value;
}

