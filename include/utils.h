#ifndef UTILS_H
#define UTILS_H

// --- Hard define the public and private key (Not very secure...)

#define PUBLIC_KEY 0xaca76354de343ef09385e263fb59561855d3cbf167961c6955624d91aa7eecf5
#define PRIVATE_KEY 0xd9661d1a709518ef6213117508acbae0c9b04f338d017a28633bc4a1c15e1369

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
void print_hex(unsigned char *thing, size_t size);


// --- Miscallenous functions

// Compare two arbitrary data
char compare_data(unsigned char *d1, size_t s1, unsigned char *d2, size_t s2);

Bloc_t encode_to_bloc(char *in);


#endif