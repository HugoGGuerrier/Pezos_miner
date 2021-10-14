#ifndef BLOCK_H
#define BLOCK_H

#include "utils.h"


// Encode a block to a binary
char *encode_block(Block_t block);

// Decode a binary to a block
Block_t decode_block(char *data);

// Verify a block correctness
int verify_block(Block_t block);

#endif