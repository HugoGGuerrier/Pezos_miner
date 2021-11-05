#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "block.h"
#include "message.h"
#include "operation.h"
#include "state.h"

#include "signature.h"
#include "hash.h"
#include "utils.h"


// ----- Memory manipulation funuctions

Block_t new_block(unsigned int level, char *pred, unsigned long time, char *ope_h, char *ctx_h, char *sig) {
    Block_t res = (Block_t) malloc(sizeof(struct block));
    res->level = level;
    res->predecessor = pred;
    res->timestamp = time;
    res->operations_hash = ope_h;
    res->context_hash = ctx_h;
    res->signature = sig;
    return res;
}

void delete_block(Block_t block) {
    free(block->predecessor);
    free(block->operations_hash);
    free(block->context_hash);
    free(block->signature);
    free(block);
}


// ----- Block encoding and decoding functions -----

char *encode_block(Block_t block) {
    // Prepare the result encoded data
    char *res = (char *) malloc(BLOCK_CODE_SIZE);

    // Prepare a copy of the address to iterate on
    char *ptr = res;

    // Copy the block level
    unsigned int real_lvl = (unsigned int) reverse_int(block->level);
    memcpy(ptr, &real_lvl, sizeof(int));
    ptr += sizeof(int);

    // Copy the block predecessor hash
    memcpy(ptr, block->predecessor, HASH_SIZE);
    ptr += HASH_SIZE;

    // Copy the block timestamp
    unsigned long real_ts = (unsigned long) reverse_long(block->timestamp);
    memcpy(ptr, &real_ts, sizeof(long));
    ptr += sizeof(long);

    // Copy the block operations hash
    memcpy(ptr, block->operations_hash, HASH_SIZE);
    ptr += HASH_SIZE;

    // Copy the block context hash
    memcpy(ptr, block->context_hash, HASH_SIZE);
    ptr += HASH_SIZE;

    // Copy the block signature
    memcpy(ptr, block->signature, SIG_SIZE);

    // Return the result
    return res;
}

Block_t decode_block(char *data) {
    // Declarations and allocations
    unsigned int level;
    char *predecessor = (char *) malloc(HASH_SIZE);
    unsigned long timestamp;
    char *operations_hash = (char *) malloc(HASH_SIZE);
    char *context_hash = (char *) malloc(HASH_SIZE);
    char *signature = (char *) malloc(SIG_SIZE);

    // Copy the block level
    memcpy(&level, data, sizeof(int));
    level = (unsigned int) reverse_int(level);
    data += sizeof(int);

    // Copy the block predecessor hash
    memcpy(predecessor, data, HASH_SIZE);
    data += HASH_SIZE;

    // Copy the block timestamp
    memcpy(&timestamp, data, sizeof(long));
    timestamp = (unsigned long) reverse_long(timestamp);
    data += sizeof(long);

    // Copy the block operations hash
    memcpy(operations_hash, data, HASH_SIZE);
    data += HASH_SIZE;

    // Copy the context hash
    memcpy(context_hash, data, HASH_SIZE);
    data += HASH_SIZE;

    // Copy the signature
    memcpy(signature, data, SIG_SIZE);

    // Build and return the block
    return new_block(level, predecessor, timestamp, operations_hash, context_hash, signature);
}


// ----- Block verification -----

Operation_t verify_bloc(Block_t b, Block_t pred, State_t state, Operations_t ops) {
    // Verifying timestamp
    long time = b->timestamp - pred->timestamp;
    if (time < BLOCK_TIME) {
        return new_bad_timestamp(state->predecessor_timestamp + BLOCK_TIME);
    }

    // Verifying predecessor
    char *pred_datas = encode_block(pred);
    char *pred_hash_res = (char *) malloc(HASH_SIZE);
    hash(pred_datas, BLOCK_CODE_SIZE, pred_hash_res);
    free(pred_datas);
    
    if (!compare_data(b->predecessor, HASH_SIZE, pred_hash_res, HASH_SIZE)) {
        return new_bad_predecessor(pred_hash_res);
    }
    free(pred_hash_res);

    // Verifying state hash
    char *state_datas = encode_state(state);
    char *state_hash_res = (char *) malloc(HASH_SIZE);
    hash(state_datas, STATE_CODE_SIZE_MIN + state->nb_account_bytes, state_hash_res);
    free(state_datas);
    
    if (!compare_data(b->context_hash, HASH_SIZE, state_hash_res, HASH_SIZE)) {
        return new_bad_context(state_hash_res);
    }
    free(state_hash_res);

    // Verifying signature
    char *block_data = encode_block(b);
    char *truncated_block_data = (char *) malloc(BLOCK_CODE_SIZE - SIG_SIZE);
    memcpy(truncated_block_data, block_data, BLOCK_CODE_SIZE - SIG_SIZE);
    free(block_data);

    if (!verify(b->signature, truncated_block_data, BLOCK_CODE_SIZE - SIG_SIZE, state->dictator_public_key)) {
        free(truncated_block_data);
        return new_bad_signature();
    }

    // Verifying operations hash
    // ... Unnecessary as ops hash must be faulty if everything else is fine
    // We do it last as it should be the most ressce-consumming verification
    char *ops_h = ops_hash(ops);
    return new_bad_operations(ops_h);
}


// ----- Utils functions -----

void print_block(Block_t block) {
    printf("--- BLOCK ---\n");
    printf("level : %u\n", block->level);
    printf("predecessor : ");
    print_hex(block->predecessor, HASH_SIZE, "\n");
    printf("timestamp : %lu\n", block->timestamp);
    printf("operations hash : ");
    print_hex(block->operations_hash, HASH_SIZE, "\n");
    printf("context hash : ");
    print_hex(block->context_hash, HASH_SIZE, "\n");
    printf("signature : ");
    print_hex(block->signature, SIG_SIZE, "\n");
    printf("-------------\n");
}
