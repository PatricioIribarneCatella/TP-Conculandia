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
	if (f < 0)
		return f;

	error = Log_escribir(log, "PROUCTOR PID: %d \n", f);
	if (error)
		return error;
	return f;
}

void Liberar_recursos(Log *log,
					  Queue *q,
					  Sellos *sellos,
					  Contador *extr_ingresados,
					  Contador *pers_deportadas,
					  Contador *pers_arrestadas,
					  PedidosCaptura *p_captura) {
	// Libero recursos
	PedidosCaptura_eliminar(p_captura);
	Contador_eliminar(pers_arrestadas);
	Contador_eliminar(pers_deportadas);
	Contador_eliminar(extr_ingresados);
	Queue_eliminar(q);
	Sellos_eliminar(sellos);
	Log_cerrar(log);
}

static int Ventanillas_init(Sellos *sellos,
							Contador *extr_ingresados,
							Contador *pers_deportadas,
							Contador *pers_arrestadas,
							PedidosCaptura *p_captura,
							Log *log,
							CmdLine *cl) {
	int error = 0;
	int i;
	pid_t *ventanillas_pids = malloc(sizeof(pid_t) * cl->ventanillas);
	if (ventanillas_pids == NULL)
		return errno;
	// Inicializo sellos (para las ventanillas)
	error = Sellos_crear(sellos, cl->sellos);

	if (error) {
		perror("Fallo al crear sellos");
		return error;
	}

	// Inicializa el contador de extranjeros deportados
	pers_deportadas->crear_sem = 1;
	error = Contador_crear(pers_deportadas, CONT_FILE_1);
	if (error) {
		perror("Fallo al crear contador personas deportadas");
		return error;
	}
	error = Contador_init_to_zero(pers_deportadas);
	if (error) {
		perror("Fallo al inicializar contador personas deportadas");
		return error;
	}

	// Inicializa el contador de residentes arrestadas
	pers_arrestadas->crear_sem = 1;

	error = Contador_crear(pers_arrestadas, CONT_FILE_2);
	if (error) {
		perror("Fallo al crear contador personas arrestadas");
		return error;
	}
	error = Contador_init_to_zero(pers_arrestadas);
	if (error) {
		perror("Fallo al inicializar contador personas arrestadas");
		return error;
	}

	// Inicializa el contador de extranjeros ingresados
	extr_ingresados->crear_sem = 1;
	error = Contador_crear(extr_ingresados, CONT_FILE_3);
	if (error) {
		perror("Fallo al crear contador extranjeros ingresados");
		return error;
	}
	error = Contador_init_to_zero(extr_ingresados);
	if (error) {
		perror("Fallo al inicializar contador extranjeros ingresados");
		return error;
	}

	// Inicializa los pedidos de captura
	error = PedidosCaptura_crear(p_captura, PCAPTURA_FILE);
	if (error) {
		perror("Fallo al crear pedidos de captura");
		return error;
	}
	error = PedidosCaptura_inicializar(p_captura);
	if (error) {
		perror("Fallo al inicializar pedidos de captura");
		return error;
	}

	// Ventanillas
	for (i = 0; i < cl->ventanillas; i++) {
		ventanillas_pids[i] = fork();
		if (ventanillas_pids[i] == 0) {
			Migraciones_run(sellos, i + 1, log);
			exit(EXIT_SUCCESS);
		}
		if (ventanillas_pids[i] < 0)
			break;
	}

	// Termino las ventanillas que fueron creadas correctamente y seteo error
	if (i < cl->ventanillas && ventanillas_pids[i] < 0) {
		for (int j = 0; j < i; j++)
			kill(SIGINT, ventanillas_pids[j]);
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

int Conculandia_init(CmdLine *cl,
					 Log *log,
					 Queue *q,
					 Sellos *sellos,
					 Contador *extr_ingresados,
					 Contador *pers_deportadas,
					 Contador *pers_arrestadas,
					 PedidosCaptura *p_captura,
					 pid_t *frontera) {
	int error;

	// Inicializa el Log
	error = Log_abrir(log, (const char *) &cl->log_filename, cl->debug);

	if (error) {
		perror("Fallo al abrir el log. Error");
		return error;
	}


	error = Log_escribir(log, "Sellos: %d, ventanillas: %d\n", cl->sellos,
						 cl->ventanillas);

	if (error) {
		perror("Fallo al escribir en el log. Error");
		return error;
	}


	// Inicializa y ejecuta la Frontera
	*frontera = Frontera_init(q, log);

	if (*frontera <= 0) {
		perror("Fallo al inicializar la frontera. Error");
		return error;
	}


	// Inicializa y ejecuta las Ventanillas
	error = Ventanillas_init(sellos, extr_ingresados, pers_deportadas,
							 pers_arrestadas, p_captura, log, cl);

	if (error) {
		kill(*frontera, SIGINT);
		return error;
	}


	// Ejecuta la Shell
	Shell_run(*frontera, log, extr_ingresados, pers_deportadas,
			  pers_arrestadas);

	Ventanillas_wait(cl->ventanillas);

	Log_escribir(log, "PERSONAS PROCESADAS :%d \n",
				 Contador_get(pers_deportadas));

	return 0;
}
