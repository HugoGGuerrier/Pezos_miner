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


// --- Type that represents a message
typedef struct message Message_t;

struct message {
    Message_Type_t tag;
    char *data;
};


// --- Enumeration for all server operation type
typedef enum {
    BAD_PREDECESSOR,
    BAD_TIMESTAMP,
    BAD_OPERATIONS_HASH,
    BAD_CONTEXT_HASH,
    BAD_SIGNATURE,
} Operation_type_t;


// --- Type that represent an operation
typedef struct operation *Operation_t;

struct operation {
    Operation_type_t op_type;
    char *data;
};


// --- Type that represents a bloc
typedef struct bloc *Bloc_t;

struct bloc {
    int level;
    char *predecessor;
    long timestamp; 
    char *operations_hash;
    char *context_hash;
    char *signature;
};

// ----- Utils function definitions -----


// --- Data creation and deletion functions

// Create a new message
Message_t new_message(Message_Type_t tag, char *data);

// Delete a message
void delete_message(Message_t message);

// Create a new operation
Operation_t new_operation(Operation_t op_type, char *data);

// Delete an operation
void delete_operation(Operation_t operation);

// Create a new bloc from the given data
Bloc_t new_bloc(int l, char * pred, long t, char* ope, char* c, char*s);

// Delete a block
void delete_block(Bloc_t bloc);


// --- Printing functions

// Print a bloc
void print_bloc(Bloc_t bloc);

void print_op(Operation_t op);

// Print a data in hexadecimal form
void print_hex(char *thing, size_t size);


// --- Miscallenous functions

// Compare two arbitrary data
char compare_data(char *d1, size_t s1, char *d2, size_t s2);

Bloc_t encode_to_bloc(char *in);


#endif