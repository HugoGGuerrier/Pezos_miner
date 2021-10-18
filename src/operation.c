#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "operation.h"


char *encode_operation(Operation_t operation) {
    // Prepare the result
    char *res = (char *) malloc(OP_TAG_SIZE + operation->data_size + KEY_SIZE + SIG_SIZE);
    char *ptr = res;

    // Prepare the operation type
    unsigned short tag;

    // Switch the operation type
    switch (operation->op_type) {
    case BAD_PREDECESSOR:
        tag = reverse_short(1);
        break;

    case BAD_TIMESTAMP:
        tag = reverse_short(2);
        break;

    case BAD_OPERATIONS_HASH:
        tag = reverse_short(3);
        break;

    case BAD_CONTEXT_HASH:
        tag = reverse_short(4);
        break;

    case BAD_SIGNATURE:
        tag = reverse_short(5);
        break;
    
    default:
        printf("Error in operation encoding : Unknown operation tag\n");
        exit(1);
        break;
    }

    // Copy the tag in the operation
    memcpy(ptr, &tag, OP_TAG_SIZE);
    ptr += OP_TAG_SIZE;

    // Copy the operation data
    memcpy(ptr, operation->data, operation->data_size);
    ptr += operation->data_size;

    // Copy the user key
    memcpy(ptr, operation->user_key, KEY_SIZE);
    ptr += KEY_SIZE;

    // Copy the signature
    memcpy(ptr, operation->signature, SIG_SIZE);

    // Return the result
    return res;
}

Operation_t decode_operation(char *data) {
    // Get the tag from the operation
    unsigned short tag = reverse_short(*((unsigned short *)data));
    data += OP_TAG_SIZE;

    // Prepare the data size
    unsigned short data_size;
    Operation_Type_t tag;

    // Switch on the tag
    switch (tag) {
    case 1:
        tag = BAD_PREDECESSOR;
        data_size = 32;
        break;

    case 2:
        tag = BAD_TIMESTAMP;
        data_size = 8;
        break;

    case 3:
        tag = BAD_OPERATIONS_HASH;
        data_size = 32;

    case 4:
        tag = BAD_CONTEXT_HASH;
        data_size = 32;

    case 5:
        tag = BAD_SIGNATURE;
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
    return new_operation(tag, data_size, op_data, op_key, op_sig);
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