#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include "blake2.h"
#include "client.h"
#include "utils.h"
#include "block.h"

int main() {
    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    printf("\n=============== TEST ===============\n\n");

    // Test the account encoding and decoding

    // Test the state encoding and decoding

    // Test the block encoding and decoding

    int d = 42;
    print_hex(&d, 1, NULL);
    printf("\n");

    char *h = read_hex_string("00001f");
    print_hex(h, 3, NULL);
    printf("\n");


    printf("\n===== Test block encoding and decoding =====\n");

    int level = 44;
    char *predecessor = read_hex_string("1c80203a30e5de4d980cc555131d1b4a4750edc82c0c443179d88de1ae4f6cdf");
    long timestamp = 0; // :') 
    char *operations_hash = 0;
    char *context_hash = read_hex_string("22a00d1c8c0fbaefedd71ddb83d455033efd259a8f0adf189b9f850a0d1945f2");
    char *signature = read_hex_string("cc3faffc696c86db13d50752fdb7edd0ee1ce19ab350f60899939fc139d58996419c13b812b7f005fafaf23924d2f1df555036bc61e7b67cb679375e5756b306");

    Block_t block_test = new_block(level, predecessor, timestamp, operations_hash, context_hash, signature);

    print_block(block_test);

    printf("\n=== Encoding :\n");
    char *data = encode_block(block_test);
    print_hex(data, BLOCK_CODE_SIZE, NULL);

    // Start the client
    // return start_client(address, port);
}
