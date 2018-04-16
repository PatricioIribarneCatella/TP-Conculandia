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

static void Ventanillas_init(Sellos *sellos,
							 Contador *personas,
							 Log *log,
							 CmdLine *cl) {
	// Inicializo sellos (para las ventanillas)
	Sellos_crear(sellos, cl->sellos);

	// Inicializa el contador de personas
	Contador_crear(personas, CONT_FILE_1);
	Contador_init_to_zero(personas);

	// Ventanillas
	for (int i = 0; i < cl->ventanillas; i++)
		if (fork() == 0) {
			Migraciones_run(sellos, i + 1, log);
			exit(EXIT_SUCCESS);
		}
}

static void Ventanillas_wait(int ventanillas) {
	
	// Esperar a que todas las ventanillas terminen
	for (int i = 0; i < ventanillas; i++)
		wait(NULL);
}

void Conculandia_init(CmdLine *cl) {
	Log log;
	Queue q;
	Sellos sellos;
	Contador personas;
	pid_t frontera;
	
	// Inicializa el Log
	Log_abrir(&log, (const char *) &cl->log_filename);
	
	Log_escribir(&log, "Sellos: %d, ventanillas: %d\n",
			cl->sellos, cl->ventanillas);

	// Inicializa y ejecuta la Frontera
	frontera = Frontera_init(&q, &log);
	
	// Inicializa y ejecuta las Ventanillas
	Ventanillas_init(&sellos, &personas, &log, cl);
	
	// Ejecuta la Shell
	Shell_run(frontera, &log, &personas);

	Ventanillas_wait(cl->ventanillas);

	Log_escribir(&log, "PERSONAS PROCESADAS :%d \n", Contador_get(&personas));

	// Libero recursos
	Contador_eliminar(&personas);
	Queue_eliminar(&q);
	Sellos_eliminar(&sellos);
	Log_cerrar(&log);
}
