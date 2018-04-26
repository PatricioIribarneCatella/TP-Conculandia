#include "CmdLine.h"
#include "Conculandia.h"
#include "Log.h"

int main(int argc, char *argv[]) {
	CmdLine cl;
	Log log;
	Queue q;
	Sellos sellos;
	Contador personas;
	Contador pers_arrestadas;
	PedidosCaptura p_captura;
	pid_t frontera;

	CmdLine_parse(argc, argv, &cl);

	Conculandia_init(&cl, &log, &q, &sellos, &personas, &pers_arrestadas,
					 &p_captura, &frontera);

	Liberar_recursos(&log, &q, &sellos, &personas, &pers_arrestadas,
					 &p_captura);

	return 0;
}
