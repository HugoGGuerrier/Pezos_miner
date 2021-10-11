#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "client.h"
#include "hash.h"
#include "signature.h"

// --- Constant definition

#define RAND_SEED_SIZE 24
#define HASH_SIZE 32

// --- Function declaration

int auth();
int client_loop();
int test_connection();
int handle_message();


// --- Global variables definition

int sock_id;

char buff[BUFF_SIZE];


// --- Client running code

int start_client(const char *addr, unsigned short port) {
    // Create the socket
    sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_id == -1) {
        printf("Cannot create the socket...\n");
        return 1;
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
        printf("Cannot connect to the server...\n");
        return 1;
    }
    printf("Connected to the dictator server !\n");

    // Authetificate the client
    printf("Try authentification process...\n");
    if(auth() != 0) {
        printf("Authetification failed...\n");
        return 1;
    }
    printf("Authentification success !\n");

    // Start the client loop
    printf("Start listening to the server...\n");
    return client_loop();
}

int auth() {
    // Get the random seed from the server
    if(read(sock_id, buff, BUFF_SIZE) == -1) {
        printf("Error in authentification : Cannot read the random seed from the server\n");
        return 1;
    }
    char rand_seed[RAND_SEED_SIZE];
    memcpy(&rand_seed, &buff, RAND_SEED_SIZE);

    // Send the key
    char public_key[KEY_SIZE] = PUBLIC_KEY;
    if(write(sock_id, &public_key, KEY_SIZE) == -1) {
        printf("Error in authentification : Cannot send the public key to the server\n");
        return 1;
    }

    // Send the signed hash value of the random seed
    char rand_hash[HASH_SIZE];
    hash(rand_seed, RAND_SEED_SIZE, rand_hash);
    char rand_hash_sign[64];
    sign(rand_hash_sign, rand_hash, HASH_SIZE);
    if(write(sock_id, rand_hash_sign, 64) == -1) {
        printf("Error in authentification : Cannot send the random seed hash signature to the server\n");
        return 1;
    }

    if(test_connection() != 0) {
        printf("Error in authentification : Server didn't acceot the connection\n");
        return 1;
    }

    return 0;
}

int client_loop() {
    return 0;
}


// --- Client manipulation functions

int test_connection() {
    // TODO : Test the connection by sending a simple request
    return 0;
}

int handle_message() {
    // TODO : Handle the message recieving
    return 0;
}

int send_message(Message_t message) {
    // TODO : Encode the message and send it to the server
    return 0;
}