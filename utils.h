#ifndef UTILS_H
#define UTILS_H

#include <string.h>

#define MAX_CHARS 32

// Takes a short int and prints 
// the binary representation of it in the buffer.
// 
// NOTE: The buffer must be at least 
// 	(MAX_CHARS + 1) chars wide (+1 for NULL terminating purposes)
void itoa(short int val, char* buf, int radix);

#endif // UTILS_H
