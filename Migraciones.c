#include "Migraciones.h"

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla) {
	//Adquiero recursos
	Queue q;
	Queue_abrir(&q, FIFO_FILE, O_RDONLY);
	Contador cont_1;
	Contador_crear(&cont_1, CONT_FILE_1);

	int stop = 0;
	while (!stop) {
		Person p;
		int r = Queue_leer(&q, &p, sizeof(Person));
		if (r == sizeof(Person)) {
			//Tomo un seloo
			Sellos_tomar_sello(sellos);

			//si es un id valido imprimo (aca iria el procesamiento de la persona)
			printf("Ventanilla pid: %d, Persona nacionalidad: %d - id: %d \n",
				   getpid(), p.nacionalidad, p.id);

			//Simulo tiempo de procesamiento (0.05 seg)
			usleep(50000);

			//incremento el contador de personas procesadas
			Contador_incrementar(&cont_1);

			//Libero el sello
			Sellos_liberar_sello(sellos);
		}
		else {
			if (r > 0)
				printf("Cantidad de bytes leidos incorrectos.Bytes leidos: %d\n",
					   r);
			stop = 1;
		}
	}

	printf("Cerrando ventanilla n° %d\n", numero_ventanilla);
	//Libero recursos
	Contador_eliminar(&cont_1);
	Queue_cerrar(&q);

	return 0;
}