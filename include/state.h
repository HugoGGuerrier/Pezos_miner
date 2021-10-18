#ifndef STATE_H
#define STATE_H

#include "utils.h"

// Take a state and encode it in a byte sequence
char * encode_state(State_t state);

// Take a byte sequence and decode it as a state
State_t decode_state(char *data);

#endif