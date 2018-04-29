#include "Shell.h"

static char buffer[BUFLEN];

static char *read_line(const char *promt) {
	int i = 0, c = 0;

	memset(buffer, 0, BUFLEN);

	fprintf(stdout, "%s", promt);

	c = getchar();

	while (c != END_LINE && c != EOF) {
		buffer[i++] = c;
		c = getchar();
	}

	if (c == EOF)
		return NULL;

	buffer[i] = END_STRING;

	return buffer;
}

void Shell_run(pid_t f, pid_t m, Log *log, Contador *e_ingres,
			   Contador *p_deport, Contador *p_arrest) {
	char *cmd;

	printf(
		"\tConculandia - Shell\n"
		" Comandos:\n"
		"\texit: terminar la simulación\n"
		"\tgetperd: cantidad de extranjeros deportadas\n"
		"\tgetpera: cantidad de residentes derivados a la Oficina de Policía\n"
		"\tgeteing: cantidad de extranjeros que ingresaron a Conculandia\n");

	while ((cmd = read_line(">")) != NULL) {
		if (strcmp(cmd, "exit") == 0) {
			Log_escribir(log, "KILL FRONTERA\n");

			// Signal a la frontera para
			// terminar de producir gente
			kill(f, SIGINT);

			// Signal al ministerio para
			// terminar de producir alertas
			kill(m, SIGINT);

			// Se espera el término
			// del proceso Frontera
			waitpid(f, NULL, 0);

			// Se espera al término
			// del proceso Ministerio
			waitpid(m, NULL, 0);

			printf("Saliendo de la simulación\n");

			return;
		}

		if (strcmp(cmd, "getperd") == 0)
			printf("Extranjeros que fueron deportadas: %d\n",
				   Contador_get(p_deport));

		if (strcmp(cmd, "getpera") == 0)
			printf(
				"Residentes que llegaron y fueron derivadas a la Oficina de "
				"Policía: %d\n",
				Contador_get(p_arrest));

		if (strcmp(cmd, "geteing") == 0)
			printf("Extranjeros que ingresarosn a Conculandia: %d\n",
				   Contador_get(e_ingres));
	}
}
