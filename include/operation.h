#ifndef OPERATION_H
#define OPERATION_H

#include "utils.h"


// Encode an operation to binary
char *encode_operation(Operation_t operation);

// Decode binary to operation
Operation_t decode_operation(char *data);

#endif