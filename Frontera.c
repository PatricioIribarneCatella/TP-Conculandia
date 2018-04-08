#include "Frontera.h"

static int quit = 0;

void Frontera_sigint_handler(int s) {
	printf("\n Frontera SIGINT recived \n");
	printf("\n Ejecutando directiva Donald Trump \n \n Levantando Muro \n \n");
	quit = 1;
}

int Frontera_run(int ventanillas) {
	//Handler señal
	signal(SIGINT, &Frontera_sigint_handler);

	//Adquiero recursos
	Queue q;
	Queue_abrir(&q, FIFO_FILE, O_WRONLY);

	//Mientras no haya problemas meto personas en la cola
	while (!quit) {
		Person p;
		Person_random_generate(&p);
		Queue_escribir(&q, &p, sizeof(Person));

		//duerme 0.1 segs para simular paso del tiempo
		//regular el tiempo de creacion de personas
		usleep(100000);
	}

	printf("PRODUCTOR TERMINANDO\n");

	//Envio a las ventanillas señal para terminar (persona con id -1)
	int i = 0;
	for (i = 0; i < ventanillas; i++) {
		Person p;
		p.id = -1;
		Queue_escribir(&q, &p, sizeof(Person));
	}

	//Libero recursos
	Queue_cerrar(&q);

	return 0;
}