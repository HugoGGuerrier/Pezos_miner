#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "main.h"
#include "blake2.h"
#include "client.h"
#include "utils.h"
#include "block.h"

void test_read_hex_string(){
    print_hex(read_hex_string("0f"), 1, "\n");
}


void test_account() {

}


void test_block() {
    // Block example from the subject
    int level = 44;
    char *predecessor = read_hex_string("1c80203a30e5de4d980cc555131d1b4a4750edc82c0c443179d88de1ae4f6cdf");
    unsigned long timestamp = 0x00000000616304e5;
    char *operations_hash = read_hex_string("0000000000000000000000000000000000000000000000000000000000000000");
    char *context_hash = read_hex_string("22a00d1c8c0fbaefedd71ddb83d455033efd259a8f0adf189b9f850a0d1945f2");
    char *signature = read_hex_string("cc3faffc696c86db13d50752fdb7edd0ee1ce19ab350f60899939fc139d58996419c13b812b7f005fafaf23924d2f1df555036bc61e7b67cb679375e5756b306");

    // Build the block
    Block_t block = new_block(level, predecessor, timestamp, operations_hash, context_hash, signature);

    print_block(block);

    // Encode the block
    char *encoded = encode_block(block);
    print_hex(encoded, BLOCK_CODE_SIZE, "\n");
    
    // Compare to the expected encoding
    char *encoded_expect = read_hex_string("0000002c1c80203a30e5de4d980cc555131d1b4a4750edc82c0c443179d88de1ae4f6cdf00000000616304e5000000000000000000000000000000000000000000000000000000000000000022a00d1c8c0fbaefedd71ddb83d455033efd259a8f0adf189b9f850a0d1945f2cc3faffc696c86db13d50752fdb7edd0ee1ce19ab350f60899939fc139d58996419c13b812b7f005fafaf23924d2f1df555036bc61e7b67cb679375e5756b306"); 
    assert(compare_data(encoded, BLOCK_CODE_SIZE, encoded_expect, BLOCK_CODE_SIZE));

    // Decode the block
    Block_t decoded = decode_block(encoded);

    print_block(decoded);

    // Compare to the expected decoded block (initial block)
    assert(compare_data(level, sizeof(int), decoded->level, sizeof(int)));
    assert(compare_data(predecessor, HASH_SIZE, decoded->predecessor, HASH_SIZE));
    assert(compare_data(timestamp, sizeof(long), decoded->timestamp, sizeof(long)));
    assert(compare_data(operations_hash, HASH_SIZE, decoded->operations_hash, HASH_SIZE));
    assert(compare_data(context_hash, HASH_SIZE, decoded->context_hash, HASH_SIZE));
    assert(compare_data(signature, SIG_SIZE, decoded->signature, SIG_SIZE));
}

void test_message() {
    
}

void test_operation() {

}

void test_state() {
    
}

void tests() {
    printf("==================== TESTS ====================\n");
    printf("========== Test Read Hex String ==========\n");
    test_read_hex_string();

    printf("========== Test Account ==========\n");
    test_account();
    
    printf("OK\n========== Test Block ==========\n");
    test_block();
    
    printf("OK\n========== Test Message ==========\n");
    test_message();
    
    printf("OK\n========== Test Operation ==========\n");
    test_operation();
    
    printf("OK\n========== Test State ==========\n");
    test_state();
    
    printf("OK\n");
    
    printf("=====================================\n");
}

int main() {
    // Define application data
    const char *address = "78.194.168.67";
    unsigned int port = 1337;

    // Run the test suite
    tests();

    // Start the client
    return start_client(address, port);
}
