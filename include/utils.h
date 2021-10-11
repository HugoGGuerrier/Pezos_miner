#ifndef UTILS_H
#define UTILS_H

enum message {
    GET_CURRENT_HEAD,
    CURRENT_HEAD,
    GET_BLOCK,
    BLOCK,
    GET_BLOCK_OPERATIONS,
    BLOCK_OPERATIONS,
    GET_BLOCK_STATE,
    BLOCK_STATE,
    INJECT_OPERATION
};

typedef struct bloc *Bloc_t;

struct {
    int level;
    char predecessor[32];
    long timestamp; 
    char operations_hash[32];
    char context_hash[32];
    char signature[128];
} bloc;

Bloc_t new_bloc(int l, char * pred, long t, char* ope, char* c, char*s);

#endif