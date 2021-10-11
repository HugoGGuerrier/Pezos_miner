#ifndef HASH_H
#define HASH_H

#include <stdlib.h>

// Hash an arbitrary piece of data with given size in a 32 byte blake2
void hash(const char *in, size_t size_in, char *out);

#endif