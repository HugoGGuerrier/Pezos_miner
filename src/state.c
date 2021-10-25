#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "account.h"

char *encode_state(State_t state) {
    // Prepare the result encoded data
    char *data_res = malloc(STATE_CODE_SIZE_MIN + state->nb_account_bytes);

    // Prepare a copy of the address to iterate on
    char *data_ptr = data_res;

    // Copy memory for each block field
    memcpy(data_ptr, state->dictator_public_key, KEY_SIZE);
    data_ptr = data_ptr + KEY_SIZE;
    memcpy(data_ptr, &state->predecessor_timestamp, sizeof(long));
    data_ptr = data_ptr + 8;
    memcpy(data_ptr, &state->nb_account_bytes, sizeof(int));
    data_ptr = data_ptr + 4;
    Accounts_t accounts = state->accounts;
    while(accounts != NULL) {
        memcpy(data_ptr, encode_account(accounts->head), ACCOUNT_CODE_SIZE);
        data_ptr = data_ptr + ACCOUNT_CODE_SIZE;
        accounts = accounts->tail;
    }
    // Return the result
    return data_res;
}

State_t decode_state(char *data) {

    // Declarations and allocations

    char *dictator_public_key = malloc(KEY_SIZE);
    unsigned long predecessor_timestamp;
    unsigned int nb_account_bytes;
    Accounts_t accounts = NULL;

    // Memory copy
    
    memcpy(dictator_public_key, data, KEY_SIZE);
    data = data + KEY_SIZE;

    memcpy(&predecessor_timestamp, data, sizeof(unsigned long));
    data = data + sizeof(unsigned long);

    memcpy(&nb_account_bytes, data, sizeof(unsigned int));
    data = data + sizeof(unsigned int);

    accounts = decode_accounts(data, nb_account_bytes/ACCOUNT_CODE_SIZE);

    // Build and return the state
    return new_state(dictator_public_key, predecessor_timestamp, nb_account_bytes, accounts);
}