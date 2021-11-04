#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <byteswap.h>

#include "utils.h"
#include "message.h"
#include "operation.h"


// --- Printing functions

void print_hex(char *thing, size_t size, const char *end) {
    for (size_t i = 0; i < size; i++) {
        printf("%02x", thing[i] & 0xff);
    }
    printf("%s", end);
}


// --- Miscallenous functions

char compare_data(char *d1, size_t s1, char *d2, size_t s2) {
    if (s1 != s2)
        return 0;
    for (size_t i = 0; i < s1; i++) {
        if (d1[i] != d2[i])
            return 0;
    }
    return 1;
}

long reverse_long(long src) {
    return bswap_64(src);
}

int reverse_int(int src) {
    return bswap_32(src);
}

short reverse_short(short src) {
    return bswap_16(src);
}

char *read_hex_string(char *hex_str) {
    int hex_str_len = strlen(hex_str);
    char *pos = hex_str;

    // One byte is represented by two hexadecimal chars
    char *res = (char *) malloc(hex_str_len / 2);

    // Iterate pairs on the hexa string
    for (int i = 0; i < hex_str_len / 2; i++) {
        sscanf(pos, "%2hhx", &res[i]);
        pos += 2;
    }

    // Return the result
    return res;
}

char *get_public_key_copy() {
    char *res = (char *) malloc(KEY_SIZE);
    char pk[KEY_SIZE] = PUBLIC_KEY;
    memcpy(res, pk, KEY_SIZE);

    return res;
}

char *get_private_key_copy() {
    char *res = (char *) malloc(KEY_SIZE);
    char pk[KEY_SIZE] = PRIVATE_KEY;
    memcpy(res, pk, KEY_SIZE);

    return res;
}