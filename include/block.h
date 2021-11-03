#ifndef BLOCK_H
#define BLOCK_H

#define BLOCK_TIME 600
#define BLOCK_CODE_SIZE (64 + 32 + 32 + 32 + 8 + 4)

#include "utils.h"


// ----- Block encoding and decoding functions -----

// Encode a block to a binary
char *encode_block(Block_t block);

// Decode a binary to a block
Block_t decode_block(char *data);


// ----- Verifying functions -----

// Verify a block correctness
Operation_t verify_bloc(Block_t block, Block_t pred, State_t state);

#endif