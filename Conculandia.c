#include "Conculandia.h"
#include "CmdLine.h"
#include "Contador.h"
#include "Frontera.h"
#include "Migraciones.h"
#include "Person.h"
#include "Queue.h"
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

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

	//Inicializo Cola
	Queue q;
	Queue_crear(&q, FIFO_FILE);

	//id del proceso que genera personas
	int producer_pid = 0;

	//fork - generador de personas
	int f = fork();
	if (f == 0) {
		Frontera_run(cl->ventanillas);
		exit(0);
	}
	else {
		producer_pid = f;
		printf("PROUCTOR PID: %d \n", f);
	}

	//Inicializo sellos (para las ventanillas)
	Sellos sellos;
	Sellos_crear(&sellos, cl->sellos);
	Contador cont_1;
	Contador_crear(&cont_1, CONT_FILE_1);

	//forks - ventanillas
	int i = 0;
	for (i = 0; i < cl->ventanillas; i++) {
		int g = fork();
		if (g == 0) {
			Migraciones_run(&sellos);
			exit(0);
		}
	}

	//busy wait, solo de prueba (aca iria la consola)
	sleep(3);

	printf("KILL PRODUCER\n");

	//signal al producer
	kill(producer_pid, SIGINT);

	//esperar a que todos los procesos terminen
	//ventanillas + 1(producer) - cambiar si se agregan otros procesos
	for (i = 0; i < cl->ventanillas + 1; i++) {
		wait(NULL);
	}

	printf("PERSONAS PROCESADAS :%d \n", Contador_get(&cont_1));

	//libero recursos
	Contador_eliminar(&cont_1);
	Queue_eliminar(&q);
	Sellos_eliminar(&sellos);
}
