#ifndef OPERATION_H
#define OPERATION_H

#define OP_TAG_SIZE sizeof(short)
#define OP_CODE_SIZE_MIN (OP_TAG_SIZE + SIG_SIZE + KEY_SIZE)


// ----- Types and enum defining -----

// --- Enumeration for all server operation type
typedef enum
{
    BAD_PREDECESSOR,
    BAD_TIMESTAMP,
    BAD_OPERATIONS_HASH,
    BAD_CONTEXT_HASH,
    BAD_SIGNATURE,
} Operation_Type_t;

// --- Type that represent an operation
typedef struct operation *Operation_t;

struct operation
{
    Operation_Type_t op_type;
    unsigned short data_size;
    char *data;
    char *user_key;
    char *signature;
};

// --- Type thar represent an operations list
typedef struct operations *Operations_t;

struct operations
{
    Operation_t head;
    Operations_t tail;
};


// ----- Memory manipulation functions -----

// Create a new operation
Operation_t new_operation(Operation_Type_t op_type, unsigned short data_size, char *data, char *user_key, char *sig);

// Delete an operation
void delete_operation(Operation_t operation);

// Create a new operation list
Operations_t new_operations(Operation_t head, Operations_t tail);

// Delete recursively an operation list
void delete_operations(Operations_t operations);

// Create a new bad predecessor operation with the correct hash
Operation_t new_bad_predecessor(char *pred_hash);


// ----- Operation specific creation functions ------

// Create a new bad timestamp operation with the correct timestamp
Operation_t new_bad_timestamp(unsigned long timestamp);

// Create a new bad operations hash operation with the correct op hash
Operation_t new_bad_operations(char *op_hash);

// Create a new bad context hash operation with the correct context hash
Operation_t new_bad_context(char *ctx_hash);

// Create a new bad signature
Operation_t new_bad_signature();


// ----- Operations encoding and decoding functions -----

// Encode an operation to binary
char *encode_operation(Operation_t operation);

// Decode binary to operation
Operation_t decode_operation(char *data);

// Decode the operations and return a reversed list of them
Operations_t decode_operations(char *data, size_t data_size);


// ----- Utils functions -----

// Get the string for an operation type
char *op_type_str(const Operation_Type_t type);

// Hash an operation list
char *ops_hash(Operations_t ops);

// Print an operation
void print_op(Operation_t op);

#endif