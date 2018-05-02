#ifndef MINISTERIOSEGURIDAD_H
#define MINISTERIOSEGURIDAD_H

#include <signal.h>
#include <unistd.h>

#include "Log.h"
#include "RasgosDeRiesgoCompartidos.h"

int MinisterioSeguridad_run(Log *log, RasgosDeRiesgoCompartidos* r);

#endif  //MINISTERIOSEGURIDAD_H
