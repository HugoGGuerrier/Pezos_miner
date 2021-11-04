#include <stdlib.h>

#include "signature.h"
#include "utils.h"
#include "ed25519.h"

// --- Define the keys

const char public_key[KEY_SIZE] = PUBLIC_KEY;
const char private_key[KEY_SIZE] = PRIVATE_KEY;


// --- Functions to sign and verify

void sign(char *signature, const char *data, size_t data_size) {
    ed25519_sign(signature, data, data_size, private_key);
}

int verify(const char *signature, const char *data, size_t data_size, const char *key) {
    return ed25519_verify(signature, data, data_size, key);
}