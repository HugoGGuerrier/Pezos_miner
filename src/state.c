#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "state.h"
#include "account.h"
#include "utils.h"


char *encode_state(State_t state) {
    // Prepare the result encoded data
    char *data_res = (char *) malloc(STATE_CODE_SIZE_MIN + state->nb_account_bytes);

    // Prepare a copy of the address to iterate on
    char *data_ptr = data_res;

    // Copy the dictator public key
    memcpy(data_ptr, state->dictator_public_key, KEY_SIZE);
    data_ptr += KEY_SIZE;

    // Copy the predecessor timestamp
    unsigned long real_timestamp = reverse_long(state->predecessor_timestamp);
    memcpy(data_ptr, &real_timestamp, sizeof(long));
    data_ptr += sizeof(long);

    // Copy the number of account bytes
    unsigned int real_nb_bytes = reverse_int(state->nb_account_bytes);
    memcpy(data_ptr, &real_nb_bytes, sizeof(int));

    // Copy the accounts
    data_ptr = data_res + (STATE_CODE_SIZE_MIN + state->nb_account_bytes) - ACCOUNT_CODE_SIZE;
    Accounts_t accounts = state->accounts;
    while(accounts != NULL) {
        char *encoded = encode_account(accounts->head);
        memcpy(data_ptr, encoded, ACCOUNT_CODE_SIZE);
        free(encoded);
        data_ptr -= ACCOUNT_CODE_SIZE;
        accounts = accounts->tail;
    }

    // Return the result
    return data_res;
}

State_t decode_state(char *data) {
    // Declarations and allocations
    char *dictator_public_key = (char *) malloc(KEY_SIZE);
    unsigned long predecessor_timestamp;
    unsigned int nb_account_bytes;
    Accounts_t accounts = NULL;

    // Get the dictator public key
    memcpy(dictator_public_key, data, KEY_SIZE);
    data += KEY_SIZE;

    // Get the predecessor timestamps
    predecessor_timestamp = reverse_long(*((unsigned long *) data));
    data += sizeof(long);

    // Get the number of account bytes
    nb_account_bytes = reverse_int(*((unsigned int *) data));
    data += sizeof(int);

    accounts = decode_accounts(data, (size_t) nb_account_bytes);

    // Build and return the state
    return new_state(dictator_public_key, predecessor_timestamp, nb_account_bytes, accounts);
}