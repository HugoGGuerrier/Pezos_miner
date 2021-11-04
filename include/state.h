#ifndef STATE_H
#define STATE_H

#include "utils.h"

#define STATE_CODE_SIZE_MIN (KEY_SIZE + sizeof(long) + sizeof(int))



// ----- State encoding and decoding functions -----

// Take a state and encode it as a byte sequence (could reverse order of the accounts in the result)
char *encode_state(State_t state);

// Take a byte sequence and decode it as a state
State_t decode_state(char *data);

#endif