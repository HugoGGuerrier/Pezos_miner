#ifndef UTILS_H
#define UTILS_H

#define PUBLIC_KEY 0xaca76354de343ef09385e263fb59561855d3cbf167961c6955624d91aa7eecf5
#define PRIVATE_KEY 0xd9661d1a709518ef6213117508acbae0c9b04f338d017a28633bc4a1c15e1369


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