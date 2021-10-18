#include <stdlib.h>

#include "block.h"


char *encode_block(Block_t block) {
    // Prepare the result encoded data
    char *data_res = malloc(BLOCK_CODE_SIZE);

    // Prepare a copy of the address to iterate on
    char *data_ptr = data_res;

    // Copy memory for each block field

    memcpy(data_ptr, block->level, 4);
    data_ptr = data_ptr + 4;

    memcpy(data_ptr, block->predecessor, 32);
    data_ptr = data_ptr + 32;

    memcpy(data_ptr, block->timestamp, 8);
    data_ptr = data_ptr + 8;

    memcpy(data_ptr, block->operations_hash, 32);
    data_ptr = data_ptr + 32;

    memcpy(data_ptr, block->context_hash, 32);
    data_ptr = data_ptr + 4;

    memcpy(data_ptr, block->signature, 64);

    // Return the result
    return data_res;
}

Block_t decode_block(char *data) {

    // Declarations and allocations

    int level;
    char * predecessor = malloc (32);
    long timestamp = 0;
    char * operations_hash = malloc (32);
    char * context_hash = malloc (32);
    char * signature = malloc (64);

    // Copy memory for each block field

    memcpy(&level, data, 4);
    data = data + 4; 

    memcpy(predecessor, data, 32);
    data = data + 32;

    memcpy(&timestamp, data, 8);
    data = data + 8; 

    memcpy(operations_hash, data,32);
    data = data + 32;

    memcpy(context_hash, data, 32);
    data = data + 32;

    memcpy(signature, data, 64);

    // Build and return the block
    return new_block(level, predecessor, timestamp, operations_hash, operations_hash, signature);
}

char *verify_block(Block_t block) {
    //TODO
    return NULL;
}