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


// ----- Block encoding and decoding functions -----

char *encode_block(Block_t block) {
    // Prepare the result encoded data
    char *data_res = (char *) malloc(BLOCK_CODE_SIZE);

    // Prepare a copy of the address to iterate on
    char *data_ptr = data_res;

    // Copy the block level
    unsigned int real_lvl = reverse_int(block->level);
    memcpy(data_ptr, &real_lvl, 4);
    data_ptr += 4;

    // Copy the block predecessor hash
    memcpy(data_ptr, block->predecessor, HASH_SIZE);
    data_ptr += HASH_SIZE;

    // Copy the block timestamp
    unsigned long real_ts = reverse_long(block->timestamp);
    memcpy(data_ptr, &real_ts, sizeof(long));
    data_ptr += sizeof(long);

    // Copy the block operations hash
    memcpy(data_ptr, block->operations_hash, HASH_SIZE);
    data_ptr += HASH_SIZE;

    // Copy the block context hash
    memcpy(data_ptr, block->context_hash, HASH_SIZE);
    data_ptr += HASH_SIZE;

    // Copy the block signature
    memcpy(data_ptr, block->signature, SIG_SIZE);

    // Return the result
    return data_res;
}

Block_t decode_block(char *data) {
    // Declarations and allocations
    unsigned int level;
    char *predecessor = (char *) malloc(HASH_SIZE);
    unsigned long timestamp;
    char *operations_hash = (char *) malloc(HASH_SIZE);
    char *context_hash = (char *) malloc (HASH_SIZE);
    char *signature = (char *) malloc (SIG_SIZE);

    // Copy the block level
    memcpy(&level, data, sizeof(int));
    level = reverse_int(level);
    data += sizeof(int);

    // Copy the block predecessor hash
    memcpy(predecessor, data, HASH_SIZE);
    data += HASH_SIZE;

    // Copy the block timestamp
    memcpy(&timestamp, data, sizeof(long));
    timestamp = reverse_long(timestamp);
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

char *ops_hash(Operations_t ops) {
    char *res_buf = (char *) malloc(HASH_SIZE);

    // If the head is NULL, just set the buffer to 0
    if (ops->head == NULL) {
        memset(res_buf, 0, HASH_SIZE);
    } 
    
    // Else process the ops hashing
    else {
        // Get the current operation
        Operation_t op = ops->head;
        char *op_code = encode_operation(op);
        size_t op_code_size = OP_CODE_SIZE_MIN + op->data_size;

        // If the tail is null, just return the head hash
        if (ops->tail == NULL) {
            hash(op_code, op_code_size, res_buf);
        }
        
        // Else recursively process the list
        else {
            // Get the hash of the tail and prepare the hash of the head
            char *rest_hash = ops_hash(ops->tail); 
            char *head_hash = (char *) malloc(HASH_SIZE);

            // Hash the head
            hash(op_code, op_code_size, head_hash);

            // Create a temporary buffer to concatenate the hashs
            char *tmp_buf = (char *) malloc(HASH_SIZE * 2);
            memcpy(tmp_buf, rest_hash, HASH_SIZE);
            memcpy(tmp_buf + HASH_SIZE, head_hash, HASH_SIZE);
            
            // Hash the concatenation
            hash(tmp_buf, HASH_SIZE * 2, res_buf);

            // Free the memory
            free(rest_hash);
            free(head_hash);
            free(tmp_buf);
        }

        // Free the operation code
        free(op_code);
    }

    // Return the result
    return res_buf;
}

Operation_t verify_bloc(Block_t b, Block_t pred, State_t state, Operations_t ops) {
    // Block_t pred = get_block(send_message_with_response(new_get_block_message(b->level - 1)));
    // State_t state = get_state(send_message_with_response(new_get_state_message(b->level)));
    // Operations_t ops = get_operations(send_message_with_response(new_get_block_operations_message(b->level)));

    // Verifying timestamp
    long time = b->timestamp - pred->timestamp;
    if (time < BLOCK_TIME) {
        return new_bad_timestamp(state->predecessor_timestamp + BLOCK_TIME);
    }

    // Verifying predecessor
    char *pred_datas = encode_block(pred);
    char *pred_hash_res = (char *) malloc(HASH_SIZE);
    hash(pred_datas, BLOCK_CODE_SIZE, pred_hash_res);
    
    if (!compare_data(b->predecessor, HASH_SIZE, pred_hash_res, HASH_SIZE)) {
        return new_bad_predecessor(pred_hash_res);
    }
    free(pred_datas);

    // Verifying state hash
    char *state_datas = encode_state(state);
    char *state_hash_res = (char *) malloc(HASH_SIZE);
    hash(state_datas, STATE_CODE_SIZE_MIN + state->nb_account_bytes, state_hash_res);
    
    if (!compare_data(b->context_hash, HASH_SIZE, state_hash_res, HASH_SIZE)) {
        return new_bad_context(state_hash_res);
    }
    free(state_datas);

    // Verifying signature
    char *block_data = encode_block(b);
    char *truncated_block_data = (char *) malloc(BLOCK_CODE_SIZE - SIG_SIZE);

    memcpy(truncated_block_data, block_data, BLOCK_CODE_SIZE - SIG_SIZE);
    if (!verify(b->signature, truncated_block_data, BLOCK_CODE_SIZE - SIG_SIZE, state->dictator_public_key)) {
        return new_bad_signature();
    }
    free(truncated_block_data);
    free(block_data);

    // Verifying operations hash
    // ... Unnecessary as ops hash must be faulty if everything else is fine
    // We do it last as it should be the most ressce-consumming verification
    char *ops_h = ops_hash(ops);
    return new_bad_operations(ops_h);
}