#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "utils.h"

#define ACCOUNT_CODE_SIZE (KEY_SIZE + 5*4) 


// ----- Types and enum defining -----

// --- Type that represents an account
typedef struct account *Account_t;

struct account
{
    char *user_public_key;
    unsigned int level_pez;
    unsigned int timestamp_pez;
    unsigned int operations_hash_pez;
    unsigned int context_hash_pez;
    unsigned int signature_pez;
};

// --- Type that represent an accout list
typedef struct accounts *Accounts_t;

struct accounts
{
    Account_t head;
    Accounts_t tail;
};


// ----- Account encoding and decoding functions -----

// Take an account and encode it as a byte sequence
char *encode_account(Account_t account);

// Take a byte sequence and decode it as an account
Account_t decode_account(char *data);

// Take a byte sequence and decode it as an account sequence
Accounts_t decode_accounts(char *data, size_t size);

#endif
