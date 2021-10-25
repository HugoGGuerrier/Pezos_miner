#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "blake2.h"
#include "client.h"
#include "utils.h"
#include "block.h"


void test_account() {

}


void test_block() {

}

void test_message() {
    
}

void test_operation() {

}

void test_state() {
    
}

void tests() {
    test_account();
    test_block();
    test_message();
    test_operation();
    test_state();
}

int main() {
    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    // Start the client
    return start_client(address, port);
}
