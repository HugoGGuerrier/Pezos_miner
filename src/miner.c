#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "miner.h"
#include "blake2.h"

unsigned char * hash(char *name, char *surname) {
    // Compute the id length
    int name_l = strlen(name);
    int surname_l = strlen(surname);
    int l = name_l + surname_l + 2; // + 2 for the '\0' and the ':'

    // Create the string to hash 
    char *id = (char *) malloc(l);
    memcpy(id, name, name_l);
    id[name_l] = ':';
    memcpy(id + name_l + 1, surname, surname_l);

    // Prepare the result
    unsigned char *res = malloc(32); // 32 bytes hash size

    // Get the hash
    blake2b((void *) res, 32, (void *) id, l-1, NULL, 0);

    free(id);

    return res;
}

unsigned char * hash_value(unsigned char *hash, unsigned int nonce) {
    int encoded = encode_int(nonce);

    // Create the concatenate byte array
    unsigned char *concat = malloc(36);
    memcpy(concat, hash, 32);
    memcpy(concat + 32, &encoded, 4);

    // Hash the result
    unsigned char *res = malloc(32);
    blake2b((void *) res, 32, (void *) concat, 36, NULL, 0);
    free(concat);
    return res;
}

unsigned int count_zero_prefix(unsigned char *hash, size_t size) {
    int res = 0;
    int size_bit = 7;
    
    while(res < size * 8) {
        if(((0b1 << size_bit) & *hash) == 0) {
            res++;
            size_bit--;
            if(size_bit < 0) {
                size_bit = 7;
                hash = hash + 1;
            }
        } else {
            return res;
        }
    }

    return res;
}

char is_valid(unsigned char *hash, unsigned int nonce, int difficulty) {
    unsigned char *hv = hash_value(hash, nonce);
    int cpt = count_zero_prefix(hv, 32);
    free(hv);

    if(cpt >= difficulty) {
        return 1; 
    } else {
        return 0;
    }
}

unsigned int mine(unsigned char *hash, int diff) {
    unsigned int nonce = 0;
    while(nonce < (unsigned int)INT32_MAX * 2u) {
        if(is_valid(hash, nonce, diff)) {
            return nonce;
        } else {
            nonce++;
        }
    }
    return 0;
}

void print_hex(unsigned char *thing, size_t size) {
    for(size_t i = 0; i < size ; i++) {
        printf("%02x", thing[i]);
    }
    printf("\n");
}

int encode_int(int to_reverse) {
    return ((to_reverse>>24)&0xFF) | ((to_reverse<<8)&0xFF0000) | ((to_reverse>>8)&0xFF00) | ((to_reverse<<24)&0xFF000000);
}