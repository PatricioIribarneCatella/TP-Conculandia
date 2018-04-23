#include "Frontera.h"

static int quit;

void SIGINT_handler(int signum) {
	quit = 1;
}

int Frontera_run(Log *log) {
	int error;
	// Setea el handler para
	// la señal SIGINT de terminar
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIGINT_handler;
	sigfillset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	//Adquiero recursos
	Queue q;
	error = Queue_abrir(&q, FIFO_FILE, O_WRONLY);

	int p_created = 0;

	//Mientras no haya problemas meto personas en la cola
	while (!quit && !error) {
		Person p;
		Person_random_generate(&p);

		Queue_escribir(&q, &p, sizeof(Person));

		p_created++;

		//duerme 0.01 segs para simular paso del tiempo
		//regular el tiempo de creacion de personas
		usleep(10000);
	}

	if (quit)
		Log_escribir(log, "Frontera SIGINT received\n");

	Log_escribir(log, "PRODUCTOR TERMINANDO \n");
	Log_escribir(log, "%d PERSONAS ENTRARON POR LA FRONTERA \n\n", p_created);

	//Libero recursos
	Queue_cerrar(&q);

	return 0;
}
