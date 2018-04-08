#include "Migraciones.h"

int Migraciones_run() {
	//Adquiero recursos
	Queue q;
	Queue_abrir(&q, FIFO_FILE, O_RDONLY);

	//SI LA VENTANILLA RECIBE UNA PERSONA CON ID -1 ES SEÑAL PARA DETENERSE
	int stop = 0;
	while (!stop) {
		Person p;
		int r = Queue_leer(&q, &p, sizeof(Person));
		if (r > 0) {
			// id es -1 es señal de terminar
			if (p.id == -1) {
				printf("VENTANILLA STOPED\n");
				stop = 1;
			}
			else {
				//si es un id valido imprimo (aca iria el procesamiento de la persona)
				printf(
					"Ventanilla pid: %d, Persona nacionalidad: %d - id: %d \n",
					getpid(), p.nacionalidad, p.id);
			}
		}
		else {
			printf("errno: %s \n", strerror(errno));
			stop = 1;
		}
	}

	//Libero recursos
	Queue_cerrar(&q);

	return 0;
}