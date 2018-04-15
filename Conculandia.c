#include "Conculandia.h"

void Conculandia_init(CmdLine *cl) {
	printf("Sellos: %d, ventanillas: %d\n", cl->sellos, cl->ventanillas);

	/*
	 * 1 - realizar fork() para mantener la simulación en segundo plano
	 * 2 - realizar un segundo fork() para generar un proceso que produzca
	 * 	las personas que van llegando
	 * 2 - en primer plano dejar la 'consola' recibiendo
	 * 	las preguntas del enunciado
	 * 3 - en la simulación lanzar 'v' procesos para simular 
	 * 	las distintas ventanillas
	 * 4 - cada ventanilla tiene que leer de un 'fifo' tal vez,
	 * 	las personas que van llegando, y tomar un sello (adquirir un lock),
	 * 	realizar el procesamiento de la persona (extranjero o residente),
	 * 	liberar el lock, y volver a empezar el ciclo.
	 * 5 - las peticiones desde la consola se podrían enviar, tal vez, por pipe()
	 * 	hacia el proceso de la simulación
	 * */

	Queue q;
	Sellos sellos;
	Contador cont_personas;
	pid_t f, g, producer_pid = 0;
	int i;

	// Inicializo Cola
	Queue_crear(&q, FIFO_FILE);

	// Generador de personas
	f = fork();
	if (f == 0) {
		Frontera_run();
		exit(EXIT_SUCCESS);
	}
	else {
		producer_pid = f;
		printf("PROUCTOR PID: %d \n", f);
	}

	// Inicializo sellos (para las ventanillas)
	Sellos_crear(&sellos, cl->sellos);

	// Inicializa el contador de personas
	Contador_crear(&cont_personas, CONT_FILE_1);
    Contador_init_to_zero(&cont_personas);

	// Ventanillas
	for (i = 0; i < cl->ventanillas; i++) {
		g = fork();
		if (g == 0) {
			Migraciones_run(&sellos, i + 1);
			exit(EXIT_SUCCESS);
		}
	}

	//busy wait, solo de prueba (aca iria la consola)
	sleep(3);

	printf("KILL PRODUCER\n");

	//signal al producer
	kill(producer_pid, SIGINT);

	//esperar a que todos los procesos terminen
	//ventanillas + 1(producer) - cambiar si se agregan otros procesos
	for (i = 0; i < cl->ventanillas + 1; i++)
		wait(NULL);

	printf("PERSONAS PROCESADAS :%d \n", Contador_get(&cont_personas));

	// Libero recursos
	Contador_eliminar(&cont_personas);
	Queue_eliminar(&q);
	Sellos_eliminar(&sellos);
}
