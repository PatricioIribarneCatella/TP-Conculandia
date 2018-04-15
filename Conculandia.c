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

void Conculandia_init(CmdLine *cl) {
	Log log;
	Queue q;
	Sellos sellos;
	Contador personas;
	pid_t frontera;

	Log_abrir(&log, (const char *) &cl->log_filename);
	Log_escribir(&log, "Sellos: %d, ventanillas: %d\n", cl->sellos,
				 cl->ventanillas);

	frontera = Frontera_init(&q, &log);

	Ventanillas_init(&sellos, &personas, &log, cl);


	//busy wait, solo de prueba (aca iria la consola)
	sleep(3);


	Log_escribir(&log, "KILL PRODUCER\n");

	// Signal a la frontera para
	// terminar de producir gente
	kill(frontera, SIGINT);

	// esperar a que todos los procesos terminen
	// ventanillas + 1(producer) - cambiar si se agregan otros procesos
	for (int i = 0; i < cl->ventanillas + 1; i++)
		wait(NULL);

	Log_escribir(&log, "PERSONAS PROCESADAS :%d \n", Contador_get(&personas));

	// Libero recursos
	Contador_eliminar(&personas);
	Queue_eliminar(&q);
	Sellos_eliminar(&sellos);
	Log_cerrar(&log);
}
