#ifndef UTILS_H
#define UTILS_H

#include "account.h"
#include "message.h"
#include "operation.h"
#include "block.h"
#include "state.h"


// ----- Hard define the public and private key (Not very secure...) -----

#define HASH_SIZE 32
#define KEY_SIZE 32
#define SIG_SIZE 64
#define PUBLIC_KEY {0xac, 0xa7, 0x63, 0x54, 0xde, 0x34, 0x3e, 0xf0, 0x93, 0x85, 0xe2, 0x63, 0xfb, 0x59, 0x56, 0x18, 0x55, 0xd3, 0xcb, 0xf1, 0x67, 0x96, 0x1c, 0x69, 0x55, 0x62, 0x4d, 0x91, 0xaa, 0x7e, 0xec, 0xf5}
#define PRIVATE_KEY {0xd9, 0x66, 0x1d, 0x1a, 0x70, 0x95, 0x18, 0xef, 0x62, 0x13, 0x11, 0x75, 0x08, 0xac, 0xba, 0xe0, 0xc9, 0xb0, 0x4f, 0x33, 0x8d, 0x01, 0x7a, 0x28, 0x63, 0x3b, 0xc4, 0xa1, 0xc1, 0x5e, 0x13, 0x69}


// ----- Utils function definitions -----


// --- Printing functions

// Print a data in hexadecimal form
void print_hex(char *thing, size_t size, const char *end);


// --- Miscallenous functions

// Compare two arbitrary data
char compare_data(char *d1, size_t s1, char *d2, size_t s2);

// Reverse the short endianness
long reverse_long(long src);

// Reverse the integer endianness
int reverse_int(int src);

// Reverse the short endianness
short reverse_short(short src);

// Read bytes from an hexadecimal string
char *read_hex_string(char *hex_str);

// Get a copy of the public key
char *get_public_key_copy();

// Get a copy of the private key
char *get_private_key_copy();

#endif