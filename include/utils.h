#ifndef UTILS_H
#define UTILS_H

#include "message.h"
#include "operation.h"
#include "account.h"
#include "block.h"
#include "state.h"



// ----- Hard define the public and private key (Not very secure...) -----

#define HASH_SIZE 32
#define KEY_SIZE 32
#define SIG_SIZE 64
#define PUBLIC_KEY {0xac, 0xa7, 0x63, 0x54, 0xde, 0x34, 0x3e, 0xf0, 0x93, 0x85, 0xe2, 0x63, 0xfb, 0x59, 0x56, 0x18, 0x55, 0xd3, 0xcb, 0xf1, 0x67, 0x96, 0x1c, 0x69, 0x55, 0x62, 0x4d, 0x91, 0xaa, 0x7e, 0xec, 0xf5}
#define PRIVATE_KEY {0xd9, 0x66, 0x1d, 0x1a, 0x70, 0x95, 0x18, 0xef, 0x62, 0x13, 0x11, 0x75, 0x08, 0xac, 0xba, 0xe0, 0xc9, 0xb0, 0x4f, 0x33, 0x8d, 0x01, 0x7a, 0x28, 0x63, 0x3b, 0xc4, 0xa1, 0xc1, 0x5e, 0x13, 0x69}


// ----- Utils function definitions -----


// --- Data creation and deletion functions

// Create a new message
Message_t new_message(Message_Type_t tag, unsigned short data_size, char *data);

// Delete a message
void delete_message(Message_t message);

// Create a new operation
Operation_t new_operation(Operation_Type_t op_type, unsigned short data_size, char *data, char *user_key, char *sig);

// Delete an operation
void delete_operation(Operation_t operation);

// Create a new operaiton list
Operations_t new_operations(Operation_t head, Operations_t tail);

// Delete recursively an operation list
void delete_operations(Operations_t operations);

// Create a new block from the given data
Block_t new_block(unsigned int level, char *pred, unsigned long time, char *ope_h, char *ctx_h, char *sig);

// Delete a block
void delete_block(Block_t block);

// Create a new state from the given data
State_t new_state(char *dict_pub_key, unsigned long pred_time, unsigned long nb_bytes, Accounts_t accounts);

// Delete a state
void delete_state(State_t state);

// Create a new account with the given data
Account_t new_account(char *user_pub_key, unsigned int lev_pez, unsigned int time_pez, unsigned int op_h_pez, unsigned int ctx_h_pez, unsigned int sig_pez);

// Delete an account
void delete_account(Account_t account);

// Create a new accout list
Accounts_t new_accounts(Account_t head, Accounts_t tail);

// Delete recursivly an account list
void delete_accounts(Accounts_t accounts);


// --- Printing functions

// Print an operation
void print_op(Operation_t op);

// Print a block
void print_block(Block_t block);

// Print an account
void print_account(Account_t account);

// Print a message
void print_message(Message_t message);

// Print a state
void print_state(State_t state);

// Print a data in hexadecimal form
void print_hex(char *thing, size_t size, const char *end);


// --- Miscallenous functions

// Compare two arbitrary data
char compare_data(char *d1, size_t s1, char *d2, size_t s2);

// Reverse the short endianness
long reverse_long(long src);

// Reverse the integer endianness
int reverse_int(int src);

// Reverse the short endianness
short reverse_short(short src);

// Read bytes from an hexadecimal string
char *read_hex_string(char *hex_str);

// Get a copy of the public key
char *get_public_key_copy();

// Get a copy of the private key
char *get_private_key_copy();

#endif