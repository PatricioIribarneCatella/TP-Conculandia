#ifndef UTILS_H
#define UTILS_H

#include <string.h>

#define MAX_CHARS 32
// Takes an short int and prints the binary representation of it in tbe buffer.
// NOTE: The buffer must be at least MAX_CHARS+1 chars wide(+1 for NULL terminating purposes)
void itoa(short int val, char* buf, int radix);

#endif // UTILS_H
