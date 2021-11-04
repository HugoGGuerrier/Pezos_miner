#ifndef CLIENT_H
#define CLIENT_H

#include "utils.h"

#define BUFF_SIZE 65536
#define RAND_SEED_SIZE 24
#define TIMEOUT_SEC 10


// Start the client and do the login on the server
int start_client(const char *addr, unsigned short port);

// Send a message to the server and delete it
int send_message(Message_t message);

// Send a message and return the server response
Message_t send_message_with_response(Message_t message);

#endif