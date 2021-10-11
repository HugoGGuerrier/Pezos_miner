#ifndef UTILS_H
#define UTILS_H

typedef struct bloc *Bloc_t;

struct
{
    int level;
    char predecessor[32];
    time_t timestamp; 
    char operations_hash[32];
    char context_hash[32];
    char signature[128];
} bloc;

Bloc_t new_bloc(int l, char * pred, time_t t, char* ope, char* c, char*s);

#endif