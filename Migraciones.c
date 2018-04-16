#include "Migraciones.h"

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla, Log *log) {
	//Adquiero recursos
	Queue q;
	Contador cont_personas;
	int r, stop = 0;

	Queue_abrir(&q, FIFO_FILE, O_RDONLY);
	Contador_crear(&cont_personas, CONT_FILE_1);

	while (!stop) {
		Person p;
		r = Queue_leer(&q, &p, sizeof(Person));

		if (r == sizeof(Person)) {
			//Tomo un sello
			Sellos_tomar_sello(sellos);

			//si es un id valido imprimo (aca iria el procesamiento de la persona)
			Log_escribir(
				log, "Ventanilla pid: %d, Persona nacionalidad: %d - id: %d \n",
				getpid(), p.nacionalidad, p.id);

			//Simulo tiempo de procesamiento (0.02 seg)
			usleep(20000);

			//incremento el contador de personas procesadas
			Contador_incrementar(&cont_personas);

			//Libero el sello
			Sellos_liberar_sello(sellos);
		}
		else {
			if (r > 0)
				Log_escribir(log, "Bytes leidos incorrectos. Leidos: %d\n", r);
			stop = 1;
		}
	}

	Log_escribir(log, "Cerrando ventanilla n° %d\n", numero_ventanilla);

	//Libero recursos
	Contador_eliminar(&cont_personas);
	Queue_cerrar(&q);

	return 0;
}
