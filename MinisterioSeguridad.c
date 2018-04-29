#include "MinisterioSeguridad.h"

static int quit;

void MSeguridad_SIGINT_handler(int signum) {
	quit = 1;
}

int MinisterioSeguridad_run(Log *log) {
	int error;
	quit = 0;

	// Setea el handler
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = MSeguridad_SIGINT_handler;
	sigfillset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	//Adquiero recursos
	RasgosDeRiesgoCompartidos ras_riesgo;
	error = RasgosCompartidos_crear(&ras_riesgo, 0);

	while (!quit && !error) {
		//FUNCION QUE AÑADA O REMUEVA AL AZAR;

		sleep(3);
	}

	if (quit)
		Log_escribir(log, "Ministerio SIGINT recibido\n");

	Log_escribir(log, "PRODUCTOR ALERTAS TERMINADO\n");

	//Libero recursos
	RasgosCompartidos_eliminar(&ras_riesgo);

	return 0;
}
