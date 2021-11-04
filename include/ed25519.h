#ifndef ED25519_H
#define ED25519_H

#include <stdint.h>


// Signing and verify functions
void ed25519_sign(char *out, const char *in, unsigned long in_size, const char *private_key);
int32_t ed25519_verify(const char *signature, const char *message, unsigned long msg_size, const char *public_key);

#endif
