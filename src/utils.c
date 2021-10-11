#include <stdlib.h>

#include "utils.h"


Bloc_t new_bloc(int l, char *pred, long t, char *ope, char *c, char* s) {
    Bloc_t res = (Bloc_t) malloc(sizeof(struct bloc));
    res->level = l;
    res->predecessor = pred;
    res->timestamp = t; 
    res->operations_hash = ope; 
    res->context_hash  = c; 
    res->signature = s;

    return res;
}



/*
0000002c1c80203a30e5de4d980cc555131d1b4a4750edc82c0c443179d88de1ae4f6cdf00000000
616304e5000000000000000000000000000000000000000000000000000000000000000022a00d1c
8c0fbaefedd71ddb83d455033efd259a8f0adf189b9f850a0d1945f2cc3faffc696c86db13d50752
fdb7edd0ee1ce19ab350f60899939fc139d58996419c13b812b7f005fafaf23924d2f1df555036bc
61e7b67cb679375e5756b306
*/

/*
Bloc_t convert_txt_to_bloc (const char *b){
    //tronquer le txt 
    int level = 
    

return  0;
}

char* convert_bloc_to_txt(Bloc_t bloc) {
  char txt[344] = "";
  return name;
}
*/

void print_bloc(Bloc_t bloc) {
    printf("===== BLOC =====\n");
    printf("level : %d\n", bloc->level);
    printf("predecessor : ");
    print_hex(bloc->predecessor, 32);
    printf("timestamp : %ld\n", bloc->timestamp); // TODO : format heure
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