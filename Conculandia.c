#include "Conculandia.h"

static int Frontera_init(Queue *q, Log *log) {
	pid_t f;
	int error;

	// Inicializo Cola
	error = Queue_crear(q, FIFO_FILE);

	if (error)
		return error;

	// Generador de personas
	if ((f = fork()) == 0) {
		Frontera_run(log);
		exit(EXIT_SUCCESS);
	}

	if (f < 0) {
		Queue_eliminar(q);
		return f;
	}

	error = Log_escribir(log, "PROUCTOR PERSONAS PID: %d\n", f);

	if (error)
		return error;

	return f;
}

static int Ministerio_init(Log *log, RasgosDeRiesgoCompartidos* rasgos) {
	pid_t m;
	int error;

	// Generador de alertas
	if ((m = fork()) == 0) {
		MinisterioSeguridad_run(log, rasgos);
		exit(EXIT_SUCCESS);
	}

	if (m < 0)
		return m;

	error = Log_escribir(log, "PRODUCTOR ALERTAS PID: %d\n", m);

	return error ? error : m;
}

static int Contador_personas_init(Contador *c, const char *f, char *cont_name,
								  Log *l) {
	int res;
	char buf[100];

	c->crear_sem = 1;
	res = Contador_crear(c, f);

	if (res) {
		snprintf(buf, sizeof buf, "ERROR: Fallo al crear contador %s ",
				 cont_name);
		Log_escribir(l, buf);
		perror(buf);
		return res;
	}

	res = Contador_init_to_zero(c);

	if (res) {
		snprintf(buf, sizeof buf, "ERROR: Fallo al inicializar contador %s ",
				 cont_name);
		Log_escribir(l, buf);
		perror(buf);
		return res;
	}

	return res;
}

static int Ventanillas_init(Sellos *sellos, Contador *extr_ingresados,
					Contador *pers_deportadas,
					Contador *pers_arrestadas,
					PedidosCaptura *p_captura, RasgosDeRiesgoCompartidos* rasgos,
					Log *log, CmdLine *cl) {
	int i, error = 0;
	pid_t *ventanillas_pids;

	// Inicializo sellos
	error = Sellos_crear(sellos, cl->sellos);

	if (error) {
		Log_escribir(log, "ERROR: Fallo al crear sellos");
		perror("Fallo al crear sellos");
		return error;
	}

	error = Contador_personas_init(pers_deportadas, CONT_FILE_1,
								   "personas deportadas", log);

	if (error) 
		return error;


	error = Contador_personas_init(pers_arrestadas, CONT_FILE_2,
								   "personas arrestadas", log);

	if (error) 
		return error;

	error = Contador_personas_init(extr_ingresados, CONT_FILE_3,
								   "extranjeros ingresados", log);

	if (error) 
		return error;

	// Inicializa los pedidos de captura
	error = PedidosCaptura_crear(p_captura, PCAPTURA_FILE);

	if (error) {
		perror("Fallo al crear pedidos de captura");
		Log_escribir(log, "ERROR: Fallo al crear pedidos de captura");
		return error;
	}

	error = PedidosCaptura_inicializar(p_captura);

	if (error) {
		perror("Fallo al inicializar pedidos de captura");
		Log_escribir(log, "ERROR: Fallo al inicializar pedidos de captura");
		return error;
	}

	ventanillas_pids = malloc(sizeof(pid_t) * cl->ventanillas);

	if (ventanillas_pids == NULL) {
		error = errno;
		Log_escribir(log, "ERROR: Fallo en malloc - ventanillas_pid");
		return error;
	}

	// Ventanillas
	for (i = 0; i < cl->ventanillas; i++) {
		ventanillas_pids[i] = fork();

		if (ventanillas_pids[i] == 0) {
			Migraciones_run(sellos, i + 1, log, rasgos);
			exit(EXIT_SUCCESS);
		}

		if (ventanillas_pids[i] < 0)
			break;
	}

	// Termino las ventanillas que fueron creadas correctamente y seteo error
	if (i < cl->ventanillas && ventanillas_pids[i] < 0) {
		for (int j = 0; j < i; j++)
			kill(SIGINT, ventanillas_pids[j]);

		Log_escribir(log, "ERROR: Fallo al inicializar las ventanillas");
		perror("Error al inicializar las ventanillas");
		error = ventanillas_pids[i];
	}

	free(ventanillas_pids);

	return error;
}

static void Ventanillas_wait(int ventanillas) {
	// Esperar a que todas las ventanillas terminen
	for (int i = 0; i < ventanillas; i++)
		wait(NULL);
}


static int Conculandia_init(pid_t *frontera, pid_t *ministerio, CmdLine *cl,
							Log *log, Queue *q, Sellos *sellos,
							Contador *extr_ingresados,
							Contador *pers_deportadas,
							Contador *pers_arrestadas,
							PedidosCaptura *p_captura,
							RasgosDeRiesgoCompartidos* rasgos_riesgo) {
	int error;

	// Inicializa el Log
	error = Log_abrir(log, (const char *) &cl->log_filename, cl->debug);

	if (error) {
		perror("ERROR: Fallo al abrir el log ");
		return error;
	}

	error = Log_escribir(log, "Sellos: %d, ventanillas: %d\n", cl->sellos,
						 cl->ventanillas);

	if (error) {
		perror("ERROR: Fallo al escribir en el log ");
		return error;
	}

	// Inicializa y ejecuta la Frontera
	*frontera = Frontera_init(q, log);

	if (*frontera < 0) {
		Log_escribir(log, "ERROR: Fallo al crear la Frontera\n");
		perror("ERROR: Fallo al crear la Frontera ");
		return (*frontera);
	}

	// Crea e Inicializa los Rasgos de Riesgo	
	error = RasgosCompartidos_crear(rasgos_riesgo);
	
	if (error) {
		Log_escribir(log, "ERROR: Fallo al inicializar los rasgos compartidos ");
		return error;
	}

	// Inicializa y ejecuta el
	// Ministerio de Seguridad
	*ministerio = Ministerio_init(log, rasgos_riesgo);

	if (*ministerio < 0) {
		kill(*frontera, SIGINT);
		Log_escribir(log, "ERROR: Fallo al crear el Ministerio de Seguridad\n");
		perror("ERROR: Fallo al crear el Ministerio de Seguridad ");
		return error;
	}

	// Inicializa y ejecuta las Ventanillas
	error = Ventanillas_init(sellos,
			extr_ingresados, pers_deportadas, pers_arrestadas,
			p_captura, rasgos_riesgo, log, cl);

	if (error) {
		kill(*frontera, SIGINT);
		kill(*ministerio, SIGINT);
		return error;
	}

	return 0;
}

static void Liberar_recursos(Log *log, Queue *q, Sellos *sellos,
							 Contador *extr_ingresados,
							 Contador *pers_deportadas,
							 Contador *pers_arrestadas,
							 PedidosCaptura *p_captura,
							 RasgosDeRiesgoCompartidos* rasgos_riesgo) {
	// Libero recursos
	Contador_eliminar(pers_arrestadas);
	Contador_eliminar(pers_deportadas);
	Contador_eliminar(extr_ingresados);
	PedidosCaptura_eliminar(p_captura);
	RasgosCompartidos_eliminar(rasgos_riesgo);
	Sellos_eliminar(sellos);
	Queue_eliminar(q);
	Log_cerrar(log);
}

void Conculandia_run(CmdLine *cl) {
	int error;
	pid_t frontera, ministerio;
	Log log;
	Queue q;
	Sellos sellos;
	Contador extr_ingresados;
	Contador pers_deportadas;
	Contador pers_arrestadas;
	PedidosCaptura p_captura;
	RasgosDeRiesgoCompartidos rasgos_riesgo;

	// Inicializa las estructuras y procesos de Conculandia:
	// - Frontera
	// - Ventanillas
	// - Ministerio de Seguridad
	// No libera los recursos en caso de error, se liberan luego
	error = Conculandia_init(&frontera, &ministerio, cl, &log, &q, &sellos,
							 &extr_ingresados, &pers_deportadas,
							 &pers_arrestadas, &p_captura, &rasgos_riesgo);


	if (!error) {
	
		// Ejecuta la Shell
		Shell_run(frontera, ministerio, &log, &extr_ingresados, &pers_deportadas,
				&pers_arrestadas);

		Ventanillas_wait(cl->ventanillas);

		Log_escribir(&log,
					"PERSONAS DEPORTADAS: %d\n"
					"PERSONAS ARRESTADAS: %d\n"
					"PERSONAS EXTRANJERAS INGRESADAS: %d\n",
					Contador_get(&pers_deportadas), Contador_get(&pers_arrestadas),
					Contador_get(&extr_ingresados));

	}


	Liberar_recursos(&log, &q, &sellos, &extr_ingresados, &pers_deportadas,
					 &pers_arrestadas, &p_captura, &rasgos_riesgo);
}
