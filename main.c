
/* 207934902 Dor Darmon */
#include <stdio.h>
#include <stdlib.h>
#include "cache_utils.h"

// This is the main entry point of the program. It simulates memory reads using a cache structure.
int main()
{
    int n;

    // Ask the user for the size of the memory (RAM) to simulate
    printf("Size of data: ");
    scanf("%d", &n);

    // Allocate memory and read its content from user input
    uchar *mem = malloc(n);
    printf("Input data >> ");
    for (int i = 0; i < n; i++)
        scanf("%hhd", mem + i);

    // Read the cache configuration parameters: s, t, b, and E
    int s, t, b, E;
    printf("s t b E: ");
    scanf("%d %d %d %d", &s, &t, &b, &E);

    // Initialize the cache structure with the given parameters
    cache_t cache = initialize_cache(s, t, b, E);
    // Read memory offsets from input and simulate reading them through the cache
    while (1)
    {
        scanf("%d", &n);
        if (n < 0)
            break;
        read_byte(cache, mem, n);
    }

    // Print the final state of the cache
    puts("");
    print_cache(cache);

    // Free the simulated memory (RAM)
    free(mem);
}