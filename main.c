#include "CmdLine.h"
#include "Conculandia.h"

int main(int argc, char *argv[]) {
	CmdLine cl;
	Log log;
	Queue q;
	Sellos sellos;
	Contador extr_ingresados;
	Contador pers_deportadas;
	Contador pers_arrestadas;
	PedidosCaptura p_captura;

	CmdLine_parse(argc, argv, &cl);

	Conculandia_init(&cl, &log, &q, &sellos, &extr_ingresados, &pers_deportadas,
					 &pers_arrestadas, &p_captura);

	Liberar_recursos(&log, &q, &sellos, &extr_ingresados, &pers_deportadas,
					 &pers_arrestadas, &p_captura);

	return 0;
}
