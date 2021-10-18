#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "operation.h"


char *encode_operation(Operation_t operation) {
    // TODO
    return NULL;
}

Operation_t decode_operation(char *data) {
    // TODO
    return NULL;
}

Operations_t decode_operations(char *data, size_t size) {
    // Prepare the result and the cursor
    size_t cursor = 0;
    Operations_t res = NULL;

    while(cursor < size) {
        // Get the operation tag
        short tag;
        memcpy(&tag, data, TAG_SIZE);
        tag = reverse_short(tag);

        // Prepare the data size
        unsigned short data_size;

        // Switch on the tag
        switch (tag) {
        case 1:
            data_size = 32;
            break;

        case 2:
            data_size = 8;
            break;

        case 3:
            data_size = 32;

        case 4:
            data_size = 32;

        case 5:
            data_size = 0;
        
        default:
            printf("Error in operation decoding : Unknown operation tag\n");
            exit(1);
            break;
        }

        // Get the operation data
        data_size += sizeof(tag) + KEY_SIZE + SIG_SIZE;
        char *op_data = malloc(data_size);
        memcpy(op_data, data, data_size);

        // Decode the operation
        Operation_t op = decode_operation(op_data);

        // Free the allocated data
        free(op_data);

        // Add the operation to the result
        Operations_t tmp = new_operations(op, res);
        res = tmp;

        // Increase the data and cursor
        data += data_size;
        cursor += data_size;
    }

    // Return the result
    return res;
}