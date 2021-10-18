#include <stdlib.h>

#include "account.h"

char * encode_account(Account_t account) {
    // Prepare the result encoded data
    char *data_res = malloc(ACCOUNT_CODE_SIZE);

    // Prepare a copy of the address to iterate on
    char *data_ptr = data_res;

    // Copy memory for each block field

    memcpy(data_ptr, account->user_public_key, KEY_SIZE);
    data_ptr = data_ptr + KEY_SIZE;

    memcpy(data_ptr, account->level_pez, 4);
    data_ptr = data_ptr + 4;

    memcpy(data_ptr, account->timestamp_pez, 4);
    data_ptr = data_ptr + 4;

    memcpy(data_ptr, account->operations_hash_pez, 4);
    data_ptr = data_ptr + 4;

    memcpy(data_ptr, account->context_hash_pez, 4);
    data_ptr = data_ptr + 4;

    memcpy(data_ptr, account->signature_pez, 4);

    return data_res;
}

Account_t decode_account(char *data) {

    // Declarations and allocations

    char *user_public_key = malloc(KEY_SIZE);
    unsigned int level_pez;
    unsigned int timestamp_pez;
    unsigned int operations_hash_pez;
    unsigned int context_hash_pez;
    unsigned int signature_pez;

    // Memory copy

    memcpy(user_public_key, data, KEY_SIZE);
    data = data + KEY_SIZE;

    memcpy(&level_pez, data, sizeof(unsigned int));
    data = data + 4;

    memcpy(&timestamp_pez, data, sizeof(unsigned int));
    data = data + 4;

    memcpy(&operations_hash_pez, data, sizeof(unsigned int));
    data = data + 4;

    memcpy(&context_hash_pez, data, sizeof(unsigned int));
    data = data + 4;

    memcpy(&signature_pez, data, sizeof(unsigned int));

    // Build and return the account
    return new_account(user_public_key, level_pez, timestamp_pez, operations_hash_pez, context_hash_pez, signature_pez);
}

Accounts_t decode_accounts(char *data, unsigned int nb_accounts) {
    Accounts_t accounts = NULL;

    for(int i = 0 ; i < nb_accounts ; i++) {
        Account_t account = decode_account(data);
        // Rajout de l'account courant en tête des accounts
        Accounts_t accounts_tmp = new_accounts(account, accounts);
        accounts = accounts_tmp;
        data = data + ACCOUNT_CODE_SIZE;
    }

    return accounts;
}