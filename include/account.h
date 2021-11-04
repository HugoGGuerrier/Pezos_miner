#ifndef ACCOUNT_H
#define ACCOUNT_H

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


// ----- Memory manipulation functions -----

// Create a new account with the given data
Account_t new_account(char *user_pub_key, unsigned int lev_pez, unsigned int time_pez, unsigned int op_h_pez, unsigned int ctx_h_pez, unsigned int sig_pez);

// Delete an account
void delete_account(Account_t account);

// Create a new accout list
Accounts_t new_accounts(Account_t head, Accounts_t tail);

// Delete recursivly an account list
void delete_accounts(Accounts_t accounts);


// ----- Account encoding and decoding functions -----

// Take an account and encode it as a byte sequence
char *encode_account(Account_t account);

// Take a byte sequence and decode it as an account
Account_t decode_account(char *data);

// Take a byte sequence and decode it as an account sequence
Accounts_t decode_accounts(char *data, size_t size);


// ----- utils functions -----

// Print an account
void print_account(Account_t account);

#endif
