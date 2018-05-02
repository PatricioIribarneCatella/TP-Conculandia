#include "utils.h"

static char numeros[] = "0123456789ABCDEF";

void itoa(short int val, char* buf, int radix) {
	char aux_buf[MAX_CHARS + 1];

	if (val == 0) {
		strcpy(buf, "0");
		return;
	}

	memset(aux_buf, 0, sizeof(char) * (MAX_CHARS + 1));

	int i;

	for (i = MAX_CHARS - 2; val > 0 ; --i) {

		aux_buf[i] = numeros[val % radix];
		val /= radix;
	}
	
	strcpy(buf, &(aux_buf[i+1]));
}

