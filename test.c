#include <stdlib.h>
#include <stdio.h>

static char bytes[10];

int main(){
    char* temp = &bytes[0];
    *(size_t*)temp = 500;

    printf("bytes[0] is %lu\n", (size_t)bytes[0]);
    // printf("bytes[0] is %c\n", bytes[0]);

    // size_t x = 10;

    // printf("size of x: %zu\n", sizeof(x));

    return EXIT_SUCCESS;
}