#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "blake2.h"
#include "client.h"
#include "utils.h"

int main()
{
    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    // Start the client
    return start_client(address, port);
}

int verify(Bloc_t b) {
    Bloc_t pred = get_predecessor(b.level);

    // Vérifier predecessor
    char *pred_datas = get_block_datas(pred);
    unsigned char *pred_hash_res = malloc(32); // TODO: free
    blake2b((void *) pred_hash_res, 32, (void *) pred_datas, 172, NULL, 0);
    if (compare_data(b.predecessor, pred_hash_res, 32, 32)) {

        free(pred_hash_res);
        // Vérifier timestamp
        long time = b.timestamp - pred.timestamp;
        if (time >= 600) {
            
            // Vérifier op hash

            if /*TODO: si op hash ok*/ {
                
                // Vérifier state hash
                State_t state = get_state();
                char *state_datas = get_state_datas(state);
                unsigned char *state_hash_res = malloc(32);
                blake2b((void *) state_hash_res, 32, (void *) state_datas, /* 44 + 52 * account nb */, NULL, 0);
                if (compare_data(b.context_hash, state_hash_res, 32, 32)) {
                    
                    free(state_hash_res);
                    // Vérifier sig
                    // ... Mais en fait si on arrive là la signature est forcément mauvaise, donc :
                    return BAD_SIGNATURE
                } else {
                    free(state_hash_res);
                    return BAD_CONTEXT_HASH
                }
            } else {
                return BAD_OPERTATIONS
            }
        } else {
            return BAD_TIMESTAMP;
        }
    } else {
        free(pred_hash_res);
        return BAD_PREDECESSOR;
    }
}