#include <stdio.h>
#include <string.h>

#include "main.h"
#include "test.h"

int main(int argc, char *argv[]) {
    // Parse the arg to see if it is the test mode
    if(argc >= 2) {
        if(strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "-test") == 0) {
            run_tests();
            return 0;
        }
    }

    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    // Start the client
    return start_client(address, port);
}
