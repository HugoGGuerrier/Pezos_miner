#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include "client.h"

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
    return client_loop();
}

int auth() {
    // Get the random seed from the server
    if(read(sock_id, buff, BUFF_SIZE) == -1) {
        printf("Error in authentification : Cannor read the random seed from the server\n");
    }
    char rand_seed[24];
    memcpy(&rand_seed, &buff, sizeof(rand_seed));

    // Send the key
    

    // Send the signed hash value of the random seed
}

int client_loop() {

}


// --- Client manipulation functions

int handle_message(char *buffer) {

}

int send_message(Message_t message) {
    
}