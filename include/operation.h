#ifndef OPERATION_H
#define OPERATION_H

#define OP_TAG_SIZE 2

#include "utils.h"


// ----- Operation specific creation functions ------

// Create a new bad predecessor operation with the correct hash
Operation_t new_bad_predecessor(char *pred_hash);

// Create a new bad timestamp operation with the correct timestamp
Operation_t new_bad_timestamp(unsigned long timestamp);

// Create a new bad operations hash operation with the correct op hash
Operation_t new_bad_operations(char *op_hash);

// Create a new bad context hash operation with the correct context hash
Operation_t new_bad_context(char *ctx_hash);

// Create a new bad signature
Operation_t new_bad_signature();


// ----- Operations encoding and decoding functions -----

// Encode an operation to binary
char *encode_operation(Operation_t operation);

// Decode binary to operation
Operation_t decode_operation(char *data);

// Decode the operations and return a reversed list of them
Operations_t decode_operations(char *data, size_t data_size);


// ----- Utils functions -----

// Get the string for an operation type
char *op_type_str(const Operation_Type_t type);

#endif