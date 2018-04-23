#include "Migraciones.h"
static int Adquirir_recursos(Queue* q, Contador* personas, Contador* pers_arrestadas, PedidosCaptura* p_captura) {
	int error, fd;

	fd = Queue_abrir(q, FIFO_FILE, O_RDONLY);
	error = fd < 0 ? fd : 0;
	if (error)
		return error;
	personas->crear_sem = 0;
	error = Contador_crear(personas, CONT_FILE_1);
	if (error)
		return error;

	pers_arrestadas->crear_sem = 0;
	error = Contador_crear(pers_arrestadas, CONT_FILE_2);
	if (error)
		return error;

	error = PedidosCaptura_crear(p_captura, PCAPTURA_FILE);
	return error;

}
int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla, Log *log) {
	//Adquiero recursos
	Queue q;
	Contador cont_personas;
	Contador cont_pers_arrest;
	PedidosCaptura p_captura;
	int r, stop = 0, error;


	stop = Adquirir_recursos(&q, &cont_personas, &cont_pers_arrest, &p_captura);

	while (!stop) {
		Person p;
		r = Queue_leer(&q, &p, sizeof(Person));

		if (r == sizeof(Person)) {
			if (Person_es_extranjero(&p)) {
				//ENTRANJERO

				//Tomo un sello
				error = Sellos_tomar_sello(sellos);
				if (error)
					break;

				//si es un id valido imprimo (aca iria el procesamiento de la persona)
				Log_escribir(
					log,
					"Ventanilla pid: %d, Persona nacionalidad: %d - id: %d \n",
					getpid(), p.nacionalidad, p.id);

				//Simulo tiempo de procesamiento (0.02 seg)
				usleep(20000);

				//incremento el contador de personas procesadas
				error = Contador_incrementar(&cont_personas);
				if (error)
					break;

				//Libero el sello
				error = Sellos_liberar_sello(sellos);
				if (error)
					break;
			}
			else {
				//NATIVO
				Log_escribir(log,
							 "Ventanilla: %d, Ingreso de Persona con dni: %d\n",
							 numero_ventanilla, p.id);

				usleep(20000);

				//si tiene pedido va  ala comisaría
				if (PedidosCaptura_check_persona(&p_captura, &p)) {
					error = Contador_incrementar(&cont_pers_arrest);
					if (error)
						break;
					Log_escribir(log,
								 "Ventanilla: %d, Persona con dni: %d, "
								 "derivado a la Oficina de Policía\n",
								 numero_ventanilla, p.id);
				}
				else {
					Log_escribir(log,
								 "Ventanilla: %d, Persona con dni: %d, Feliz "
								 "regreso a Conculandia\n",
								 numero_ventanilla, p.id);
				}

				//incremento el contador de personas procesadas
				error = Contador_incrementar(&cont_personas);
				if (error)
					break;
			}
		}
		else {
			if (r > 0)
				Log_escribir(log, "Bytes leidos incorrectos. Leidos: %d\n", r);
			stop = 1;
		}
	}

	if (error)
		Log_escribir(log, "Hubo un error en la ventanilla n° %d\n", numero_ventanilla);

	Log_escribir(log, "Cerrando ventanilla n° %d\n", numero_ventanilla);

	//Libero recursos
	PedidosCaptura_eliminar(&p_captura);
	Contador_eliminar(&cont_pers_arrest);
	Contador_eliminar(&cont_personas);
	Queue_cerrar(&q);

	return 0;
}
