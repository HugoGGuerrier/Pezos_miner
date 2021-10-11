#include "hash.h"
#include "blake2.h"

void hash(const char *in, size_t size_in, char *out) {
    blake2b((void *)out, 32, (void *)in, size_in, NULL, 0);
}