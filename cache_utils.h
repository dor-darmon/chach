/*207934902 Dor Darmon */
#ifndef CACHE_UTILS_H
#define CACHE_UTILS_H

// Define a byte as an unsigned char
typedef unsigned char uchar;

// Struct representing a single line in the cache
typedef struct cache_line_s
{
    uchar valid;
    uchar frequency;
    long int tag;
    uchar *block;
} cache_line_t;

// Struct representing the entire cache
typedef struct cache_s
{
    uchar s;
    uchar t;

    uchar b;
    uchar E;
    cache_line_t **cache;
} cache_t;

cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E);
uchar read_byte(cache_t cache, uchar *start, long int off);
void write_byte(cache_t cache, uchar *start, long int off, uchar new);
void print_cache(cache_t cache);
#endif