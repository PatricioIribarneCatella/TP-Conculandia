#include "Migraciones.h"
static int Adquirir_recursos(Queue *q,
							 Contador *extr_ingresados,
							 Contador *pers_deportadas,
							 Contador *pers_arrestadas,
							 PedidosCaptura *p_captura,
                             RasgosDeRiesgoCompartidos *rasg_r_comp
) {
	int error, fd;

	fd = Queue_abrir(q, FIFO_FILE, O_RDONLY);
	error = fd < 0 ? fd : 0;

	if (error)
		return error;

    pers_deportadas->crear_sem = 0;
	error = Contador_crear(pers_deportadas, CONT_FILE_1);

	if (error)
		return error;

	pers_arrestadas->crear_sem = 0;
	error = Contador_crear(pers_arrestadas, CONT_FILE_2);

	if (error)
		return error;

	extr_ingresados->crear_sem = 0;
	error = Contador_crear(extr_ingresados, CONT_FILE_3);

	if (error)
		return error;

	error = PedidosCaptura_crear(p_captura, PCAPTURA_FILE);

    if (error)
        return error;

    error = RasgosCompartidos_crear(rasg_r_comp, O_RDONLY);

	return error;
}
int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla, Log *log) {
	//Adquiero recursos
	Queue q;
	Contador cont_extr_ingres;
	Contador cont_pers_deport;
	Contador cont_pers_arrest;
	PedidosCaptura p_captura;
    RasgosDeRiesgoCompartidos rasg_r_comp;
	int r, stop = 0, error;


	stop = Adquirir_recursos(&q, &cont_extr_ingres, &cont_pers_deport, &cont_pers_arrest, &p_captura, &rasg_r_comp);

	while (!stop) {
		Person p;
		r = Queue_leer(&q, &p, sizeof(Person));

		if (r == sizeof(Person)) {
			if (Person_es_extranjero(&p)) {
				//ENTRANJERO

                //Chequeo alertas
                if (RasgosCompartidos_Persona_es_de_riesgo(&rasg_r_comp, &p)){

                    //incremento el contador de extranjeros deportados
                    error = Contador_incrementar(&cont_pers_deport);
                    if (error)
                        break;

                    Log_escribir(
                            log,
                            "Ventanilla: %d, Persona con pasaporte: %d, deportado \n",
                            numero_ventanilla, p.id);
                } else {
                    //Si no lo fue deportado

                    //Tomo un sello
                    error = Sellos_tomar_sello(sellos);
                    if (error)
                        break;

                    //Simulo tiempo de procesamiento (0.02 seg)
                    //Sellar el pasaporte
                    usleep(20000);

                    Log_escribir(
                        log,
                        "Ventanilla: %d, Persona con pasaporte: %d, Bienvenido a Conculandia \n",
                        getpid(), p.nacionalidad, p.id);

                    //Libero el sello
                    error = Sellos_liberar_sello(sellos);
                    if (error)
                        break;

					error = Contador_incrementar(&cont_extr_ingres);
					if (error)
						break;
                }
			}
			else {
				//NATIVO
				Log_escribir(log,
							 "Ventanilla: %d, Ingreso de Persona con dni: %d\n",
							 numero_ventanilla, p.id);

				usleep(20000);

				// si tiene pedido va a la comisaría
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
			}
		}
		else {
			if (r > 0)
				Log_escribir(log, "Bytes leidos incorrectos. Leidos: %d\n", r);
			stop = 1;
		}
	}

	if (error)
		Log_escribir(log, "Hubo un error en la ventanilla n° %d\n",
					 numero_ventanilla);

	Log_escribir(log, "Cerrando ventanilla n° %d\n", numero_ventanilla);

	//Libero recursos
    RasgosCompartidos_destruir(&rasg_r_comp);
	PedidosCaptura_eliminar(&p_captura);
	Contador_eliminar(&cont_extr_ingres);
	Contador_eliminar(&cont_pers_arrest);
	Contador_eliminar(&cont_pers_deport);
	Queue_cerrar(&q);

	return 0;
}
