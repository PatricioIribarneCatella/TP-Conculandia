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

	int p_created = 0;

	//Mientras no haya problemas meto personas en la cola
	while (!quit) {
		Person p;
		Person_random_generate(&p);
		Queue_escribir(&q, &p, sizeof(Person));

		p_created = p_created + 1;

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