#include "Conculandia.h"

void Conculandia_init(CmdLine* cl) {

	printf("Sellos: %d, ventanillas: %d\n",
		cl->sellos, cl->ventanillas);

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
}
