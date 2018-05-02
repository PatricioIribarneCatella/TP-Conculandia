#include "Migraciones.h"

static int Adquirir_recursos(Queue *q, Contador *extr_ingresados,
							 Contador *pers_deportadas,
							 Contador *pers_arrestadas,
							 PedidosCaptura *p_captura,
							 RasgosDeRiesgoCompartidos *rasg_r_comp) {
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

	error = RasgosCompartidos_inicializar(rasg_r_comp, READ);

	return error;
}

static void Liberar_recursos(Contador *e_ing, Contador *p_arrest,
							 Contador *p_deport, RasgosDeRiesgoCompartidos *r,
							 PedidosCaptura *p, Queue *q) {
	Contador_eliminar(e_ing);
	Contador_eliminar(p_arrest);
	Contador_eliminar(p_deport);

	RasgosCompartidos_eliminar(r);
	PedidosCaptura_eliminar(p);

	Queue_cerrar(q);
}

static int Migraciones_procesar_extranjero(Sellos *s, int ventanilla,
										   RasgosDeRiesgoCompartidos *rasgos,
										   Person *p,
										   Contador *cont_pers_deport,
										   Contador *cont_extr_ingres, Log *l) {
	int error;
	char buffer[MAX_CHARS + 1];
	// Se chequean alertas de riesgo
	if (RasgosCompartidos_Persona_es_de_riesgo(rasgos, p)) {
		error = Contador_incrementar(cont_pers_deport);

		if (error) {
			Log_escribir(l,
						 "ERROR: fallo al incrementar el "
						 "contador de personas deportadas. "
						 "Ventanilla: %d",
						 ventanilla);
			return error;
		}
		itoa(Person_get_caracteristica_especial(p), buffer, 2);
		Log_escribir(
			l,
			"Ventanilla: %d, Persona con pasaporte: %d, DEPORTADO - "
			"Características: [ojos: %d, pelo: %d, sexo: %d, esp: %s]\n",
			ventanilla, p->id, Person_get_ojos(p), Person_get_pelo(p),
			Person_get_sexo(p), buffer);
	}
	else {
		//Tomo un sello
		error = Sellos_tomar_sello(s);

		if (error) {
			Log_escribir(l,
						 "ERROR: fallo al tomar un sello. "
						 "Ventanilla: %d",
						 ventanilla);
			return error;
		}

		// Se simula tiempo de procesamiento (0.02 seg)
		// Sellar el pasaporte
		usleep(20000);

		itoa(Person_get_caracteristica_especial(p), buffer, 2);

		Log_escribir(l,
					 "Ventanilla: %d, Persona con pasaporte: %d, "
					 "Características: [ojos: %d, pelo: %d, sexo: %d, esp: %s] "
					 "Bienvenido a Conculandia \n",
					 ventanilla, p->id, Person_get_ojos(p), Person_get_pelo(p),
					 Person_get_sexo(p), buffer);

		//Libero el sello
		error = Sellos_liberar_sello(s);

		if (error) {
			Log_escribir(l,
						 "ERROR: fallo al liberar un sello. "
						 "Ventanilla: %d",
						 ventanilla);
			return error;
		}

		error = Contador_incrementar(cont_extr_ingres);

		if (error) {
			Log_escribir(l,
						 "ERROR: fallo al incrementar el contador "
						 "de personas extranjeras ingresadas. "
						 "Ventanilla: %d",
						 ventanilla);
			return error;
		}
	}

	return 0;
}

static int Migraciones_procesar_residente(int ventanilla,
										  PedidosCaptura *p_captura, Person *p,
										  Contador *cont_pers_arrest, Log *l) {
	int error;

	Log_escribir(l, "Ventanilla: %d, Ingreso de Persona con dni: %d\n",
				 ventanilla, p->id);

	usleep(20000);

	// Si tiene pedido de captura va a la comisaría
	if (PedidosCaptura_check_persona(p_captura, p)) {
		error = Contador_incrementar(cont_pers_arrest);

		if (error) {
			Log_escribir(l,
						 "ERROR: fallo al incrementar el "
						 "contador de personas arrestadas. "
						 "Ventanilla: %d",
						 ventanilla);
			return error;
		}

		Log_escribir(l,
					 "Ventanilla: %d, Persona con dni: %d, "
					 "ARRESTADA -> Oficina de Policía\n",
					 ventanilla, p->id);
	}
	else {
		Log_escribir(l,
					 "Ventanilla: %d, Persona con dni: %d, Feliz "
					 "regreso a Conculandia\n",
					 ventanilla, p->id);
	}

	return 0;
}

int Migraciones_run(Sellos *sellos, unsigned int numero_ventanilla,
		Log *log, RasgosDeRiesgoCompartidos* rasg_r_comp) {
	Queue q;
	Contador cont_extr_ingres;
	Contador cont_pers_deport;
	Contador cont_pers_arrest;
	PedidosCaptura p_captura;
	int r, stop = 0, error;

	//Adquiero recursos
	stop = Adquirir_recursos(&q, &cont_extr_ingres, &cont_pers_deport,
							 &cont_pers_arrest, &p_captura, rasg_r_comp);
	while (!stop) {
		Person p;
		r = Queue_leer(&q, &p, sizeof(Person));

		if (r == sizeof(Person)) {
			if (Person_es_extranjero(&p))
				error = Migraciones_procesar_extranjero(
					sellos, numero_ventanilla, rasg_r_comp, &p,
					&cont_pers_deport, &cont_extr_ingres, log);
			else
				error = Migraciones_procesar_residente(
					numero_ventanilla, &p_captura, &p, &cont_pers_arrest, log);

			if (error)
				break;
		}
		else {
			if (r > 0)
				Log_escribir(log, "Bytes leidos incorrectos. Leidos: %d\n", r);
			stop = 1;
		}
	}

	if (error)
		Log_escribir(log, "ERROR: ventanilla n° %d\n", numero_ventanilla);

	Log_escribir(log, "Cerrando ventanilla n° %d\n", numero_ventanilla);

	Liberar_recursos(&cont_extr_ingres, &cont_pers_arrest, &cont_pers_deport,
					 rasg_r_comp, &p_captura, &q);

	return 0;
}
