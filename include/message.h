#ifndef MESSAGE_H
#define MESSAGE_H

#include "utils.h"

#define MSG_SIZE_SIZE 2
#define MSG_TAG_SIZE 2
#define MSG_CODE_SIZE_MIN (MSG_SIZE_SIZE + MSG_TAG_SIZE)


// ----- Message specific creation functions -----

// Create a GET CURRENT HEAD message
Message_t new_get_current_head_message();

// Create a GET BLOCK message
Message_t new_get_block_message(unsigned int level);

// Create a GET BLOCK OPERATIONS message
Message_t new_get_block_operations_message(unsigned int level);

// Create a GET STATE message
Message_t new_get_state_message(unsigned int level);

// Create an INJECT OPERATION message
Message_t new_inject_operation_message(Operation_t operation);


// ----- Message information getting function -----

// Get the current head from a CURRENT HEAD message
Block_t get_current_head(Message_t message);

// Get the block from a BLOCK message
Block_t get_block(Message_t message);

// Get the operations list from a BLOCK OPERATIONS message
Operations_t get_operations(Message_t message);

// Get the state from a BLOCK STATE message
State_t get_state(Message_t message);


// ----- Message encoding and decoding functions -----

// Encode a message to binary
char *encode_message(Message_t message);

// Decode binary to a message
Message_t decode_message(char *data);


// ----- Utils functions -----

// Get the string for a message type
char *msg_type_str(const Message_Type_t type);

#endif