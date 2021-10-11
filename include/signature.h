#ifndef SIGNATURE_H
#define SIGNATURE_H

#include <stdlib.h>

// Sign an arbitrary data and put it in the out buffer
void sign(char *signature, const char *data, size_t data_size);

// Verify a signature
void verify(const char *signature, const char *data, const char *key);

#endif