#ifndef ACCOUNT_H
#define ACCOUNT_H

#include "utils.h"

#define ACCOUNT_CODE_SIZE (KEY_SIZE + 5*4) 


// ----- Account encoding and decoding functions -----

// Take an account and encode it as a byte sequence
char *encode_account(Account_t account);

// Take a byte sequence and decode it as an account
Account_t decode_account(char *data);

// Take a byte sequence and decode it as an account sequence
Accounts_t decode_accounts(char *data, size_t size);

#endif
