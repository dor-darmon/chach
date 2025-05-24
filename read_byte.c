#include "cache_utils.h"

uchar read_byte(cache_t cache, uchar* start, long int off) {
    int B = 1 << cache.b;
    int S = 1 << cache.s;

    int block_offset = off & ((1 << cache.b) - 1);
    int set_index = (off >> cache.b) & ((1 << cache.s) - 1);
    long int tag = off >> (cache.s + cache.b);

    int hit_index = -1;
    for (int i = 0; i < cache.E; i++) {
        cache_line_t* line = &cache.cache[set_index][i];
        if (line->valid && line->tag == tag) {
            hit_index = i;
            break;
        }
    }

    if (hit_index != -1) {
        cache.cache[set_index][hit_index].frequency++;
        return cache.cache[set_index][hit_index].block[block_offset];
    }

    int insert_index = -1;
    for (int i = 0; i < cache.E; i++) {
        if (!cache.cache[set_index][i].valid) {
            insert_index = i;
            break;
        }
    }

    if (insert_index == -1) {
        int min_freq = cache.cache[set_index][0].frequency;
        insert_index = 0;
        for (int i = 1; i < cache.E; i++) {
            if (cache.cache[set_index][i].frequency < min_freq) {
                min_freq = cache.cache[set_index][i].frequency;
                insert_index = i;
            }
        }
    }

    cache_line_t* line = &cache.cache[set_index][insert_index];
    line->valid = 1;
    line->frequency = 1;
    line->tag = tag;

    long int block_base_addr = (off >> cache.b) << cache.b;
    for (int i = 0; i < B; i++) {
        line->block[i] = start[block_base_addr + i];
    }

    return line->block[block_offset];
}
