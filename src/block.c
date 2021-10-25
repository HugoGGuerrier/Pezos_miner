#include <stdlib.h>

#include "block.h"
#include "client.h"
#include "message.h"
#include "operation.h"
#include "signature.h"
#include "utils.h"


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


// --- Block verification

unsigned char *ops_hash(Operations_t ops) {
    unsigned char *res_buf = malloc(32);
    if (ops->head == NULL) {
        for (int i=0; i < 32; i++) {
            res_buf[i] = 0;
        }
    } else {
        Operation_t op = ops->head;
        if (ops->tail == NULL) {
            blake2b((void *) res_buf, 32, (void *) encode_operation(op), (2 + op->data_size), NULL, 0);
        } else {
            unsigned char *rest_hash = ops_hash(ops->tail); 
            unsigned char *head_hash = malloc(32); 
            blake2b((void *) head_hash, 32, (void *) encode_operation(op), (2 + op->data_size), NULL, 0);
            unsigned char *tmp_buf = malloc(64); 
            for (int i=0; i < 32; i++) {
                tmp_buf[i] = rest_hash[i];
                tmp_buf[i+32] = head_hash[i];
            }
            blake2b((void *) res_buf, 32, (void *) tmp_buf, 64, NULL, 0);
            free(rest_hash);
            free(head_hash);
            free(tmp_buf);
        }
    }
    return res_buf;
}

Operation_t verify_bloc(Block_t b) {
    Block_t pred = get_block(send_message_with_response(new_get_block_message(b->level - 1)));
    State_t state = get_state(send_message_with_response(new_get_state_message(b->level)));

    // Verifying timestamp
    long time = b->timestamp - pred->timestamp;
    if (time < 600) {
        return new_bad_timestamp(state->predecessor_timestamp + 10);
    }

    // Verifying predecessor
    char *pred_datas = encode_block(pred);
    unsigned char *pred_hash_res = malloc(32);
    blake2b((void *) pred_hash_res, 32, (void *) pred_datas, 172, NULL, 0);
    if (!compare_data(b->predecessor, pred_hash_res, 32, 32)) {
        return new_bad_predecessor(pred_hash_res);
    }
    free(pred_hash_res);

    // Verifying state hash
    char *state_datas = encode_state(state);
    unsigned char *state_hash_res = malloc(32);
    blake2b((void *) state_hash_res, 32, (void *) state_datas, (44 + state->nb_account_bytes), NULL, 0);
    if (compare_data(b->context_hash, state_hash_res, 32, 32)) {
        return new_bad_context(state_hash_res);
    }
    free(state_hash_res);

    // Verifying signature
    unsigned char *block_data = encode_block(b);
    unsigned char *truncated_block_data = malloc(108);
    memcpy(truncated_block_data, block_data, 108);
    free(block_data);
    if (!verify(b->signature, truncated_block_data, 108, state->dictator_public_key)) {
        free(truncated_block_data);
        return new_bad_signature();
    }
    free(truncated_block_data);

    // Verifying operations hash
    // ... Unnecessary as ops hash must be faulty if everything else is fine
    // We do it last as it should be the most ressce-consumming verification
    Operations_t ops = get_operations(b->level);
    unsigned char *ops_h = ops_hash(ops); 
    return new_bad_operations(ops_h);
}