#include "Frontera.h"

#define BUFLEN 200

static int quit;

void SIGINT_handler(int signum) {
	char buf[BUFLEN] = {0};
	snprintf(buf, sizeof buf, "\n %s \n", "Frontera SIGINT recived");
	write(STDOUT_FILENO, buf, sizeof buf);
	quit = 1;
}

int Frontera_run(void) {

	// Setea el handler para
	// la señal SIGINT de terminar
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = SIGINT_handler;
	sigfillset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	//Adquiero recursos
	Queue q;
	Queue_abrir(&q, FIFO_FILE, O_WRONLY);

	int p_created = 0;

	//Mientras no haya problemas meto personas en la cola
	while (!quit) {
		
		Person p;
		Person_random_generate(&p);
		
		Queue_escribir(&q, &p, sizeof(Person));

		p_created++;

		//duerme 0.01 segs para simular paso del tiempo
		//regular el tiempo de creacion de personas
		usleep(10000);
	}

	printf("PRODUCTOR TERMINANDO \n");
	printf("%d PERSONAS ENTRARON POR LA FRONTERA \n\n", p_created);

	//Libero recursos
	Queue_cerrar(&q);

	return 0;
}
