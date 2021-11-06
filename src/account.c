#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "account.h"
#include "utils.h"


// ----- Memory manipulation functions -----

Account_t new_account(char *user_pub_key, unsigned int lev_pez, unsigned int time_pez, unsigned int op_h_pez, unsigned int ctx_h_pez, unsigned int sig_pez) {
    Account_t res = (Account_t) malloc(sizeof(struct account));
    res->user_public_key = user_pub_key;
    res->predecessor_pez = lev_pez;
    res->timestamp_pez = time_pez;
    res->operations_hash_pez = op_h_pez;
    res->context_hash_pez = ctx_h_pez;
    res->signature_pez = sig_pez;
    return res;
}

void delete_account(Account_t account) {
    free(account->user_public_key);
    free(account);
}

Accounts_t new_accounts(Account_t head, Accounts_t tail) {
    Accounts_t res = (Accounts_t) malloc(sizeof(struct accounts));
    res->head = head;
    res->tail = tail;
    return res;
}

void delete_accounts(Accounts_t accounts) {
    if (accounts != NULL) {
        delete_accounts(accounts->tail);
        delete_account(accounts->head);
    }
}


// ----- Accounts encoding and decoding functions -----

char *encode_account(Account_t account) {
    // Prepare the result encoded data
    char *res = (char *) malloc(ACCOUNT_CODE_SIZE);

    // Prepare a copy of the address to iterate on
    char *ptr = res;

    // Copy the account user public key
    memcpy(ptr, account->user_public_key, KEY_SIZE);
    ptr += KEY_SIZE;

    // Copy the account level pez
    unsigned int real_pez_level = reverse_int(account->predecessor_pez);
    memcpy(ptr, &real_pez_level, sizeof(int));
    ptr += sizeof(int);

    // Copy the account timestamp pez
    unsigned int real_timstamp_pez = reverse_int(account->timestamp_pez);
    memcpy(ptr, &real_timstamp_pez, sizeof(int));
    ptr += sizeof(int);

    // Copy the account operation hash pez
    unsigned int real_operation_h_pez = reverse_int(account->operations_hash_pez);
    memcpy(ptr, &real_operation_h_pez, sizeof(int));
    ptr += sizeof(int);

    // Copy the account context hash pez
    unsigned int real_context_h_pez = reverse_int(account->context_hash_pez);
    memcpy(ptr, &real_context_h_pez, sizeof(int));
    ptr += sizeof(int);

    // Copy the account signature pez
    unsigned int real_sig_pez = reverse_int(account->signature_pez);
    memcpy(ptr, &real_sig_pez, sizeof(int));

    return res;
}

Account_t decode_account(char *data) {
    // Declarations and allocations
    char *user_public_key = (char *) malloc(KEY_SIZE);
    unsigned int level_pez;
    unsigned int timestamp_pez;
    unsigned int operations_hash_pez;
    unsigned int context_hash_pez;
    unsigned int signature_pez;

    // Copy the account user public key
    memcpy(user_public_key, data, KEY_SIZE);
    data += KEY_SIZE;

    // Copy the account level pez
    memcpy(&level_pez, data, sizeof(int));
    level_pez = reverse_int(level_pez);
    data += sizeof(int);

    // Copy the account timestamp pez
    memcpy(&timestamp_pez, data, sizeof(int));
    timestamp_pez = reverse_int(timestamp_pez);
    data += sizeof(int);

    // Copy the account operation hash pez
    memcpy(&operations_hash_pez, data, sizeof(int));
    operations_hash_pez = reverse_int(operations_hash_pez);
    data += sizeof(int);

    // Copy the account context hash pez
    memcpy(&context_hash_pez, data, sizeof(int));
    context_hash_pez = reverse_int(context_hash_pez);
    data += sizeof(int);

    // Copy the account signature pez
    memcpy(&signature_pez, data, sizeof(int));
    signature_pez = reverse_int(signature_pez);

    // Build and return the account
    return new_account(user_public_key, level_pez, timestamp_pez, operations_hash_pez, context_hash_pez, signature_pez);
}

Accounts_t decode_accounts(char *data, size_t size) {
    // Prepare the result and the cursor
    size_t cursor = 0;
    Accounts_t res = NULL;

    while (cursor < size){
        // Decode the first account
        Account_t account = decode_account(data);

        // Add it to the list
        Accounts_t tmp = new_accounts(account, res);
        res = tmp;

        // Increment the data and the cursor
        cursor += ACCOUNT_CODE_SIZE;
        data += ACCOUNT_CODE_SIZE;
    }
    
    // Return the result
    return res;
}


// ----- Utils functions -----

void print_account(Account_t account) {
    printf("--- ACCOUNT ---\n");
    printf("user public key : ");
    print_hex(account->user_public_key, HASH_SIZE, "\n");
    printf("predecessor_pez : %u\n", account->predecessor_pez);
    printf("timestamp_pez : %u\n", account->timestamp_pez);
    printf("operations_hash_pez : %u\n", account->operations_hash_pez);
    printf("context_hash_pez : %u\n", account->context_hash_pez);
    printf("signature_pez : %u\n", account->signature_pez);
    printf("---------------\n");
}
