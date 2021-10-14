#ifndef UTILS_H
#define UTILS_H

// --- Hard define the public and private key (Not very secure...)

#define KEY_SIZE 32
#define PUBLIC_KEY {0xac,0xa7,0x63,0x54,0xde,0x34,0x3e,0xf0,0x93,0x85,0xe2,0x63,0xfb,0x59,0x56,0x18,0x55,0xd3,0xcb,0xf1,0x67,0x96,0x1c,0x69,0x55,0x62,0x4d,0x91,0xaa,0x7e,0xec,0xf5}
#define PRIVATE_KEY {0xd9,0x66,0x1d,0x1a,0x70,0x95,0x18,0xef,0x62,0x13,0x11,0x75,0x08,0xac,0xba,0xe0,0xc9,0xb0,0x4f,0x33,0x8d,0x01,0x7a,0x28,0x63,0x3b,0xc4,0xa1,0xc1,0x5e,0x13,0x69}

// ----- Types and enum defining -----


// --- Enumeration for all message tag
typedef enum {
    GET_CURRENT_HEAD,
    CURRENT_HEAD,
    GET_BLOCK,
    BLOCK,
    GET_BLOCK_OPERATIONS,
    BLOCK_OPERATIONS,
    GET_BLOCK_STATE,
    BLOCK_STATE,
    INJECT_OPERATION,
} Message_Type_t;


// --- Enumeration for all server operation type
typedef enum {
    BAD_PREDECESSOR,
    BAD_TIMESTAMP,
    BAD_OPERATIONS_HASH,
    BAD_CONTEXT_HASH,
    BAD_SIGNATURE,
} Operation_Type_t;


// --- Type that represents a message
typedef struct message *Message_t;

struct message {
    Message_Type_t tag;
    unsigned short data_size;
    char *data;
};


// --- Type that represent an operation
typedef struct operation *Operation_t;

struct operation {
    Operation_Type_t op_type;
    unsigned short data_size;
    char *data;
};


// --- Type thar represent an operations list
typedef struct operations *Operations_t;

struct operations {
    Operation_t head;
    Operations_t tail;
};


// --- Type that represents a block
typedef struct block *Block_t;

struct block {
    int level;
    char *predecessor;
    long timestamp; 
    char *operations_hash;
    char *context_hash;
    char *signature;
};


// --- Type that represents a state
typedef struct state *State_t;

struct state {
    char *dictator_public_key;
    unsigned long predecessor_timestamp;
    Accounts_t accounts;
};


// --- Type that represents an account
typedef struct account *Account_t;

struct account {
    char *user_public_key;
    unsigned int level_pez;
    unsigned int timestamp_pez;
    unsigned int operations_hash_pez;
    unsigned int context_hash_pez;
    unsigned int signature_pez;
};


// --- Type that represent an accout list
typedef struct accounts *Accounts_t;

struct accounts {
    Account_t head;
    Accounts_t tail;
};


// ----- Utils function definitions -----


// --- Data creation and deletion functions

// Create a new message
Message_t new_message(Message_Type_t tag, char *data);

// Delete a message
void delete_message(Message_t message);

// Create a new operation
Operation_t new_operation(Operation_Type_t op_type, char *data);

// Delete an operation
void delete_operation(Operation_t operation);

// Create a new operaiton list
Operations_t new_operations(Operation_t head, Operations_t tail);

// Delete recursively an operation list
void delete_operations(Operations_t operations);

// Create a new block from the given data
Block_t new_block(int level, char *pred, long time, char *ope_h, char *ctx_h, char* sig);

// Delete a block
void delete_block(Block_t block);

// Create a new state from the given data
State_t new_state(char *dict_pub_key, unsigned long pred_time, Accounts_t accounts);

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

// Print a message
void print_message(Message_t message);

// Print a data in hexadecimal form
void print_hex(char *thing, size_t size);


// --- Miscallenous functions

// Compare two arbitrary data
char compare_data(char *d1, size_t s1, char *d2, size_t s2);

#endif