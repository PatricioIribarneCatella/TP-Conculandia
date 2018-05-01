// clang-format off
#include "MinisterioSeguridad.h"

// Verifica si está en el rango [a, b)
#define IN_RANGE(X, a, b) (X >= a && X < b)

static int quit;

void MSeguridad_SIGINT_handler(int signum) {
	quit = 1;
}

static enum colores_ojos get_color_ojos(int r) {
	if (IN_RANGE(r, 0, 25))
		return OJOS_VERDES;
	if (IN_RANGE(r, 25, 50))
		return OJOS_AZULES;
	if (IN_RANGE(r, 50, 75))
		return OJOS_MARRONES_OSCUROS;
	return OJOS_MARRONES_CLAROS;
}

static enum tipos_pelos get_color_pelo(int r) {
	if (IN_RANGE(r, 0, 17))
		return PELO_RUBIO;
	if (IN_RANGE(r, 17, 33))
		return PELO_MOROCHO;
	if (IN_RANGE(r, 33, 50))
		return PELO_CASTANIO;
	if (IN_RANGE(r, 50, 67))
		return PELO_COLORADO;
	if (IN_RANGE(r, 67, 83))
		return PELO_CANOSO;
	return PELADO;
}

static enum sexos get_tipo_sexo(int r) {
	return (r >= 50) ? MUJER : HOMBRE;
}

static enum caracteristicas_especiales get_carac_esp(int r) {
	if (IN_RANGE(r, 0, 17))
		return GORRA;
	if (IN_RANGE(r, 17, 33))
		return TATUAJE_EN_ROSTRO;
	if (IN_RANGE(r, 33, 50))
		return TATUAJE_EN_PIERNAS;
	if (IN_RANGE(r, 50, 67))
		return AROS;
	if (IN_RANGE(r, 67, 83))
		return PIERCING_EN_NARIZ;
	return PIERCING_EN_BOCA;
}

static int generar_nueva_alerta(RasgosDeRiesgoCompartidos *rasgos, Log *l) {
	int rp_nuevo, rp_quitar, ro_nuevo, ro_quitar, rs_nuevo, rs_quitar,
		rce_nuevo, rce_quitar;
	int error;

	rp_nuevo = random() % 100;
	rp_quitar = random() % 100;
	ro_nuevo = random() % 100;
	ro_quitar = random() % 100;
	rs_nuevo = random() % 100;
	rs_quitar = random() % 100;
	rce_nuevo = random() % 100;
	rce_quitar = random() % 100;

	error = RasgosCompartidos_tomar_lock(rasgos, WRITE);

	if (error) {
		Log_escribir(l,
					 "ERROR: fallo al adquirir lock - Ministerio Seguridad\n");
		return error;
	}

	RasgosCompartidos_Aniadir_pelo(rasgos, get_color_pelo(rp_nuevo));
	RasgosCompartidos_Remover_pelo(rasgos, get_color_pelo(rp_quitar));

	RasgosCompartidos_Aniadir_ojos(rasgos, get_color_ojos(ro_nuevo));
	RasgosCompartidos_Remover_ojos(rasgos, get_color_ojos(ro_quitar));

	RasgosCompartidos_Aniadir_sexo(rasgos, get_tipo_sexo(rs_nuevo));
	RasgosCompartidos_Remover_sexo(rasgos, get_tipo_sexo(rs_quitar));

	RasgosCompartidos_Aniadir_caracteristica_especial(
		rasgos, get_carac_esp(rce_nuevo));

	RasgosCompartidos_Remover_caracteristica_especial(
		rasgos, get_carac_esp(rce_quitar));

	Log_escribir(l, "ALERTA GENERADA: [ojos: %d, pelo: %d, sexo: %d, esp: %s]\n",
			RasgosCompartidos_get_ojos(rasgos),
			RasgosCompartidos_get_pelo(rasgos),
			RasgosCompartidos_get_sexo(rasgos),
			RasgosCompartidos_get_caracteristica_especial(rasgos));

	error = RasgosCompartidos_liberar_lock(rasgos, WRITE);

	if (error)
		Log_escribir(l,
					 "ERROR: fallo al liberar lock - Ministerio Seguridad\n");

	return error ? error : 0;
}

int MinisterioSeguridad_run(Log *log) {
	int error;
	quit = 0;

	// Setea el handler
	struct sigaction act;
	memset(&act, 0, sizeof(act));
	act.sa_handler = MSeguridad_SIGINT_handler;
	sigfillset(&act.sa_mask);

	sigaction(SIGINT, &act, NULL);

	//Adquiero recursos
	RasgosDeRiesgoCompartidos ras_riesgo;
	error = RasgosCompartidos_crear(&ras_riesgo, WRITE);

	while (!quit && !error) {
		error = generar_nueva_alerta(&ras_riesgo, log);

		// Simula tiempo entre cada alerta
		// dentro de un uniforme (1, 10)
		sleep(random() % 10);
	}

	if (error)
		Log_escribir(log, "ERROR: fallo en las alertas\n");

	if (quit)
		Log_escribir(log, "Ministerio SIGINT recibido\n");

	Log_escribir(log, "PRODUCTOR ALERTAS TERMINADO\n");

	//Libero recursos
	RasgosCompartidos_eliminar(&ras_riesgo);

	return error ? error : 0;
}
