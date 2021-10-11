#include <stdio.h>
#include <stdlib.h>

#include "main.h"

int main(int argc, char const *argv[])
{
    unsigned char *hash_test = hash("nakamoto", "satoshi");
    printf("%d\n", mine(hash_test, 15));
    free(hash_test);
    return 0;
}
