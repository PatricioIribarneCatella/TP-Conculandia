#include "Migraciones.h"

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla, Log *log) {
	//Adquiero recursos
	Queue q;
	Contador cont_personas;
    Contador cont_pers_arrest;
	PedidosCaptura p_captura;
	int r, stop = 0;

	Queue_abrir(&q, FIFO_FILE, O_RDONLY);
	Contador_crear(&cont_personas, CONT_FILE_1);
    Contador_crear(&cont_pers_arrest, CONT_FILE_2);
	PedidosCaptura_crear(&p_captura, PCAPTURA_FILE);

	while (!stop) {
		Person p;
		r = Queue_leer(&q, &p, sizeof(Person));

		if (r == sizeof(Person)) {
			if(Person_es_extranjero(&p)){
				//ENTRANJERO

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

			} else {
				//NATIVO
                Log_escribir(
                        log, "Ventanilla: %d, Ingreso de Persona con dni: %d\n",
                        numero_ventanilla, p.id);

				usleep(20000);

				//si tiene pedido va  ala comisaría
				if(PedidosCaptura_check_persona(&p_captura, &p)){
                    Contador_incrementar(&cont_pers_arrest);
					Log_escribir(
							log, "Ventanilla: %d, Persona con dni: %d, derivado a la Oficina de Policía\n",
							numero_ventanilla, p.id);
				} else {
					Log_escribir(
							log, "Ventanilla: %d, Persona con dni: %d, Feliz regreso a Conculandia\n",
                            numero_ventanilla, p.id);
				}

				//incremento el contador de personas procesadas
				Contador_incrementar(&cont_personas);
			}
		}
		else {
			if (r > 0)
				Log_escribir(log, "Bytes leidos incorrectos. Leidos: %d\n", r);
			stop = 1;
		}
	}

	Log_escribir(log, "Cerrando ventanilla n° %d\n", numero_ventanilla);

	//Libero recursos
	PedidosCaptura_eliminar(&p_captura);
    Contador_eliminar(&cont_pers_arrest);
	Contador_eliminar(&cont_personas);
	Queue_cerrar(&q);

	return 0;
}
