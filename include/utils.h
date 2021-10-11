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

struct bloc {
    int level;
    char predecessor[32];
    long timestamp; 
    char operations_hash[32];
    char context_hash[32];
    char signature[128];
};

Bloc_t new_bloc(int l, char * pred, long t, char* ope, char* c, char*s);

void print_bloc(Bloc_t bloc);

void print_hex(unsigned char *thing, size_t size);

char compare_data(unsigned char *d1, unsigned char *d2, size_t s1, size_t s2);

#endif