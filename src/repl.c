#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "repl.h"
#include "client.h"
#include "utils.h"
#include "state.h"
#include "account.h"
#include "block.h"

// --- Global vars

char char_buff[1024];
int running = 1;
pthread_t miner_thread;


// --- Internal functions

int parse_buffer(char **);
void evaluate();

void get_head_repl();
void get_account_repl();
void get_next();
void get_block_repl(unsigned int level);
void get_state_repl(unsigned int level, unsigned char full);
void display_help();
void stop_miner();


// --- REPL Starting function

void start_repl(pthread_t miner) {
    // Set the miner thread
    miner_thread = miner;

    // While running, read the user input
    while (running) {
        // Set the working vars
        unsigned char end = 0;
        unsigned short cursor = 0;
        char ch;

        // Display the input symbol
        printf("> ");
        fflush(stdout);

        // Read the input char by char
        while(!end) {
            if(read(STDIN_FILENO, &ch, 1) == -1) {
                printf("ERROR : Cannot read the user input\n");
                exit(1);
            }
            if(ch == '\n' || ch =='\r') {
                char_buff[cursor] = '\0';
                end = 1;
                evaluate();
            } else {
                char_buff[cursor] = ch;
                cursor++;
            }
        }
    }
}


// --- Internal function

int parse_buffer(char **result) {
    // Prepare the working vars
    unsigned char nb_args = 0;

    // Get the arguments
    unsigned short cursor = 0;
    unsigned short start = 0;
    char ch = char_buff[cursor];
    while (ch != '\0') {
        // If the ch is a space
        if(ch == ' ') {
            unsigned short new_arg_size = cursor - start + 1;

            char *new_arg = (char *) malloc(new_arg_size);
            memcpy(new_arg, char_buff + start, new_arg_size - 1);
            new_arg[new_arg_size - 1] = '\0';
            result[nb_args++] = new_arg;

            start = cursor + 1;
        }

        // Forward the cursor
        cursor++;
        ch = char_buff[cursor];
    }

    // Get the last arg
    if(start != cursor) {
        unsigned short new_arg_size = cursor - start + 1;
        char *new_arg = (char *) malloc(new_arg_size);
        memcpy(new_arg, char_buff + start, new_arg_size - 1);
        new_arg[new_arg_size - 1] = '\0';
        result[nb_args++] = new_arg;
    }

    // Return the result
    return nb_args;    
}

void evaluate() {
    // Parse the args
    char *args[64];
    int nb_args = parse_buffer(args);

    // Evaluate the args
    if(nb_args > 0) {
        // Help command
        if(strcmp(args[0], "help") == 0) {
            display_help();
        }
        
        // Stop command
        else if(strcmp(args[0], "stop") == 0) {
            stop_miner();
        }

        // Head command
        else if(strcmp(args[0], "head") == 0) {
            get_head_repl();
        }

        // Account command
        else if(strcmp(args[0], "account") == 0) {
            get_account_repl();
        }

        // Account command
        else if(strcmp(args[0], "next") == 0) {
            get_next();
        }
        
        // The block command
        else if(strcmp(args[0], "block") == 0) {
            if(nb_args == 2) {
                get_block_repl((unsigned int) atoi(args[1]));
            } else {
                printf("Syntaxe : block <level>\n");
            }
        }

        // The state command
        else if(strcmp(args[0], "state") == 0) {
            if(nb_args == 2) {
                get_state_repl((unsigned int) atoi(args[1]), 0);
            } else {
                printf("Syntaxe : state <level>\n");
            }
        }

        // The full state command
        else if(strcmp(args[0], "state-full") == 0) {
            if(nb_args == 2) {
                get_state_repl((unsigned int) atoi(args[1]), 1);
            } else {
                printf("Syntaxe : state-full <level>\n");
            }
        }
        
        // Unknown command
        else {
            printf("Unknown command...\n");
        }
    } else {
        printf("Input a command (\"help\" to see the help message)\n");
    }

    // Free the memory
    for(int i = 0 ; i < nb_args ; i++) {
        free(args[i]);
    }
}


void get_head_repl() {
    printf("Current chain head :\n");
    Block_t head = get_head_client();
    print_block(head);
}

void get_account_repl() {
    printf("Current account state :\n");
    State_t cur_state = get_state_head_client();

    char my_key[KEY_SIZE] = PUBLIC_KEY;
    Accounts_t ptr = cur_state->accounts;
    while(ptr != NULL) {
        if(compare_data(ptr->head->user_public_key, KEY_SIZE, my_key, KEY_SIZE)) {
            print_account(ptr->head);
            ptr = NULL;
        } else {
            ptr = ptr->tail;
        }
    }
}

void get_next() {
    time_t cur_time = time(NULL);
    int next_time = BLOCK_TIME - (cur_time - get_head_client()->timestamp);
    if(next_time > 0 && next_time < BLOCK_TIME) {
        printf("Next block in %d sec\n", next_time);
    } else {
        printf("Unpredictable next block time...\n");
    }
}

void get_block_repl(unsigned int level) {
    printf("Block %u :\n", level);
    Block_t block = get_block_client(level);
    print_block(block);
    delete_block(block);
}

void get_state_repl(unsigned int level, unsigned char full) {
    printf("State %u :\n", level);
    State_t state = get_state_client(level);
    if(full) print_state(state);
    else print_state_l(state);
    delete_state(state);
}

void display_help() {
    printf("REPL commands :\n");
    printf("\t- head : Get the current blockchain head\n");
    printf("\t- account : Get your account\n");
    printf("\t- next : Get the time before the next block\n");
    printf("\t- block <level> : Get the wanted block\n");
    printf("\t- state <level> : Get the wanted state\n");
    printf("\t- state-full <level> : Get the wanted state (full display)\n");
    printf("\t- help : Display this help message\n");
    printf("\t- stop : Stop the miner and the REPL\n");
}

void stop_miner() {
    printf("Stoping the miner and REPL...\n");
    pthread_cancel(miner_thread);
    running = 0;
    printf("Goodbye !\n");
}