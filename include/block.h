#ifndef BLOCK_H
#define BLOCK_H

#include "utils.h"

#define BLOCK_TIME 600
#define BLOCK_CODE_SIZE (SIG_SIZE + HASH_SIZE * 3 + 8 + 4)


// ----- Block encoding and decoding functions -----

// Encode a block to a binary
char *encode_block(Block_t block);

// Decode a binary to a block
Block_t decode_block(char *data);


// ----- Verifying functions -----

// Verify a block correctness
Operation_t verify_bloc(Block_t block, Block_t pred, State_t state, Operations_t ops);

#endif