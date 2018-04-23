#include "Conculandia.h"

static int Frontera_init(Queue *q, Log *log) {
	pid_t f;

	// Inicializo Cola
	Queue_crear(q, FIFO_FILE);

	// Generador de personas
	if ((f = fork()) == 0) {
		Frontera_run(log);
		exit(EXIT_SUCCESS);
	}

	Log_escribir(log, "PROUCTOR PID: %d \n", f);

	return f;
}

void Liberar_recursos(Log* log,
                                Queue* q,
                                Sellos* sellos,
                                Contador* personas,
                                Contador* pers_arrestadas,
                                PedidosCaptura* p_captura) {

    // Libero recursos
    PedidosCaptura_eliminar(p_captura);
    Contador_eliminar(pers_arrestadas);
    Contador_eliminar(personas);
    Queue_eliminar(q);
    Sellos_eliminar(sellos);
    Log_cerrar(log);
}

static int Ventanillas_init(Sellos *sellos,
							 Contador *personas,
							 Contador *pers_arrestadas,
							 PedidosCaptura *p_captura,
							 Log *log,
							 CmdLine *cl) {
    int error = 0;
    int i;
    pid_t* ventanillas_pids = malloc(sizeof(pid_t) * cl->ventanillas);
    if (ventanillas_pids == NULL)
        return errno;

	// Inicializo sellos (para las ventanillas)
    error = Sellos_crear(sellos, cl->sellos);

    if (error)
        return error;

    // Inicializa el contador de personas
    error = Contador_crear(personas, CONT_FILE_1);
    if (error)
        return error;
    error = Contador_init_to_zero(personas);
    if (error)
        return error;

	// Inicializa el contador de residentes arrestadas
    error = Contador_crear(pers_arrestadas, CONT_FILE_2);
    if (error)
        return error;
    error = Contador_init_to_zero(pers_arrestadas);
    if (error)
        return error;

	// Inicializa los pedidos de captura
    error = PedidosCaptura_crear(p_captura, PCAPTURA_FILE);
    if (error)
        return error;
    error = PedidosCaptura_inicializar(p_captura);
    if (error)
        return error;

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
    if(ventanillas_pids[i] < 0) {
        for (int j = 0; j < i; j++)
            kill(SIGINT, ventanillas_pids[j]);
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
                     Log* log,
                     Queue* q,
                     Sellos* sellos,
                     Contador* personas,
                     Contador* pers_arrestadas,
                     PedidosCaptura* p_captura,
                     pid_t* frontera) {
	int error;

    // Inicializa el Log
    error = Log_abrir(log, (const char *) &cl->log_filename);

    if (error) {
        perror("Fallo al abrir el log. Error: ");
        return error;
    }


	error = Log_escribir(log, "Sellos: %d, ventanillas: %d\n", cl->sellos,
				 cl->ventanillas);

    if (error) {
        perror ("Fallo al escribir en el log. Error: ");
        return error;
    }


	// Inicializa y ejecuta la Frontera
	*frontera = Frontera_init(q, log);

    if (*frontera < 0) {
        perror("Fallo al inicializar la frontera. Error: ");
        return error;
    }


	// Inicializa y ejecuta las Ventanillas
	error = Ventanillas_init(sellos, personas, pers_arrestadas, p_captura, log, cl);

    if (error) {
        perror ("Fallo al inicializar las ventanillas. Error: ");
        return error;
    }


	// Ejecuta la Shell
	Shell_run(*frontera, log, personas, pers_arrestadas);

	Ventanillas_wait(cl->ventanillas);

	Log_escribir(log, "PERSONAS PROCESADAS :%d \n", Contador_get(personas));

    return 0;
}
