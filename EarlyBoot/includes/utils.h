/*
 * utils.h
 * Paxon
 * By Reyaansh Mishra on 19/11/26
 */

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static inline uint64_t round_up(uint64_t num, uint64_t to_multiple_of)
{
    if (to_multiple_of == 0) return num;
    return (((num + (to_multiple_of-1))/to_multiple_of) * to_multiple_of);
};

static inline uint64_t round_down(uint64_t num, uint64_t to_multiple_of)
{
    if (to_multiple_of == 0) return num;
    return (num / to_multiple_of) * to_multiple_of;
};

static inline bool get_bit(uint64_t source, uint64_t bit) {
    return ((source >> bit) & 1ULL);
};

static inline uint64_t get_bits(uint64_t src, size_t start_bit, size_t end_bit) {  // start_bit -> end_bit
    return (src >> end_bit) & ((1ULL << (start_bit - end_bit + 1)) - 1);
};

static inline void set_bit(uint64_t* target, uint64_t bit, bool val) {
    if (!target) { return; };

    if (val)
        *target |= (1ULL << bit);
    else
        *target &= ~(1ULL << bit);
};

static inline void set_bits(uint64_t* target, size_t hi, size_t lo, uint64_t src) {
    size_t size = hi - lo + 1;
    
    for (size_t i = 0; i < size; i++) {
        set_bit(target, lo + i, get_bit(src, i));
    };
};
