#ifndef BLOCK_H
#define BLOCK_H

#include "state.h"
#include "operation.h"

#define BLOCK_TIME 600
#define BLOCK_CODE_SIZE (SIG_SIZE + HASH_SIZE * 3 + sizeof(long) + sizeof(int))


// ----- Types and enum defining -----

// --- Type that represents a block
typedef struct block *Block_t;

struct block
{
    unsigned int level;
    char *predecessor;
    unsigned long timestamp;
    char *operations_hash;
    char *context_hash;
    char *signature;
};

// ----- Memory manipulation functions -----

// Create a new block from the given data
Block_t new_block(unsigned int level, char *pred, unsigned long time, char *ope_h, char *ctx_h, char *sig);

// Delete a block
void delete_block(Block_t block);


// ----- Block encoding and decoding functions -----

// Encode a block to a binary
char *encode_block(Block_t block);

// Decode a binary to a block
Block_t decode_block(char *data);


// ----- Verifying functions -----

// Verify a block correctness
Operation_t verify_bloc(Block_t block, Block_t pred, State_t state, Operations_t ops);


// ----- Utils functions -----

// Print a block
void print_block(Block_t block);

#endif