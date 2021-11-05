#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"

#define BUFF_SIZE 65536
#define RAND_SEED_SIZE 24


// Start the client and do the login on the server
pthread_t start_client(const char* addr, unsigned short port);

// Get the current chain head (cached)
Block_t get_head_client();

// Get the current head state (cached)
State_t get_state_head_client();

// Get the block of the wanted level from the server (if level is 0, get the current head)
Block_t get_block_client(unsigned int level);

// Get the state of the wanted level from the server
State_t get_state_client(unsigned int level);

// Get the operations of the wanted leveles from the server
Operations_t get_ops_client(unsigned int level);

// Send a message to the server and delete it
int send_message(Message_t message);

// Send a message and return the server response
Message_t send_message_with_response(Message_t message);

#endif