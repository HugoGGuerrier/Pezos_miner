#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "client.h"
#include "hash.h"
#include "signature.h"

// --- Function declaration

int auth();
int client_loop();
int test_connection();
int sanitize_read();


// --- Global variables definition

int sock_id;
char buff[BUFF_SIZE];
Block_t current_head = NULL;


// --- Client running code

int start_client(const char *addr, unsigned short port) {
    // Create the socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == -1) {
        printf("ERROR : Cannot create the socket\n");
        return -1;
    }
    printf("Socket created !\n");

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
        return -1;
    }
    printf("Connected to the dictator server !\n");

    // Authetificate the client
    printf("Try authentification process...\n");
    if(auth() != 0) {
        printf("ERROR : Authetification failed\n");
        return -1;
    }
    printf("Authentification success !\n");

    // Start the client loop
    printf("Start listening to the server...\n");
    return client_loop();
}

int auth() {
    // Read the random seed from the server
    if(read(sock_id, buff, BUFF_SIZE) == -1) {
        printf("ERROR : Cannot read the random seed from the server\n");
        return -1;
    }
    char rand_seed[RAND_SEED_SIZE];
    memcpy(&rand_seed, &buff, RAND_SEED_SIZE);

    // Print the random seed
    printf("Random seed = ");
    print_hex(rand_seed, RAND_SEED_SIZE, "\n");

    // Get the public key
    char public_key[KEY_SIZE] = PUBLIC_KEY;

    // Prepare the message to send to the server
    char public_key_msg[2 + KEY_SIZE];
    public_key_msg[0] = 0;
    public_key_msg[1] = KEY_SIZE;
    memcpy(public_key_msg + 2, public_key, KEY_SIZE);

    // Print the sending of the public key
    printf("Sending the public key...\n");

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

    // Print the hash signature and the sending action
    printf("Sending the random seed hash signature...\n");

    // Send the signature to the server
    if(write(sock_id, rand_hash_sign, 2 + SIG_SIZE) == -1) {
        printf("ERROR : Cannot send the random seed hash signature to the server\n");
        return -1;
    }

    // Print the testing connection
    printf("Testing connection...\n");

    // Test the connection
    if(test_connection() != 0) {
        printf("ERROR : Server didn't accept the connection\n");
        return -1;
    }

    // Print the success
    printf("Connection success !\n");

    // Return the authetification success
    return 0;
}

int client_loop() {
    // Infinite loop to listen to the server
    while(1) {
        // Read the next block
        printf("Waiting for a new block...\n");
        if(sanitize_read() == -1) {
            printf("ERROR : Cannot listen to the server\n");
        }

        // Read the message
        Message_t msg = decode_message(buff);

        // If the message is not a current head
        if(msg->msg_type != CURRENT_HEAD) {
            printf("Unexpected message : \n");
            print_message(msg);
        } else {
            
            // Get the block
            Block_t head = get_current_head(msg);
            printf("New head recieved !\n");
            print_block(head);


            // Get the predecessor
            Block_t pred;
            if(current_head == NULL) {
                // Get the predecessor from the server
                printf("--- Get the predecessor :\n");
                Message_t get_pred = new_get_block_message(head->level - 1);
                Message_t pred_resp = send_message_with_response(get_pred);
                pred = get_block(pred_resp);

                // Free the memory
                delete_message(get_pred);
                delete_message(pred_resp);

                // Display it
                print_block(pred);
            } else {
                pred = current_head;
            }


            // Get the current state
            printf("--- Get the current state :\n");
            Message_t get_cur_state = new_get_state_message(head->level);
            Message_t state_resp = send_message_with_response(get_cur_state);
            State_t state = get_state(state_resp);

            // Free the memory
            delete_message(get_cur_state);
            delete_message(state_resp);

            // Display the state without all accounts
            print_state_l(state);


            // Get the block operations
            printf("--- Get the block operations :\n");
            Message_t get_ops = new_get_block_operations_message(head->level);
            Message_t ops_resp = send_message_with_response(get_ops);
            Operations_t ops = get_operations(ops_resp);

            // Free the memory
            delete_message(get_ops);
            delete_message(ops_resp);

            // Display the operations
            Operations_t ptr = ops;
            while (ptr != NULL) {
                print_op(ptr->head);
                ptr = ptr->tail;
            }
            
            // Verify the bloc
            Operation_t correction = verify_bloc(head, pred, state, ops);

            printf("--- Correction to the block :\n");
            print_op(correction);

            // Send the correction
            printf("Sending the correction...\n");
            Message_t correction_msg = new_inject_operation_message(correction);
            if(send_message(correction_msg) == -1) {
                printf("ERROR : Cannot send the correction to the client\n");
                return -1;
            }
            printf("Done !\n");

            // Free the memory
            delete_block(pred);
            delete_state(state);
            delete_operations(ops);
            delete_operation(correction);
            delete_message(correction_msg);

            // Set the new head
            current_head = head;

        }

        // Free the memory
        delete_message(msg);
    }

    // Return the success
    return 0;
}


// --- Client manipulation functions

int test_connection() {
    // Prepare the working variables
    int error = 0;
    socklen_t len = sizeof(error);

    // Get the socket state
    int res = getsockopt(sock_id, SOL_SOCKET, SO_ERROR, &error, &len);

    // Return the result
    if(res != 0 && error != 0) return -1;
    
    // Try to get the chain head
    Message_t get_head = new_get_current_head_message();
    Message_t response = send_message_with_response(get_head);
    delete_message(get_head);

    // Test if the response is null
    if(response == NULL) return -1;

    // Test the message
    if(response->msg_type != CURRENT_HEAD) {
        delete_message(response);
        return -1;
    }

    // Decode the message data
    Block_t head = get_current_head(response);

    // Free the memory
    delete_message(response);

    // Print the current head
    printf("Current head : \n");
    print_block(head);

    // Free the memory
    delete_block(head);


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

    // Return the result
    return res;
}