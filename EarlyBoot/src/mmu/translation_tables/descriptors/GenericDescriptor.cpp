/*
 * descriptors.cpp
 * Paxon
 * By Reyaansh Mishra on 19/11/26
 */

#include "descriptors.hpp"
extern "C" {
    #include "utils.h"
}

// This file is for the GENERIC Table Discriptor only.

void GenericDescriptor::init() {
    raw = 0;
};

void GenericDescriptor::reset() {
    raw = 0;
};

bool GenericDescriptor::is_active() const {
    return get_bit(raw, 0);
};

bool GenericDescriptor::is_block() const {
    return get_bit(raw, 1);
};

bool GenericDescriptor::is_table() const {
        return (raw & 0b11) == 0b11;
};
