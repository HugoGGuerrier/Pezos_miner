#ifndef BLOCK_H
#define BLOCK_H

#include "utils.h"

#define BLOCK_CODE_SIZE (64 + 32 + 32 + 32 + 8 + 4)

// Encode a block to a binary
char *encode_block(Block_t block);

// Decode a binary to a block
Block_t decode_block(char *data);

// Verify a block correctness
Operation_t verify_bloc(Block_t block);

#endif