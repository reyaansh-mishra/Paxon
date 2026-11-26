/*
 * src/mmu/translation_tables/descriptors/TableDescriptor.cpp
 * By Reyaansh Mishra
 */

#include "descriptors.hpp"

extern "C" {
        #include <utils.h>
        #include "descriptor_formats.h"
};

/* Stage 1 Table descriptor, EL1&0, 4K granule (L0-L2 only):
 * [63]:    NSTable (only meaningful in Secure state, else ignored)
 * [62:61]: APTable[1:0]  hierarchical AP override for the whole subtree
 *            00 = no effect
 *            01 = no EL0 access
 *            10 = read-only (EL1 and EL0)
 *            11 = read-only, and no EL0 access
 * [60]:    UXNTable (1 = everything below is EL0 execute-never)
 * [59]:    PXNTable (1 = everything below is EL1 execute-never)
 * [58:51]: IGNORED (software use)
 * [50:48]: RES0 (48-bit PA; address bits with LPA2/52-bit)
 * [47:12]: Next-level table address (4K aligned)
 * [11:2]:  IGNORED
 * [1]:     1 (Table)
 * [0]:     1 (Valid)
 */

consteval struct TABLE_DESCRIPTOR_BITS drop_in_template_table() {
        struct TABLE_DESCRIPTOR_BITS descriptor = {
                true,
                true,
                0,
                0,
                0,
                0,
                false,
                true,
                ACCESS_TABLE_NO_EFFECT,
                0
        };
        return descriptor;
};

uintptr_t TABLE_DESCRIPTOR::get_next_table() {
        return get_bits(raw, 47, 12) << 12;
};

void TABLE_DESCRIPTOR::set_next_table(uintptr_t ptr_to_next_table) {
        if (ptr_to_next_table & 0xFFF)
                return;
        set_bits(&raw, 47, 12, ptr_to_next_table >> 12);
};

bool TABLE_DESCRIPTOR::activate(uintptr_t ptr_to_next_table) {
        if (ptr_to_next_table & 0xFFF)
                return false;
        struct TABLE_DESCRIPTOR_BITS descriptor = drop_in_template_table();
        raw = __builtin_bit_cast(uint64_t, descriptor);
                
        set_next_table(ptr_to_next_table);
        return true;
};

