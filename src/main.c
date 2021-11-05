#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "main.h"
#include "client.h"
#include "repl.h"
#include "test.h"

int main(int argc, const char *argv[]) {
    // Display the infos
    printf("====================================================\n");
    printf("  Pezos miner : Le groupe C\n\n");
    printf("  Version : 0.7\n");
    printf("  License : MIT\n");
    printf("====================================================\n");

    // Parse the arg to see if it is the test mode
    if(argc >= 2) {
        if(strcmp(argv[1], "-t") == 0 || strcmp(argv[1], "-test") == 0) {
            run_tests();
            return 0;
        }
    }

    // Define the server attributes
    const char *addr = "78.194.168.67";
    unsigned short port = 1337;

    // Start the miner in a thread
    pthread_t miner_thread = start_client(addr, port);

    // Start the repl
    start_repl(miner_thread);

    // Ensure the miner thread is finished
    pthread_join(miner_thread, NULL);
}
