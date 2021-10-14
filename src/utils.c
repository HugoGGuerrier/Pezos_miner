#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"


// --- Message functions 

Message_t new_message(Message_Type_t tag, char *data) {
    Message_t res = (Message_t) malloc(sizeof(struct message));
    res->tag = tag;
    res->data = data;
    return res;
}

void delete_message(Message_t message) {
    free(message->data);
    free(message);
}


// --- Operation functions

Operation_t new_operation(Operation_Type_t op_type, char *data) {
    Operation_t res = (Operation_t) malloc(sizeof(struct operation));
    res->op_type = op_type;
    res->data = data;
    return res;
}

void delete_operation(Operation_t operation) {
    free(operation->data);
    free(operation);
}


// --- Operations functions

Operations_t new_operations(Operation_t head, Operations_t tail) {
    Operations_t res = (Operations_t) malloc(sizeof(struct operations));
    res->head = head;
    res->tail = tail;
    return res;
}

void delete_operations(Operations_t operations) {
    if(operations != NULL) {
        delete_operations(operations->tail);
        free(operations->head);
    }
}


// --- Block functions

Block_t new_block(int level, char *pred, long time, char *ope, char *ctx_h, char* sig) {
    Block_t res = (Block_t) malloc(sizeof(struct block));
    res->level = level;
    res->predecessor = pred;
    res->timestamp = time;
    res->operations_hash = ope;
    res->context_hash  = ctx_h;
    res->signature = sig;
    return res;
}

void delete_block(Block_t block) {
    free(block->predecessor);
    free(block->operations_hash);
    free(block->context_hash);
    free(block->signature);
    free(block);
}


// --- Printing functions

void print_block(Block_t block) {
    printf("===== BLOCK =====\n");
    printf("level : %d\n", block->level);
    printf("predecessor : ");
    print_hex(block->predecessor, 32);
    printf("timestamp : %ld\n", block->timestamp); // TODO : format heure
    printf("operations hash : ");
    print_hex(block->operations_hash, 32);
    printf("\ncontext hash : ");
    print_hex(block->context_hash, 32);
    printf("\nsignature : ");
    print_hex(block->signature, 128);
    printf("\n");
}

void print_hex(char *thing, size_t size) {
    for(size_t i = 0; i < size ; i++) {
        printf("%02x", thing[i]);
    }
}


// --- Miscallenous functions 

char compare_data(char *d1, size_t s1, char *d2, size_t s2) {
    if (s1 != s2) return 0;
    for (size_t i = 0 ; i < s1 ; i++) {
        if (d1[i] != d2[i]) return 0;
    }
    return 1;
}


Block_t encode_to_block(char *in) {

  
    char *level_temp =  malloc (8 * sizeof (char));
    char *predecessor =  malloc (64 * sizeof (char));
    char *timestamp_temp =   malloc (16 * sizeof (char));
    char *operations_hash =  malloc (64 * sizeof (char));
    char *context_hash =  malloc (64 * sizeof (char));
    char *signature =  malloc (128 * sizeof (char));
    int level =0; 

    strncpy(level_temp, in, 8);
    strncpy(predecessor, in+8, 64);
    strncpy(timestamp_temp, in+8+64, 16);
    strncpy(operations_hash, in+8+64+16, 64);
    strncpy(context_hash, in+8+64+16+64, 64);
    strncpy(signature, in+8+64+16+64+64, 128);
    char *eptr;
    long timestamp = strtol(timestamp_temp, &eptr, 10);
    int base = 1;
    int i = 0, value, length;


    length = strlen(level_temp);
    for(i = length--; i >= 0; i--)
    {
        if(level_temp[i] >= '0' && level_temp[i] <= '9')
        {
            level += (level_temp[i] - 48) * base;
            base *= 16;
        }
        else if(level_temp[i] >= 'A' && level_temp[i] <= 'F')
        {
            level += (level_temp[i] - 55) * base;
            base *= 16;
        }
        else if(level_temp[i] >= 'a' && level_temp[i] <= 'f')
        {
            level += (level_temp[i] - 87) * base;
            base *= 16;
        }
    }

    return new_block(level, predecessor, timestamp, operations_hash, operations_hash, signature);


}