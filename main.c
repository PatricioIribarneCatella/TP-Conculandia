#include "CmdLine.h"
#include "Conculandia.h"

int main(int argc, char *argv[]) {
	
	CmdLine cl;
	Log log;
	Queue q;
	Sellos sellos;
	Contador personas;
	Contador pers_arrestadas;
	Contador pers_deportadas;
	PedidosCaptura p_captura;
	RasgosDeRiesgoCompartidos rasgos_riesgo;
	pid_t frontera;

	CmdLine_parse(argc, argv, &cl);

	Conculandia_init(&cl, &log, &q, &sellos,
				&personas, &pers_arrestadas, &pers_deportadas, 
				&p_captura, &rasgos_riesgo, &frontera);

	Liberar_recursos(&log, &q, &sellos,
				&personas, &pers_arrestadas, &pers_deportadas,
				&p_captura, &rasgos_riesgo);

	return 0;
}
