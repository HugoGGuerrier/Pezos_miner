#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include "client.h"
#include "hash.h"
#include "signature.h"

// --- Function declaration

int auth();
void *client_loop(void *);
int test_connection();
int sanitize_read();


// --- Global variables definition

int sock_id;
char buff[BUFF_SIZE];
pthread_mutex_t lock;
Block_t current_head = NULL;
State_t current_state = NULL;


// --- Client running code

pthread_t start_client(const char* addr, unsigned short port) {
    // Create the socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == -1) {
        printf("ERROR : Cannot create the socket\n");
        exit(-1);
    }

    // Intialize the mutex
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&lock, &attr);

    // Set the explicit blocking mode
    int mode = 0;
    ioctl(sock_id, FIONBIO, &mode);

    // Create the server address
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(addr);
    server_address.sin_port = htons(port);

    // Connect to the server
    if(connect(sock_id, (struct sockaddr *)&server_address, sizeof(server_address)) != 0) {
        printf("ERROR : Cannot connect to the server\n");
        exit(-1);
    }
    printf("Connected to the dictator server !\n");

    // Authentificate the client
    if(auth() != 0) {
        printf("ERROR : Authentification failed\n");
        exit(-1);
    }
    printf("Authentification success !\n");

    // Start the client loop
    printf("Start the miner and REPL (type \"help\" for further details)...\n");
    pthread_t res;
    if(pthread_create(&res, NULL, &client_loop, NULL) != 0) {
        printf("ERROR : Cannot start the miner thread\n");
        exit(-1);
    }
    return res;
}

int auth() {
    // Read the random seed from the server
    if(read(sock_id, buff, BUFF_SIZE) == -1) {
        printf("ERROR : Cannot read the random seed from the server\n");
        return -1;
    }
    char rand_seed[RAND_SEED_SIZE];
    memcpy(&rand_seed, &buff, RAND_SEED_SIZE);

    // Get the public key
    char public_key[KEY_SIZE] = PUBLIC_KEY;

    // Prepare the message to send to the server
    char public_key_msg[2 + KEY_SIZE];
    public_key_msg[0] = 0;
    public_key_msg[1] = KEY_SIZE;
    memcpy(public_key_msg + 2, public_key, KEY_SIZE);

    // Send the key to the server
    if(write(sock_id, public_key_msg, 2 + KEY_SIZE) == -1) {
        printf("ERROR : Cannot send the public key to the server\n");
        return -1;
    }

    // Hash the random seed
    char rand_hash[HASH_SIZE];
    hash(rand_seed, RAND_SEED_SIZE, rand_hash);

    // Prepare the message for the signed hashed random seed and its size
    char rand_hash_sign[2 + SIG_SIZE];
    rand_hash_sign[0] = 0;
    rand_hash_sign[1] = SIG_SIZE;
    sign(rand_hash_sign + 2, rand_hash, HASH_SIZE);

    // Send the signature to the server
    if(write(sock_id, rand_hash_sign, 2 + SIG_SIZE) == -1) {
        printf("ERROR : Cannot send the random seed hash signature to the server\n");
        return -1;
    }

    // Test the connection
    if(test_connection() != 0) {
        printf("ERROR : Server didn't accept the connection\n");
        return -1;
    }

    // Return the authetification success
    return 0;
}

void *client_loop(void *ignored) {
    // Infinite loop to listen to the server
    while(1) {
        // Sleep until the next block
        time_t cur_time = time(NULL);
        int sleep_time = BLOCK_TIME - ((cur_time - current_head->timestamp) + 1);
        if(sleep_time < 0) sleep_time = 0;
        sleep(sleep_time);

        // Lock the socket
        pthread_mutex_lock(&lock);

        // Read the next block
        if(sanitize_read() == -1) {
            printf("ERROR : Cannot listen to the server\n");
        }

        // Read the message
        Message_t msg = decode_message(buff);

        // If the message is not a current head
        if(msg->msg_type != CURRENT_HEAD) {
            printf("\nUnexpected message : \n");
            print_message(msg);
            printf("> ");
            fflush(stdout);
        } else {
            
            // Get the block
            Block_t head = get_current_head(msg);
            printf("\nNew block recieved ! ");


            // Get the predecessor
            Block_t pred;
            if(current_head == NULL) {
                pred = get_block_client(head->level - 1);
            } else {
                pred = current_head;
            }

            // Get the current state
            State_t state = get_state_client(head->level);

            // Get the block operations
            Operations_t ops = get_ops_client(head->level);
            
            // Verify the bloc
            Operation_t correction = verify_bloc(head, pred, state, ops);
            printf("Correction : %s\n", op_type_str(correction->op_type));

            // Set the correct timestamp
            if(correction->op_type == BAD_TIMESTAMP) {
                head->timestamp = reverse_long((*(unsigned long *) correction->data));
            }

            // Send the correction
            printf("Sending the correction... ");
            Message_t correction_msg = new_inject_operation_message(correction);
            if(send_message(correction_msg) == -1) {
                printf("ERROR : Cannot send the correction to the client\n");
                exit(-1);
            }
            printf("Done !\n> ");
            fflush(stdout);

            // Free the memory
            delete_block(pred);
            delete_state(current_state);
            delete_operations(ops);
            delete_operation(correction);
            delete_message(correction_msg);

            // Set the new head
            current_head = head;
            current_state = state;

        }

        pthread_mutex_unlock(&lock);

        // Free the memory
        delete_message(msg);
    }
}


// --- Client manipulation functions

Block_t get_head_client() {
    return current_head;
}

State_t get_state_head_client() {
    return current_state;
}

Block_t get_block_client(unsigned int level) {
    // Create the request message
    Message_t request;
    if(level == 0) {
        request = new_get_current_head_message();
    } else {
        request = new_get_block_message(level);
    }

    // Send the message and get the response
    Message_t response = send_message_with_response(request);

    // Get the result
    Block_t res;
    if(response->msg_type == BLOCK) {
        res = get_block(response);
    } else if(response->msg_type == CURRENT_HEAD) {
        res = get_current_head(response);
    } else {
        res = NULL;
    }

    // Free the memory
    delete_message(request);
    delete_message(response);

    // Return the result
    return res;
}

State_t get_state_client(unsigned int level) {
    // Create the request and get the response
    Message_t request = new_get_state_message(level);
    Message_t response = send_message_with_response(request);

    // Get the result
    State_t res;
    if(response->msg_type == BLOCK_STATE) {
        res = get_state(response);
    } else {
        res = NULL;
    }

    // Free the memory
    delete_message(request);
    delete_message(response);

    // Return the result
    return res;
}

Operations_t get_ops_client(unsigned int level) {
    // Create the request and get the response
    Message_t request = new_get_block_operations_message(level);
    Message_t response = send_message_with_response(request);

    // Get the result
    Operations_t res;
    if(response->msg_type == BLOCK_OPERATIONS) {
        res = get_operations(response);
    } else {
        res = NULL;
    }

    // Free the memory
    delete_message(request);
    delete_message(response);

    // Return the result
    return res;
}

int test_connection() {
    // Prepare the working variables
    int error = 0;
    socklen_t len = sizeof(error);

    // Get the socket state
    int res = getsockopt(sock_id, SOL_SOCKET, SO_ERROR, &error, &len);

    // Return the result
    if(res != 0 && error != 0) return -1;

    // Decode the message data
    Block_t head = get_block_client(0);

    // Verify the head consistency
    if(head == NULL) return -1;

    // Set the current head
    current_head = head;

    State_t state = get_state_client(head->level);
    current_state = state;
    head->timestamp = state->predecessor_timestamp + BLOCK_TIME;

    // Return the success
    return 0;
}

int sanitize_read() {

    // Read the message and place it in the buffer start + 2
    if(read(sock_id, &buff[2], BUFF_SIZE - 2) == -1) {
        return -1;
    }

    // Analyse the buff message tag to place the size
    unsigned short msg_size;
    short tag = reverse_short(*((short *) &buff[2]));
    switch (tag)
    {
    case 2:
    case 4:
        msg_size = MSG_TAG_SIZE + BLOCK_CODE_SIZE;
        break;

    case 6:
        msg_size = reverse_short(*((unsigned short *) &buff[4])) + MSG_TAG_SIZE + 2;
        break;

    case 8:
        msg_size = reverse_int(*((unsigned int *) &buff[44])) + MSG_TAG_SIZE + STATE_CODE_SIZE_MIN;
        break;
    
    default:
        printf("Cannot sanitize the buffer : Unknown message tag %hd\n", tag);
        exit(1);
    }

    // Set the message size
    buff[0] = (msg_size & 0xff00) >> 8;
    buff[1] = (msg_size & 0x00ff);

    return 0;
}

int send_message(Message_t message) {
    // Send the message data
    char *data = encode_message(message);
    int res = write(sock_id, data, MSG_CODE_SIZE_MIN + message->data_size);

    // Free the memory
    free(data);

    // Return the result
    return res;
}

Message_t send_message_with_response(Message_t message) {
    // Lock
    pthread_mutex_lock(&lock);

    // Send the message
    if(send_message(message) == -1) {
        return NULL;
    }

    // Read the response
    if(sanitize_read() == -1) {
        printf("ERROR : Cannot read the message response\n");
        return NULL;
    }

    // Decode the message
    Message_t res = decode_message(buff);

    // Unlock
    pthread_mutex_unlock(&lock);

    // Return the result
    return res;
}