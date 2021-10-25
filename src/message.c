#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "message.h"
#include "operation.h"
#include "block.h"
#include "state.h"


// ----- Internal functions -----

unsigned short mess_type_to_tag(Message_Type_t msg_type) {
    switch (msg_type) {
    case GET_CURRENT_HEAD:
        return reverse_short(1);
        break;

    case CURRENT_HEAD:
        return reverse_short(2);
        break;

    case GET_BLOCK:
        return reverse_short(3);
        break;

    case BLOCK:
        return reverse_short(4);
        break;

    case GET_BLOCK_OPERATIONS:
        return reverse_short(5);
        break;

    case BLOCK_OPERATIONS:
        return reverse_short(6);
        break;

    case GET_BLOCK_STATE:
        return reverse_short(7);
        break;

    case BLOCK_STATE:
        return reverse_short(8);
        break;

    case INJECT_OPERATION:
        return reverse_short(9);
        break;
    
    default:
        printf("Error in message encoding : Unknown message type\n");
        exit(0);
    }
}

Message_Type_t mess_tag_to_type(unsigned short tag) {
    switch (tag) {
    case 1:
        return GET_CURRENT_HEAD;
        break;

    case 2:
        return CURRENT_HEAD;
        break;

    case 3:
        return GET_BLOCK;
        break;

    case 4:
        return BLOCK;
        break;

    case 5:
        return GET_BLOCK_OPERATIONS;
        break;

    case 6:
        return BLOCK_OPERATIONS;
        break;

    case 7:
        return GET_BLOCK_STATE;
        break;

    case 8:
        return BLOCK_STATE;
        break;

    case 9:
        return INJECT_OPERATION;
        break;
    
    default:
        printf("Error in message encoding : Unknown message tag\n");
        exit(0);
    }
}


// ----- Message specific creation functions -----

Message_t new_get_current_head_message() {
    Message_t res = new_message(GET_CURRENT_HEAD, 0, NULL);
    return res;
}

Message_t new_get_block_message(unsigned int level) {
    // Prepare the data
    level = reverse_int(level);
    char *data = (char *) malloc(4);
    memcpy(data, &level, sizeof(int));

    // Craft the message and return it
    Message_t res = new_message(GET_BLOCK, sizeof(int), data);
    return res;
}

Message_t new_get_block_operations_message(unsigned int level) {
    // Prepare the data
    level = reverse_int(level);
    char *data = (char *) malloc(sizeof(int));
    memcpy(data, &level, sizeof(int));

    // Craft the message and return it
    Message_t res = new_message(GET_BLOCK_OPERATIONS, sizeof(int), data);
    return res;
}

Message_t new_get_state_message(unsigned int level) {
    // Prepare the data
    level = reverse_int(level);
    char *data = (char *) malloc(sizeof(int));
    memcpy(data, &level, sizeof(int));

    // Craft the message and return it
    Message_t res = new_message(GET_BLOCK_STATE, sizeof(int), data);
    return res;
}

Message_t new_inject_operation_message(Operation_t operation) {
    char *data = encode_operation(operation);
    return new_message(INJECT_OPERATION, OP_TAG_SIZE + operation->data_size + KEY_SIZE + SIG_SIZE, data);
}


// ----- Message information getting function -----

Block_t get_current_head(Message_t message) {
    return decode_block(message->data);
}

Block_t get_block(Message_t message) {
    return decode_block(message->data);
}

Operations_t get_operations(Message_t message) {
    return decode_operations(message->data, (size_t) message->data_size);
}

State_t get_state(Message_t message) {
    return decode_state(message->data);
}


// --- Encoding and decoding functions

char *encode_message(Message_t message) {
    // Prepare the result and the pointer
    char *data_res = (char *) malloc(MSG_SIZE_SIZE + MSG_TAG_SIZE + message->data_size);
    char *data_ptr = data_res;

    // Add the message size
    unsigned short msg_size = reverse_short(MSG_TAG_SIZE + message->data_size);
    memcpy(data_ptr, &msg_size, MSG_SIZE_SIZE);
    data_ptr += MSG_SIZE_SIZE;

    // Add the message tag
    unsigned short tag = mess_type_to_tag(message->tag);
    memcpy(data_ptr, &tag, MSG_TAG_SIZE);
    data_ptr += MSG_TAG_SIZE;

    // Add the message data
    memcpy(data_ptr, message->data, message->data_size);

    // Return the result
    return data_res;
}

Message_t decode_message(char *data) {
    // Get the message size
    unsigned short msg_size = reverse_short(*((unsigned short *) data)) - MSG_TAG_SIZE;
    data += MSG_SIZE_SIZE;

    // Get the tag
    unsigned short tag = reverse_short(*((unsigned short *) data));
    Message_Type_t msg_type = mess_tag_to_type(tag);
    data += MSG_TAG_SIZE;

    // Copy the data
    char *msg_data = (char *) malloc(msg_size);
    memcpy(msg_data, data, msg_size);

    // Create the message
    return new_message(msg_type, msg_size, msg_data);
}