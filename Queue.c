#include "Queue.h"

int Queue_crear(Queue *Q, char *file_name) {
	if (0 == mknod(file_name, S_IFIFO | 0666, 0)) {
		strcpy(Q->f_name, file_name);
		Q->fd = -1;
		return QUEUE_OK;
	}
	return ERROR_MKNOD;
}

int Queue_abrir(Queue *Q, char *file_name, int mode) {
	Q->fl.l_type = F_RDLCK;
	Q->fl.l_whence = SEEK_SET;
	Q->fl.l_start = 0;
	Q->fl.l_len = 0;
	Q->fd = open(file_name, mode);
	strcpy(Q->f_name, file_name);
	return Q->fd;
}

int Queue_leer(Queue *Q, void *ptr, size_t s) {
	//Adquiero el lock
	Q->fl.l_type = F_RDLCK;
	fcntl(Q->fd, F_SETLKW, &(Q->fl));

	int r_bytes = read(Q->fd, ptr, s);

	//Libero el lock
	Q->fl.l_type = F_UNLCK;
	fcntl(Q->fd, F_SETLKW, &(Q->fl));

	return r_bytes;
}

int Queue_escribir(Queue *Q, void *ptr, size_t s) {
	return write(Q->fd, ptr, s);
}

int Queue_cerrar(Queue *Q) {
	close(Q->fd);
	Q->fd = -1;
	return QUEUE_OK;
}

int Queue_eliminar(Queue *Q) {
	unlink(Q->f_name);
	return QUEUE_OK;
}
