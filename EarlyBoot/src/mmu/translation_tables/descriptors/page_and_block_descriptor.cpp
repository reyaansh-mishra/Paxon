/*
 * src/mmu/translation_tables/descriptors/page_and_block_descriptor.cpp
 * By Reyaansh Mishra
 */

#include "descriptors.hpp"

extern "C" {
        #include <utils.h>
        #include "descriptor_formats.h"
};

// This file is BOTH for Page and Block Descriptors

/* Stage 1 Block/Page descriptor, EL1&0, 4K granule:
 * [63]:    IGNORED (NSTable is table-descriptor only)
 * [62:59]: IGNORED / PBHA
 * [58:55]: Reserved for software use
 * [54]:    UXN (unprivileged execute-never)
 * [53]:    PXN (privileged execute-never)
 * [52]:    Contiguous
 * [51]:    DBM (Dirty Bit Modifier)
 * [50]:    GP (Guarded Page, FEAT_BTI; else RES0)
 * [49:48]: RES0 (OA[49:48] with LPA2)
 * [47:12]: Output address (block: aligned to level size)
 * [11]:    nG (Not Global)
 * [10]:    AF (Access Flag)
 * [9:8]:   SH (Shareability)
 * [7:6]:   AP[2:1]  00 = RW EL1 only, 01 = RW EL1+EL0,
 *                   10 = RO EL1 only, 11 = RO EL1+EL0
 * [5]:     NS (RES0 if non-secure only)
 * [4:2]:   AttrIndx[2:0]
 * [1]:     L0-L2: 0 = Block, 1 = Table (not valid here)
 *          L3:    1 = Page
 * [0]:     Valid
 */

uintptr_t PAGE_DESCRIPTOR::get_output_addr() {
        return get_bits(raw, 47, 12) << 12;
};

static constexpr struct PAGE_DESCRIPTOR_BITS drop_in_table(struct PAGE_DESCRIPTOR_INFO info) {     // NOTE: This is for PAGE (L3)
        struct PAGE_DESCRIPTOR_BITS descriptor = {
                true,
                TYPE_PAGE,      // Overwritten later
                info.attr_index,
                false,
                info.access,
                info.shareability,
                true,
                false,
                0,
                0,
                false,
                false,
                false,
                false,
                true,
                0,
                0,
                0
        };
        
        if (info.level == 0 || info.level == 1 || info.level == 2) {
                descriptor.type = TYPE_BLOCK;
        };
        
        return descriptor;
};

bool PAGE_DESCRIPTOR::set_out(uintptr_t phy_addr, int level) {
        uintptr_t align;
        switch (level) {
                case 1:  align = 1 GiB; break;
                case 2:  align = 2 MiB; break;
                case 3:  align = 4 KiB; break;
                default: return false;      // L0 can't map blocks at 4 KiB granule
        }
        
        if ((phy_addr & (align - 1)) || (phy_addr >> 48))
                return false;
        
        set_bits(&raw, 47, 12, phy_addr >> 12);
        return true;
}

bool PAGE_DESCRIPTOR::activate(struct PAGE_DESCRIPTOR_INFO table, uintptr_t phy_addr) {
        struct PAGE_DESCRIPTOR_BITS descriptor = drop_in_table(table);
        raw = __builtin_bit_cast(uint64_t, descriptor);
        
        if (table.is_exec) {
                set_bit(&raw, 53, 0);
        } else {
                set_bit(&raw, 53, 1);
        };

        return set_out(phy_addr, (int)table.level);
};
