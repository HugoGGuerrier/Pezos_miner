#include <stdlib.h>

#include "utils.h"


Bloc_t new_bloc(int level, char *pred, long time, char *ope, char *ctx_h, char* sig) {
    Bloc_t res = (Bloc_t) malloc(sizeof(struct bloc));
    res->level = level;
    res->predecessor = pred;
    res->timestamp = time;
    res->operations_hash = ope;
    res->context_hash  = ctx_h;
    res->signature = sig;
    return res;
}

void delete_block(Bloc_t bloc) {
    free(bloc->predecessor);
    free(bloc->operations_hash);
    free(bloc->context_hash);
    free(bloc->signature);
    free(bloc);
}


void print_bloc(Bloc_t bloc) {
    printf("===== BLOC =====\n");
    printf("level : %d\n", bloc->level);
    printf("predecessor : ");
    print_hex(bloc->predecessor, 32);
    printf("timestamp : %ld\n", bloc->timestamp); // TODO : format heure
    printf("operations hash : ");
    print_hex(bloc->operations_hash, 32);
    printf("\ncontext hash : ");
    print_hex(bloc->context_hash, 32);
    printf("\nsignature : ");
    print_hex(bloc->signature, 128);
    printf("\n");
}

void print_hex(unsigned char *thing, size_t size) {
    for(size_t i = 0; i < size ; i++) {
        printf("%02x", thing[i]);
    }
}


char compare_data(unsigned char *d1, size_t s1, unsigned char *d2, size_t s2) {
    if (s1 != s2) return 0;
    for (int i = 0 ; i < s1 ; i++) {
        if (d1[i] != d2[i]) return 0;
    }
    return 1;
}