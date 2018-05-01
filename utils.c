#include "utils.h"

static char buf[32];
static char numeros[] = "0123456789";

char* itoa(short int val, int radix) {

	if (val == 0)
		return "0";

	int i;

	for (i = 30; val > 0 ; --i) {

		buf[i] = numeros[val % radix];
		val /= radix;
	}

	return &buf[i+1];
}

