#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "client.h"

int main(int argc, char const *argv[])
{
    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    // Start the client
    return start_client(address, port);
}
