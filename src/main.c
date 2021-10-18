#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "blake2.h"
#include "client.h"
#include "utils.h"

int main() {
    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    // Start the client
    return start_client(address, port);
}
