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

    long s = 0x1234567890123456;
    char *t = (char *) &s;
    print_hex(t, 8, "\n");
    s = reverse_long(s);
    print_hex(t, 8, "\n");

    // Start the client
    // return start_client(address, port);
}
