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

typedef struct Bloc_t
{
    
    int level;
    char predecessor[32];
    time_t timestamp; 
    char operations_hash[32];
    char context_hash[32];
    char signature[128];
};

Bloc_t *new_bloc(int l, char * pred, time_t t, char* ope, char* c, char*s) {
    level

}

