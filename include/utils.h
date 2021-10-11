#ifndef UTILS_H
#define UTILS_H

#define PUBLIC_KEY 0xaca76354de343ef09385e263fb59561855d3cbf167961c6955624d91aa7eecf5
#define PRIVATE_KEY 0xd9661d1a709518ef6213117508acbae0c9b04f338d017a28633bc4a1c15e1369

typedef enum message Message_t;
typedef enum operation_type Operation_type_t;
typedef struct bloc *Bloc_t;

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

enum operation_type {
    BAD_PREDECESSOR,
    BAD_TIMESTAMP,
    BAD_OPERATIONS_HASH,
    BAD_CONTEXT_HASH,
    BAD_SIGNATURE
};

struct operation {
    Operation_type_t op_type;
    void *data;
};

/*
operations =
| BAD PREDECESSOR <hash>
| BAD TIMESTAMP <time>
| BAD OPERATIONS HASH <hash>
| BAD CONTEXT HASH <hash>
| BAD SIGNATURE
*/

struct bloc {
    int level;
    char *predecessor;
    long timestamp; 
    char *operations_hash;
    char *context_hash;
    char *signature;
};

Bloc_t new_bloc(int l, char * pred, long t, char* ope, char* c, char*s);

void print_bloc(Bloc_t bloc);

void print_hex(unsigned char *thing, size_t size);

char compare_data(unsigned char *d1, unsigned char *d2, size_t s1, size_t s2);

#endif