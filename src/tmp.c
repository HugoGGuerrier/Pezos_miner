#include <stdlib.h>

#include "utils.h"

unsigned char *ops_hash(Operations_t ops) {
    unsigned char *res_buf = malloc(32);
    if (ops.head == NULL) {
        for (int i=0; i < 32; i++) {
            res_buf[i] = 0;
        }
    } else {
        Operation_t op = ops.head;
        if (ops.tail == NULL) {
            blake2b((void *) res_buf, 32, (void *) encode_operation(op), (2 + op.data_size), NULL, 0);
        } else {
            unsigned char *rest_hash = ops_hash(ops.tail); 
            unsigned char *head_hash = malloc(32); 
            blake2b((void *) head_hash, 32, (void *) encode_operation(op), (2 + op.data_size), NULL, 0);
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

// TODO: refactor de into multiple verification function ?
// Can be optimized by verifying signature and not operations (making ops verif last) ?
int verify(Block_t b) {
    Block_t pred = get_predecessor(b.level); /* TODO: fonction */

    // Vérifier predecessor
    char *pred_datas = encode_block(pred);
    unsigned char *pred_hash_res = malloc(32);
    blake2b((void *) pred_hash_res, 32, (void *) pred_datas, 172, NULL, 0);
    if (compare_data(b.predecessor, pred_hash_res, 32, 32)) {
        free(pred_hash_res);

        // Vérifier timestamp
        long time = b.timestamp - pred.timestamp;
        if (time >= 600) {
            
            // Vérifier op hash
            Operations_t ops = get_operations(b.level);
            unsigned char *ops_hash = ops_hash(ops); //TODO: free

            if (compare_data(b.operations_hash, ops_hash, 32, 32)) {
                free(ops_hash);

                // Vérifier state hash
                State_t state = get_state(); /* TODO: fonction */
                char *state_datas = encode_state(state);
                unsigned char *state_hash_res = malloc(32);
                blake2b((void *) state_hash_res, 32, (void *) state_datas, (44 + state.nb_account_bytes), NULL, 0);
                if (compare_data(b.context_hash, state_hash_res, 32, 32)) {
                    free(state_hash_res);

                    // Vérifier sig
                    // ... Mais en fait si on arrive là la signature est forcément mauvaise, donc :
                    return BAD_SIGNATURE;
                } else {
                    free(state_hash_res);
                    return BAD_CONTEXT_HASH;
                }
            } else {
                free(ops_hash);
                return BAD_OPERTATIONS;
            }
        } else {
            return BAD_TIMESTAMP;
        }
    } else {
        free(pred_hash_res);
        return BAD_PREDECESSOR;
    }
}