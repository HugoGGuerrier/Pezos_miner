#ifndef ED25519_H
#define ED25519_H

#include <stdint.h>


// Signing and verify functions
int32_t ed25519_sign(char *out, const char *in, uint64_t in_size, const char *private_key, const char *public_key);
int32_t ed25519_verify(const char *signature, const char *message, uint64_t msg_size, const char *public_key);

#endif
