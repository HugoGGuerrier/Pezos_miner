#include <stdlib.h>

#include "state.h"
#include "account.h"

char *encode_state(State_t state) {
    // TODO
    return NULL;
}

State_t decode_state(char *data) {

    // Declarations and allocations

    char *dictator_public_key = malloc(32);
    unsigned long predecessor_timestamp;
    unsigned int nb_accounts;
    Accounts_t accounts = NULL;

    // Memory copy

    memcpy(dictator_public_key, data, 32);
    data = data + 32;

    memcpy(&predecessor_timestamp, data, sizeof(unsigned long));
    data = data + sizeof(unsigned long);

    memcpy(&nb_accounts, data, sizeof(unsigned int));
    data = data + sizeof(unsigned int);

    accounts = decode_accounts(data, nb_accounts);

    // Build and return the state
    return new_state(dictator_public_key, predecessor_timestamp, nb_accounts, accounts);
}