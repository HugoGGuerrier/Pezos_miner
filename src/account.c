#include <stdlib.h>

#include "account.h"

char *encode_account(Account_t account) {
    // Prepare the result encoded data
    char *data_res = (char *) malloc(ACCOUNT_CODE_SIZE);

    // Prepare a copy of the address to iterate on
    char *data_ptr = data_res;

    // Copy the account user public key
    memcpy(data_ptr, account->user_public_key, KEY_SIZE);
    data_ptr += KEY_SIZE;

    // Copy the account level pez
    memcpy(data_ptr, reverse_int(account->level_pez), 4);
    data_ptr += 4;

    // Copy the account timestamp pez
    memcpy(data_ptr, reverse_int(account->timestamp_pez), 4);
    data_ptr += 4;

    // Copy the account operation hash pez
    memcpy(data_ptr, reverse_int(account->operations_hash_pez), 4);
    data_ptr += 4;

    // Copy the account context hash pez
    memcpy(data_ptr, reverse_int(account->context_hash_pez), 4);
    data_ptr += 4;

    // Copy the account signature pez
    memcpy(data_ptr, reverse_int(account->signature_pez), 4);

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

    // Copy the account user public key
    memcpy(user_public_key, data, KEY_SIZE);
    data += KEY_SIZE;

    // Copy the account level pez
    memcpy(&level_pez, data, sizeof(unsigned int));
    level_pez = reverse_int(level_pez);
    data += 4;

    // Copy the account timestamp pez
    memcpy(&timestamp_pez, data, sizeof(unsigned int));
    timestamp_pez = reverse_int(timestamp_pez);
    data += 4;

    // Copy the account operation hash pez
    memcpy(&operations_hash_pez, data, sizeof(unsigned int));
    operations_hash_pez = reverse_int(operations_hash_pez);
    data += 4;

    // Copy the account context hash pez
    memcpy(&context_hash_pez, data, sizeof(unsigned int));
    context_hash_pez = reverse_int(context_hash_pez);
    data += 4;

    // Copy the account signature pez
    memcpy(&signature_pez, data, sizeof(unsigned int));
    signature_pez = reverse_int(signature_pez);

    // Build and return the account
    return new_account(user_public_key, level_pez, timestamp_pez, operations_hash_pez, context_hash_pez, signature_pez);
}

Accounts_t decode_accounts(char *data, unsigned int nb_accounts) {
    Accounts_t accounts = NULL;

    for(int i = 0 ; i < nb_accounts ; i++) {
        Account_t account = decode_account(data);

        // Add the account in the account list
        Accounts_t accounts_tmp = new_accounts(account, accounts);
        accounts = accounts_tmp;
        data += ACCOUNT_CODE_SIZE;
    }

    return accounts;
}