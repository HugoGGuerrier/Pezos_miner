#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "operation.h"


char *encode_operation(Operation_t operation) {
    // TODO
    return NULL;
}

Operation_t decode_operation(char *data) {
    // Get the tag from the operation
    unsigned short tag = *((unsigned short *)data);
    data += 2;

    // Prepare the data size
    unsigned short data_size;
    Operation_Type_t op_type;

    // Switch on the tag
    switch (tag) {
    case 1:
        op_type = BAD_PREDECESSOR;
        data_size = 32;
        break;

    case 2:
        op_type = BAD_TIMESTAMP;
        data_size = 8;
        break;

    case 3:
        op_type = BAD_OPERATIONS_HASH;
        data_size = 32;

    case 4:
        op_type = BAD_CONTEXT_HASH;
        data_size = 32;

    case 5:
        op_type = BAD_SIGNATURE;
        data_size = 0;
    
    default:
        printf("Error in operation decoding : Unknown operation tag\n");
        exit(1);
        break;
    }

    // Create the operation data
    char *op_data = (char *) malloc(data_size);
    memcpy(op_data, data, data_size);
    data += data_size;

    // Create the operation user key
    char *op_key = (char *) malloc(KEY_SIZE);
    memcpy(op_key, data, KEY_SIZE);
    data += KEY_SIZE;

    // Create the operation signature
    char *op_sig = (char *) malloc(SIG_SIZE);
    memcpy(op_sig, data, SIG_SIZE);
    data += SIG_SIZE;

    // Return the new operation
    return new_operation(op_type, data_size, op_data, op_key, op_sig);
}

Operations_t decode_operations(char *data, size_t size) {
    // Prepare the result and the cursor
    size_t cursor = 0;
    Operations_t res = NULL;

    while(cursor < size) {
        // Decode the first operation
        Operation_t op = decode_operation(data);

        // Add it to the list
        Operations_t tmp = new_operations(op, res);
        res = tmp;

        // Increment data and cursor
        cursor, data += OP_TAG_SIZE + op->data_size + KEY_SIZE + SIG_SIZE;
    }

    // Return the result
    return res;
}