#include "cache_utils.h"

void write_byte(cache_t cache, uchar* start, long int off, uchar new) {
    int B = 1 << cache.b;
    int S = 1 << cache.s;

    int block_offset = off & ((1 << cache.b) - 1);
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    long int tag = off >> (cache.s + cache.b);

    start[off] = new;

    for (int i = 0; i < cache.E; i++) {
        cache_line_t* line = &cache.cache[set_index][i];
        if (line->valid && line->tag == tag) {
            line->block[block_offset] = new;
            line->frequency++;
            return;
        }
    }

    read_byte(cache, start, off);
    write_byte(cache, start, off, new);
}
