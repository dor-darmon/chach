#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// טיפוס בסיסי
typedef unsigned char uchar;

// מבנה שורה בזיכרון מטמון
typedef struct cache_line_s {
    uchar valid;
    uchar frequency;
    long int tag;
    uchar* block;
} cache_line_t;

// מבנה הזיכרון מטמון (cache)
typedef struct cache_s {
    uchar s;
    uchar t;
    uchar b;
    uchar E;
    cache_line_t** cache;
} cache_t;

// פונקציית יצירת cache
cache_t initialize_cache(uchar s, uchar t, uchar b, uchar E) {
    cache_t cache;
    cache.s = s;
    cache.t = t;
    cache.b = b;
    cache.E = E;

    int S = 1 << s; // מספר הסטים
    int B = 1 << b; // גודל הבלוק

    // הקצאת מערך של מצביעים לסטים
    cache.cache = (cache_line_t**)malloc(S * sizeof(cache_line_t*));

    for (int i = 0; i < S; i++) {
        // לכל סט, הקצאת E שורות
        cache.cache[i] = (cache_line_t*)malloc(E * sizeof(cache_line_t));
        for (int j = 0; j < E; j++) {
            cache.cache[i][j].valid = 0;
            cache.cache[i][j].frequency = 0;
            cache.cache[i][j].tag = 0;
            // הקצאת block בגודל B בתים
            cache.cache[i][j].block = (uchar*)calloc(B, sizeof(uchar));
        }
    }

    return cache;
}
