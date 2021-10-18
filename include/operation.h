#ifndef OPERATION_H
#define OPERATION_H

#define OP_TAG_SIZE 2

#include "utils.h"


// Encode an operation to binary
char *encode_operation(Operation_t operation);

// Decode binary to operation
Operation_t decode_operation(char *data);

// Decode the operations
Operations_t decode_operations(char *data, size_t data_size);

#endif