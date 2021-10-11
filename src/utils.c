#include <stdlib.h>

#include "utils.h"

Bloc_t new_bloc(int l, char * pred, long t, char* ope, char* c, char*s) {
    level

}


void print_bloc(Bloc_t bloc) {
    printf("===== BLOC =====\n");
    printf("level : %d\n", bloc->level);
    printf("predecessor : ");
    print_hex(bloc->predecessor, 32);
    printf("timestamp : %ld\n", bloc->timestamp); //todo : format heure
    printf("operations hash : ");
    print_hex(bloc->operations_hash, 32);
    printf("context hash : ");
    print_hex(bloc->context_hash, 32);
    printf("signature : ");
    print_hex(bloc->signature, 128);
}

void print_hex(unsigned char *thing, size_t size) {
    for(size_t i = 0; i < size ; i++) {
        printf("%02x", thing[i]);
    }
    printf("\n");
}

char compare_data(unsigned char *d1, unsigned char *d2, size_t s1, size_t s2) {
    if (s1 != s2) return 0;
    for (int i = 0 ; i < s1 ; i++) {
        if (d1[i] != d2[i]) return 0;
    }
    return 1;
}