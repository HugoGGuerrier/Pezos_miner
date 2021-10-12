#ifndef MESSAGE_H
#define MESSAGE_H

#include "utils.h"


// Encode a message to binary
char *encode_message(Message_t message);

// Decode binary to a message
Message_t decode_message(char *data);

#endif