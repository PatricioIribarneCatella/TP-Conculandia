#include "Shell.h"

static char buffer[BUFLEN];

static char* read_line(const char* promt) {
	
	int i = 0,
            c = 0;

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

void Shell_run(pid_t f, Log* log, Contador* p) {

	char* cmd;

	printf(
		"\tConculandia - Shell\n"
		" Comandos:\n"
		"\texit: terminar la simulación\n"
		"\tgetper: devuelve la cantidad de personas que llegaron a la Frontera\n");

	while ((cmd = read_line(">")) != NULL) {
		
		if (strcmp(cmd, "exit") == 0) {

			Log_escribir(log, "KILL FRONTERA\n");

			// Signal a la frontera para
			// terminar de producir gente
			kill(f, SIGINT);
			
			// Se espera el término
			// del proceso Frontera
			waitpid(f, NULL, 0);

			printf("saliendo de la simulación\n");

			return;
		}

		if (strcmp(cmd, "getper") == 0)
			printf("Personas que llegaron a Conculandia: %d\n", Contador_get(p));
	}
}

