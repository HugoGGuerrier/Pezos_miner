#ifndef BLOC_H
#define BLOC_H

#include "utils.h"


// Encode a bloc to a binary
char *encode_bloc(Bloc_t bloc);

// Decode a binary to a bloc
Bloc_t decode_bloc(char *data);

// Verify a bloc correctness
int verify_bloc(Bloc_t bloc);

#endif