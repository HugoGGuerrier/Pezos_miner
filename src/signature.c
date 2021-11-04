#include <stdlib.h>

#include "signature.h"
#include "utils.h"
#include "ed25519.h"

// --- Define the keys

unsigned char public_key[KEY_SIZE] = PUBLIC_KEY;
unsigned char private_key[KEY_SIZE] = PRIVATE_KEY;


// --- Functions to sign and verify

void sign(char *signature, const char *data, size_t data_size) {
    ed25519_sign((unsigned char *)signature, (const unsigned char *)data, data_size, &public_key[0], &private_key[0]);
}

int verify(const char *signature, const char *data, size_t data_size, const char *key) {
    return ed25519_verify((const unsigned char *)signature, (const unsigned char *)data, data_size, (const unsigned char *)key);
}