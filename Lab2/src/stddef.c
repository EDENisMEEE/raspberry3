#include "stddef.h"

// minimal string/memory functions

#include "stdint.h"  // 用到 uint8_t

// minimal memcmp
int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) return (int)(p1[i] - p2[i]);
    }
    return 0;
}
