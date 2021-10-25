#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <byteswap.h>

#include "utils.h"

// --- Message functions

Message_t new_message(Message_Type_t tag, unsigned short data_size, char *data)
{
    Message_t res = (Message_t)malloc(sizeof(struct message));
    res->tag = tag;
    res->data_size = data_size;
    res->data = data;
    return res;
}

void delete_message(Message_t message)
{
    if (message->data != NULL)
    {
        free(message->data);
    }
    free(message);
}

// --- Operation functions

Operation_t new_operation(Operation_Type_t op_type, unsigned short data_size, char *data, char *user_key, char *sig)
{
    Operation_t res = (Operation_t)malloc(sizeof(struct operation));
    res->op_type = op_type;
    res->data_size = data_size;
    res->data = data;
    res->user_key = user_key;
    res->signature = sig;
    return res;
}

void delete_operation(Operation_t operation)
{
    if (operation->data != NULL)
    {
        free(operation->data);
    }
    free(operation->user_key);
    free(operation->signature);
    free(operation);
}

// --- Operation list functions

Operations_t new_operations(Operation_t head, Operations_t tail)
{
    Operations_t res = (Operations_t)malloc(sizeof(struct operations));
    res->head = head;
    res->tail = tail;
    return res;
}

void delete_operations(Operations_t operations)
{
    if (operations != NULL)
    {
        delete_operations(operations->tail);
        delete_operation(operations->head);
    }
}

// --- Block functions

Block_t new_block(int level, char *pred, long time, char *ope_h, char *ctx_h, char *sig)
{
    Block_t res = (Block_t)malloc(sizeof(struct block));
    res->level = level;
    res->predecessor = pred;
    res->timestamp = time;
    res->operations_hash = ope_h;
    res->context_hash = ctx_h;
    res->signature = sig;
    return res;
}

void delete_block(Block_t block)
{
    free(block->predecessor);
    free(block->operations_hash);
    free(block->context_hash);
    free(block->signature);
    free(block);
}

// --- State functions

State_t new_state(char *dict_pub_key, unsigned long pred_time, unsigned long nb_bytes, Accounts_t accounts)
{
    State_t res = (State_t)malloc(sizeof(struct state));
    res->dictator_public_key = dict_pub_key;
    res->predecessor_timestamp = pred_time;
    res->nb_account_bytes = nb_bytes;
    res->accounts = accounts;
    return res;
}

void delete_state(State_t state)
{
    free(state->dictator_public_key);
    delete_accounts(state->accounts);
    free(state);
}

// --- Account functions

Account_t new_account(char *user_pub_key, unsigned int lev_pez, unsigned int time_pez, unsigned int op_h_pez, unsigned int ctx_h_pez, unsigned int sig_pez)
{
    Account_t res = (Account_t)malloc(sizeof(struct account));
    res->user_public_key = user_pub_key;
    res->level_pez = lev_pez;
    res->timestamp_pez = time_pez;
    res->operations_hash_pez = op_h_pez;
    res->context_hash_pez = ctx_h_pez;
    res->signature_pez = sig_pez;
    return res;
}

void delete_account(Account_t account)
{
    free(account->user_public_key);
    free(account);
}

// --- Account list functions

Accounts_t new_accounts(Account_t head, Accounts_t tail)
{
    Accounts_t res = (Accounts_t)malloc(sizeof(struct accounts));
    res->head = head;
    res->tail = tail;
    return res;
}

void delete_accounts(Accounts_t accounts)
{
    if (accounts != NULL)
    {
        delete_accounts(accounts->tail);
        delete_account(accounts->head);
    }
}

// --- Printing functions

void print_block(Block_t block)
{
    printf("--- BLOCK ---\n");
    printf("level : %d\n", block->level);
    printf("predecessor : ");
    print_hex(block->predecessor, HASH_SIZE, "\n");
    printf("timestamp : %lu\n", block->timestamp);
    printf("operations hash : ");
    print_hex(block->operations_hash, HASH_SIZE, "\n");
    printf("context hash : ");
    print_hex(block->context_hash, HASH_SIZE, "\n");
    printf("signature : ");
    print_hex(block->signature, SIG_SIZE, "\n");
    printf("-------------\n");
}

void print_account(Account_t account)
{
    printf("--- ACCOUNT ---\n");

    printf("user public key : ");
    print_hex(account->user_public_key, HASH_SIZE, "\n");
    printf("level_pez : %d\n", account->level_pez);
    printf("operations hash_pez : %d\n", account->operations_hash_pez);
    printf("context_hash_pez : %d\n", account->context_hash_pez);
    printf("signature_pez : %d\n", account->signature_pez);
    printf("-------------\n");
}

void print_hex(char *thing, size_t size, const char *end)
{
    for (size_t i = 0; i < size; i++)
    {
        printf("%02x", thing[i] & 0xff);
    }
    printf(end);
}

// --- Miscallenous functions

char compare_data(char *d1, size_t s1, char *d2, size_t s2)
{
    if (s1 != s2)
        return 0;
    for (size_t i = 0; i < s1; i++)
    {
        if (d1[i] != d2[i])
            return 0;
    }
    return 1;
}

long reverse_long(long src)
{
    return bswap_64(src);
}

int reverse_int(int src)
{
    return bswap_32(src);
}

short reverse_short(short src)
{
    return bswap_16(src);
}

char *read_hex_string(char *hex_str)
{
    int hex_str_len = strlen(hex_str);
    char *pos = hex_str;

    // One byte is represented by two hexadecimal chars
    char *res = (char *)malloc(hex_str_len / 2);

    // Iterate pairs on the hexa string
    for (int i = 0; i < hex_str_len / 2; i++)
    {
        sscanf(pos, "%2hhx", &res[i]);
        pos += 2;
    }

    // Return the result
    return res;
}

char *get_public_key_copy()
{
    char *res = (char *)malloc(KEY_SIZE);
    char pk[KEY_SIZE] = PUBLIC_KEY;
    memcpy(res, pk, KEY_SIZE);

    return res;
}