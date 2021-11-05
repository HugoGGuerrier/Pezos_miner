#ifndef STATE_H
#define STATE_H

#include "account.h"

#define STATE_CODE_SIZE_MIN (KEY_SIZE + sizeof(long) + sizeof(int))


// ----- Types and enum defining -----

// --- Type that represents a state
typedef struct state *State_t;

struct state
{
    char *dictator_public_key;
    unsigned long predecessor_timestamp;
    unsigned int nb_account_bytes;
    Accounts_t accounts;
};


// ----- Memory manipulation functions -----

// Create a new state from the given data
State_t new_state(char *dict_pub_key, unsigned long pred_time, unsigned long nb_bytes, Accounts_t accounts);

// Delete a state
void delete_state(State_t state);


// ----- State encoding and decoding functions -----

// Take a state and encode it as a byte sequence (could reverse order of the accounts in the result)
char *encode_state(State_t state);

// Take a byte sequence and decode it as a state
State_t decode_state(char *data);


// ----- Utils functions -----

// Print a state
void print_state(State_t state);

// Print a state with only account number
void print_state_l(State_t state);

#endif