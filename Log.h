#ifndef LOG_H
#define LOG_H

#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LOG_OK 0
#define ERROR_LOG_OPEN -1
#define ERROR_LOG_WRITE -2
#define ERROR_LOG_WRITE_MSGTOOLONG -3

#define MSG_MAX_SIZE 512

typedef struct {
	struct flock fl;
	int fd;
	int debug;
} Log;

int Log_abrir(Log *LG, const char *filename, int debug);

int Log_escribir(Log *LG, const char *msg, ...);

int Log_cerrar(Log *LG);

#endif  //LOG_H
