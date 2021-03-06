#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "operation.h"
#include "hash.h"
#include "signature.h"
#include "utils.h"


// ----- Internal functions -----

unsigned short op_type_to_tag(Operation_Type_t op_type) {
    switch (op_type) {
    case BAD_PREDECESSOR:
        return reverse_short(1);

    case BAD_TIMESTAMP:
        return reverse_short(2);

    case BAD_OPERATIONS_HASH:
        return reverse_short(3);

    case BAD_CONTEXT_HASH:
        return reverse_short(4);

    case BAD_SIGNATURE:
        return reverse_short(5);
    
    default:
        printf("Error in operation encoding : Unknown operation tag\n");
        exit(1);
    }
}

Operation_Type_t tag_to_op_type(unsigned short tag) {
    switch (tag) {
    case 1:
        return BAD_PREDECESSOR;

    case 2:
        return BAD_TIMESTAMP;

    case 3:
        return BAD_OPERATIONS_HASH;

    case 4:
        return BAD_CONTEXT_HASH;

    case 5:
        return BAD_SIGNATURE;
    
    default:
        printf("Error in operation decoding : Unknown operation tag\n");
        exit(1);
    }
}

void sign_operation(Operation_t op) {
    // Prepare the byte array to hash
    size_t to_hash_size = OP_TAG_SIZE + op->data_size + KEY_SIZE;
    char *to_hash = (char *) malloc(to_hash_size);
    char *ptr = to_hash;

    // Add the tag
    unsigned short tag = op_type_to_tag(op->op_type);
    memcpy(ptr, &tag, OP_TAG_SIZE);
    ptr += OP_TAG_SIZE;

    // Add the operation data
    memcpy(ptr, op->data, op->data_size);
    ptr += op->data_size;

    // Add our public key
    memcpy(ptr, op->user_key, KEY_SIZE);

    // Hash the result
    char to_sign[HASH_SIZE];
    hash(to_hash, to_hash_size, to_sign);

    // Free the to hash array
    free(to_hash);

    // Get the signature
    char *signature = (char *) malloc(SIG_SIZE);
    sign(signature, to_sign, HASH_SIZE);

    // Put it in the operation
    op->signature = signature;
}


// ----- Memory manipulation functions -----

Operation_t new_operation(Operation_Type_t op_type, unsigned short data_size, char *data, char *user_key, char *sig) {
    Operation_t res = (Operation_t)malloc(sizeof(struct operation));
    res->op_type = op_type;
    res->data_size = data_size;
    res->data = data;
    res->user_key = user_key;
    res->signature = sig;
    return res;
}

void delete_operation(Operation_t operation) {
    if (operation->data != NULL) {
        free(operation->data);
    }
    free(operation->user_key);
    free(operation->signature);
    free(operation);
}

Operations_t new_operations(Operation_t head, Operations_t tail) {
    Operations_t res = (Operations_t)malloc(sizeof(struct operations));
    res->head = head;
    res->tail = tail;
    return res;
}

void delete_operations(Operations_t operations) {
    if (operations != NULL) {
        delete_operations(operations->tail);
        delete_operation(operations->head);
    }
}


// ----- Operation specific creation functions ------

Operation_t new_bad_predecessor(char *pred_hash) {
    // Create a public key copy
    char *public_key = get_public_key_copy();

    // Create the operation, sign it and send it
    Operation_t res = new_operation(BAD_PREDECESSOR, HASH_SIZE, pred_hash, public_key, NULL);
    sign_operation(res);
    return res;
}

Operation_t new_bad_timestamp(unsigned long timestamp) {
    // Create a public key copy
    char *public_key = get_public_key_copy();

    // Create the timestamp
    timestamp = reverse_long(timestamp);
    char *data = (char *) malloc(sizeof(unsigned long));
    memcpy(data, &timestamp, sizeof(unsigned long));

    // Create the operation, sign it and send it
    Operation_t res = new_operation(BAD_TIMESTAMP, sizeof(unsigned long), data, public_key, NULL);
    sign_operation(res);
    return res;
}

Operation_t new_bad_operations(char *op_hash) {
    // Create a public key copy
    char *public_key = get_public_key_copy();

    // Create the operation, sign it and send it
    Operation_t res = new_operation(BAD_OPERATIONS_HASH, HASH_SIZE, op_hash, public_key, NULL);
    sign_operation(res);
    return res;
}

Operation_t new_bad_context(char *ctx_hash) {
    // Create a public key copy
    char *public_key = get_public_key_copy();

    // Create the operation, sign it and send it
    Operation_t res = new_operation(BAD_CONTEXT_HASH, HASH_SIZE, ctx_hash, public_key, NULL);
    sign_operation(res);
    return res;
}

Operation_t new_bad_signature() {
    // Create a public key copy
    char *public_key = get_public_key_copy();

    // Create the operation, sign it and send it
    Operation_t res = new_operation(BAD_SIGNATURE, 0, NULL, public_key, NULL);
    sign_operation(res);
    return res;
}


// ----- Operations encoding and decoding functions -----

char *encode_operation(Operation_t operation) {
    // Prepare the result
    char *res = (char *) malloc(OP_CODE_SIZE_MIN + operation->data_size);
    char *ptr = res;

    // Prepare the operation type
    unsigned short tag = op_type_to_tag(operation->op_type);

    // Copy the tag in the operation
    memcpy(ptr, &tag, OP_TAG_SIZE);
    ptr += OP_TAG_SIZE;

    // Copy the operation data
    memcpy(ptr, operation->data, operation->data_size);
    ptr += operation->data_size;

    // Copy the user key
    memcpy(ptr, operation->user_key, KEY_SIZE);
    ptr += KEY_SIZE;

    // Copy the signature
    memcpy(ptr, operation->signature, SIG_SIZE);

    // Return the result
    return res;
}

Operation_t decode_operation(char *data) {
    // Get the tag from the operation
    unsigned short tag = reverse_short(*((unsigned short *)data));
    data += OP_TAG_SIZE;

    // Prepare the data size
    unsigned short data_size;
    Operation_Type_t op_type;

    // Switch on the tag
    switch (tag) {
    case 1:
        op_type = BAD_PREDECESSOR;
        data_size = 32;
        break;

    case 2:
        op_type = BAD_TIMESTAMP;
        data_size = 8;
        break;

    case 3:
        op_type = BAD_OPERATIONS_HASH;
        data_size = 32;
        break;

    case 4:
        op_type = BAD_CONTEXT_HASH;
        data_size = 32;
        break;

    case 5:
        op_type = BAD_SIGNATURE;
        data_size = 0;
        break;
    
    default:
        printf("Error in operation decoding : Unknown operation tag\n");
        exit(1);
    }

    // Create the operation data
    char *op_data = (char *) malloc(data_size);
    memcpy(op_data, data, data_size);
    data += data_size;

    // Create the operation user key
    char *op_key = (char *) malloc(KEY_SIZE);
    memcpy(op_key, data, KEY_SIZE);
    data += KEY_SIZE;

    // Create the operation signature
    char *op_sig = (char *) malloc(SIG_SIZE);
    memcpy(op_sig, data, SIG_SIZE);
    data += SIG_SIZE;

    // Return the new operation
    return new_operation(op_type, data_size, op_data, op_key, op_sig);
}

Operations_t decode_operations(char *data, size_t size) {
    // Prepare the result and the cursor
    size_t cursor = 0;
    Operations_t res = NULL;

    while(cursor < size) {
        // Decode the first operation
        Operation_t op = decode_operation(data);

        // Add it to the list
        Operations_t tmp = new_operations(op, res);
        res = tmp;

        // Increment data and cursor
        cursor += OP_CODE_SIZE_MIN + op->data_size;
        data += OP_CODE_SIZE_MIN + op->data_size;
    }

    // Return the result
    return res;
}


// ----- Utils functions -----

char *op_type_str(const Operation_Type_t type) {
    switch (type) {
    case BAD_PREDECESSOR:
        return "BAD_PREDECESSOR";

    case BAD_TIMESTAMP:
        return "BAD_TIMESTAMP";

    case BAD_OPERATIONS_HASH:
        return "BAD_OPERATIONS_HASH";

    case BAD_CONTEXT_HASH:
        return "BAD_CONTEXT_HASH";

    case BAD_SIGNATURE:
        return "BAD_SIGNATURE";
    
    default:
        printf("Error in operation encoding : Unknown operation tag\n");
        exit(1);
    }
}

char *ops_hash(Operations_t ops) {
    // Prepare the result
    char *res_buf = (char *) malloc(HASH_SIZE);

    // If the head is NULL, just set the buffer to 0
    if (ops->head == NULL) {
        memset(res_buf, 0, HASH_SIZE);
    } 
    
    // Else process the ops hashing
    else {
        // Get the current operation
        Operation_t op = ops->head;
        char *op_code = encode_operation(op);
        size_t op_code_size = OP_CODE_SIZE_MIN + op->data_size;

        // If the tail is null, just return the head hash
        if (ops->tail == NULL) {
            hash(op_code, op_code_size, res_buf);
        }
        
        // Else recursively process the list
        else {
            // Get the hash of the tail and prepare the hash of the head
            char *rest_hash = ops_hash(ops->tail); 
            char head_hash[HASH_SIZE];

            // Hash the head
            hash(op_code, op_code_size, head_hash);

            // Create a temporary buffer to concatenate the hashs
            char tmp_buf[HASH_SIZE * 2];
            memcpy(tmp_buf, rest_hash, HASH_SIZE);
            memcpy(tmp_buf + HASH_SIZE, head_hash, HASH_SIZE);
            
            // Hash the concatenation
            hash(tmp_buf, HASH_SIZE * 2, res_buf);

            // Free the memory
            free(rest_hash);
        }

        // Free the operation code
        free(op_code);
    }

    // Return the result
    return res_buf;
}

void print_op(Operation_t op) {
    printf("--- OPERATION ---\n");
    printf("op_type : %s\n", op_type_str(op->op_type));
    printf("data_size : %hu\n", op->data_size);
    printf("data : ");
    print_hex(op->data, op->data_size, "\n");
    printf("user_key : ");
    print_hex(op->user_key, KEY_SIZE, "\n");
    printf("signature : ");
    print_hex(op->signature, SIG_SIZE, "\n");
    printf("-----------------\n");
}
