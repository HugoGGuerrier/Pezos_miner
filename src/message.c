#include <string.h>
#include <stdlib.h>

#include "message.h"
#include "operation.h"
#include "block.h"
#include "state.h"


// ----- Message specific creation functions -----

Message_t new_get_current_head_message() {
    Message_t res = new_message(GET_CURRENT_HEAD, 0, NULL);
    return res;
}

Message_t new_get_block_message(unsigned int level) {
    level = reverse_int(level);
    char *data = (char *) malloc(4);
    memcpy(data, &level, 4);

    Message_t res = new_message(GET_BLOCK, 4, data);
    return res;
}

Message_t new_get_block_operations_message(unsigned int level) {
    level = reverse_int(level);
    char *data = (char *) malloc(4);
    memcpy(data, &level, 4);

    Message_t res = new_message(GET_BLOCK_OPERATIONS, 4, data);
    return res;
}

Message_t new_get_state_message(unsigned int level) {
    level = reverse_int(level);
    char *data = (char *) malloc(4);
    memcpy(data, &level, 4);

    Message_t res = new_message(GET_BLOCK_STATE, 4, data);
    return res;
}

Message_t new_inject_operation_message(Operation_t operation) {
    char *data = encode_operation(operation);
    return new_message(INJECT_OPERATION, operation->data_size + 2 + 32 + 64, data);
}


// ----- Message information getting function -----

Block_t get_current_head(Message_t message) {
    return decode_block(message->data);
}

Block_t get_block(Message_t message) {
    return decode_block(message->data);
}

Operations_t get_operations(Message_t message) {
    Operations_t res = NULL;
}

State_t get_state(Message_t message) {

}


// --- Encoding and decoding functions

char *encode_message(Message_t message) {
    // TODO
    return NULL;
}

Message_t decode_message(char *data) {
    // TODO
    return NULL;
}