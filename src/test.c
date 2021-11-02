#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include "test.h"
#include "account.h"
#include "block.h"
#include "utils.h"
#include "state.h"


void test_read_hex_string(){
    char *test = read_hex_string("3412");
    assert(*((int *) test) == 4660);
    printf("===> OK\n");
}


void test_account() {
    // Random account example
    char *user_public_key = read_hex_string("aca76354de343ef09385e263fb59561855d3cbf167961c6955624d91aa7eecf5");
    unsigned int level_pez = 1;
    unsigned int timestamp_pez = 2;
    unsigned int operations_hash_pez = 3;
    unsigned int context_hash_pez = 4;
    unsigned int signature_pez = 5;
   
    // Build the account
    Account_t account = new_account(user_public_key, level_pez, timestamp_pez, operations_hash_pez, context_hash_pez, signature_pez);

    printf("Account before encoding:\n");
    print_account(account);

    // Encode the account
    char *encoded = encode_account(account);

    // Compare to the expected encoding
    char *encoded_expect = read_hex_string("aca76354de343ef09385e263fb59561855d3cbf167961c6955624d91aa7eecf50000000100000002000000030000000400000005"); 
    assert(compare_data(encoded, ACCOUNT_CODE_SIZE, encoded_expect, ACCOUNT_CODE_SIZE));

    // Decode the account
    Account_t decoded = decode_account(encoded);

    printf("\nAccount after encoding & decoding:\n");
    print_account(decoded);

    // Compare to the expected decoded account (initial account)
    assert(compare_data(user_public_key, KEY_SIZE, decoded->user_public_key, KEY_SIZE));
    assert(level_pez == decoded->level_pez);
    assert(timestamp_pez == decoded->timestamp_pez);
    assert(operations_hash_pez == decoded->operations_hash_pez);
    assert(context_hash_pez == decoded->context_hash_pez);
    assert(signature_pez == decoded->signature_pez);

    // Free the memory
    delete_account(account);
    delete_account(decoded);
    free(encoded);
    free(encoded_expect);
    
    printf("\n===> OK\n");
}


void test_block() {
    // Block example from the subject
    unsigned int level = 44;
    char *predecessor = read_hex_string("1c80203a30e5de4d980cc555131d1b4a4750edc82c0c443179d88de1ae4f6cdf");
    unsigned long timestamp = 0x00000000616304e5;
    char *operations_hash = read_hex_string("0000000000000000000000000000000000000000000000000000000000000000");
    char *context_hash = read_hex_string("22a00d1c8c0fbaefedd71ddb83d455033efd259a8f0adf189b9f850a0d1945f2");
    char *signature = read_hex_string("cc3faffc696c86db13d50752fdb7edd0ee1ce19ab350f60899939fc139d58996419c13b812b7f005fafaf23924d2f1df555036bc61e7b67cb679375e5756b306");

    // Build the block
    Block_t block = new_block(level, predecessor, timestamp, operations_hash, context_hash, signature);

    // Print the block
    printf("Block before encoding:\n");
    print_block(block);

    // Encode the block
    char *encoded = encode_block(block);
    
    // Compare to the expected encoding
    char *encoded_expect = read_hex_string("0000002c1c80203a30e5de4d980cc555131d1b4a4750edc82c0c443179d88de1ae4f6cdf00000000616304e5000000000000000000000000000000000000000000000000000000000000000022a00d1c8c0fbaefedd71ddb83d455033efd259a8f0adf189b9f850a0d1945f2cc3faffc696c86db13d50752fdb7edd0ee1ce19ab350f60899939fc139d58996419c13b812b7f005fafaf23924d2f1df555036bc61e7b67cb679375e5756b306"); 
    assert(compare_data(encoded, BLOCK_CODE_SIZE, encoded_expect, BLOCK_CODE_SIZE));
    
    // Decode the block
    Block_t decoded = decode_block(encoded);

    printf("\nBlock after encoding & decoding:\n");
    print_block(decoded);

    // Compare to the expected decoded block (initial block)
    assert(level == decoded->level);
    assert(compare_data(predecessor, HASH_SIZE, decoded->predecessor, HASH_SIZE));
    assert(timestamp == decoded->timestamp);
    assert(compare_data(operations_hash, HASH_SIZE, decoded->operations_hash, HASH_SIZE));
    assert(compare_data(context_hash, HASH_SIZE, decoded->context_hash, HASH_SIZE));
    assert(compare_data(signature, SIG_SIZE, decoded->signature, SIG_SIZE));

    // Free the memory
    delete_block(block);
    delete_block(decoded);
    free(encoded);
    free(encoded_expect);

    printf("\n===> OK\n");
}

void test_message() {
    
    printf("===> OK\n");
}

void test_operation() {

    printf("===> OK\n");
}

void test_state() {
    // Making a dummy state
    Account_t account_1 = new_account(read_hex_string("46f097c96542dcd604c7436230daaa4603c39b4ecadd8be8e019d2ce51596f5f"), 10, 20, 30, 40, 50);
    Account_t account_2 = new_account(read_hex_string("e176367e487a576febd6fd9f53494191b55bb7b5f070d05200d6483aa817078f"), 1, 2, 3, 4, 5);
    Accounts_t tmp = new_accounts(account_2, NULL);
    Accounts_t accounts = new_accounts(account_1, tmp);

    // Build the state
    State_t state = new_state(read_hex_string("55900480c288c492b27be1b7a4fbbdd0317a0fd6c96087683c6477bdc08cd93c"), 1024, ACCOUNT_CODE_SIZE * 2, accounts);

    // Print the state
    printf("State before encoding :\n");
    print_state(state);

    // Encoding the state
    char *encoded = encode_state(state);

    // Compare the expect and the real
    char *encoded_expect = read_hex_string("55900480c288c492b27be1b7a4fbbdd0317a0fd6c96087683c6477bdc08cd93c000000000000040000000068e176367e487a576febd6fd9f53494191b55bb7b5f070d05200d6483aa817078f000000010000000200000003000000040000000546f097c96542dcd604c7436230daaa4603c39b4ecadd8be8e019d2ce51596f5f0000000a000000140000001e0000002800000032");
    assert(compare_data(encoded, STATE_CODE_SIZE_MIN + ACCOUNT_CODE_SIZE * 2, encoded_expect, STATE_CODE_SIZE_MIN + ACCOUNT_CODE_SIZE * 2));

    // Deconding the encoded state
    State_t decoded_state = decode_state(encoded);

    // Print the decoded
    printf("State after encoding & decoding :\n");
    print_state(decoded_state);

    // Verifying data is ok
    assert(compare_data(state->dictator_public_key, KEY_SIZE, decoded_state->dictator_public_key, KEY_SIZE));
    assert(state->predecessor_timestamp == decoded_state->predecessor_timestamp);
    assert(state->nb_account_bytes == decoded_state->nb_account_bytes);
    assert(compare_data(state->accounts->head->user_public_key, KEY_SIZE, decoded_state->accounts->head->user_public_key, KEY_SIZE));
    assert(compare_data(state->accounts->tail->head->user_public_key, KEY_SIZE, decoded_state->accounts->tail->head->user_public_key, KEY_SIZE));

    // Free the memory
    delete_state(state);
    delete_state(decoded_state);
    free(encoded);
    free(encoded_expect);

    printf("===> OK\n");
}

void run_tests() {
    printf("===============================================\n");
    printf("==================== TESTS ====================\n");
    printf("===============================================\n\n");

    printf("\n========== Test Read Hex String ==========\n\n");
    test_read_hex_string();

    printf("\n========== Test Account ==========\n\n");
    test_account();
    
    printf("\n========== Test Block ==========\n\n");
    test_block();
    
    printf("\n========== Test Message ==========\n\n");
    test_message();
    
    printf("\n========== Test Operation ==========\n\n");
    test_operation();
    
    printf("\n========== Test State ==========\n\n");
    test_state();

    printf("\n===============================================\n");
}