#ifndef MINER_H
#define MINER_H

/** Hash a name and a surname */
unsigned char * hash(char *, char *);

/** Concatenate the integer big-endian with the given hash */
unsigned char * hash_value(unsigned char *, unsigned int);

/** Count the number of 0 bits on the prefix */
unsigned int count_zero_prefix(unsigned char *, size_t);

/** Verify if the created hash is valid following its parameters */
char is_valid(unsigned char *, unsigned int, int);

/** Mine a bloc with the wanted difficulty */
unsigned int mine(unsigned char *, int);

/** Display an hexadecimal char array */
void print_hex(unsigned char *, size_t);

/** Reverse the int endianess */
int encode_int(int);

#endif