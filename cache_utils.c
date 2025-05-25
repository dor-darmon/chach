
/* 207934902 Dor Darmon */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache_utils.h"

// Initializes the cache with the given parameters
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E)
{
    cache_t cache;
    cache.s = s;
    cache.t = t;
    cache.b = b;
    cache.E = E;
    int S = 1 << s;
    int B = 1 << b;
    cache.cache = (cache_line_t **)malloc(S * sizeof(cache_line_t *));
    for (int i = 0; i < S; i++)
    {
        cache.cache[i] = (cache_line_t *)malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++)
        {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0;
            cache.cache[i][j].block = (uchar *)calloc(B, sizeof(uchar));
        }
    }
    return cache;
}

// Simulates reading a byte from memory using the cacheuchar
read_byte(cache_t cache, uchar *start, long int off)
{
    int B = 1 << cache.b;
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    long int tag = off >> (cache.s + cache.b);
    int block_offset = off & ((1 << cache.b) - 1);
    int hit_index = -1;
    for (int i = 0; i < cache.E; i++)
    {
        cache_line_t *line = &cache.cache[set_index][i];
        if (line->valid && line->tag == tag)
        {
            hit_index = i;
            break;
        }
    }
    if (hit_index != -1)
    {
        cache.cache[set_index][hit_index].frequency++;
        return cache.cache[set_index][hit_index].block[block_offset];
    }
    int insert_index = -1;
    for (int i = 0; i < cache.E; i++)
    {
        if (!cache.cache[set_index][i].valid)
        {
            insert_index = i;
            break;
        }
    }
    if (insert_index == -1)
    {
        int min_freq = cache.cache[set_index][0].frequency;
        insert_index = 0;
        for (int i = 1; i < cache.E; i++)
        {
            if (cache.cache[set_index][i].frequency < min_freq)
            {
                min_freq = cache.cache[set_index][i].frequency;
                insert_index = i;
            }
        }
    }
    cache_line_t *line = &cache.cache[set_index][insert_index];
    line->valid = 1;
    line->frequency = 1;
    line->tag = tag;
    long int block_base_addr = (off >> cache.b) << cache.b;
    for (int i = 0; i < B; i++)
    {
        line->block[i] = start[block_base_addr + i];
    }
    return line->block[block_offset];
} 

// Simulates writing a byte to memory and the cache (write-through policy)
void write_byte(cache_t cache, uchar *start, long int off, uchar new)
{
    int block_offset = off & ((1 << cache.b) - 1);
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    long int tag = off >> (cache.s + cache.b);
    start[off] = new;
    for (int i = 0; i < cache.E; i++)
    {
        cache_line_t *line = &cache.cache[set_index][i];
        if (line->valid && line->tag == tag)
        {
            line->block[block_offset] = new;
            line->frequency++;
            return;
        }
    }
    read_byte(cache, start, off);
    write_byte(cache, start, off, new);
}

// Prints the current state of the cache in the required formatvoid
print_cache(cache_t cache)
{
    int S = 1 << cache.s;

    int B = 1 << cache.b;
    for (int i = 0; i < S; i++)
    {
        printf("Set %d\n", i);
        for (int j = 0; j < cache.E; j++)
        {
            printf("%1d %d 0x%0*lx ",
                   cache.cache[i][j].valid,
                   cache.cache[i][j].frequency,
                   cache.t,
                   cache.cache[i][j].tag);
            for (int k = 0; k < B; k++)
            {
                printf("%02x ", cache.cache[i][j].block[k]);
            }
            puts("");
        }
    }
}