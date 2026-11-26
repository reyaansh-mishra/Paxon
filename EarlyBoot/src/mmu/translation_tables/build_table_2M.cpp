/*
 * build_table_2M.cpp
 * Paxon
 * By Reyaansh Mishra on 23/11/26
 */

#include "descriptors/descriptors.hpp"

extern "C" {
        #include <memory.h>
        #include "translation_tables.h"
        #include <utils.h>
};

static bool setup_l1(uintptr_t virt, uintptr_t next_table) {
        uintptr_t           l0_bits     = get_bits(virt, 47, 39);
        uintptr_t           l1_bits     = get_bits(virt, 38, 30);

        uintptr_t           l1_table    = (uintptr_t)cmalloc(1);

        TABLE_DESCRIPTOR*   L0          = &L0_table[l0_bits];
        TABLE_DESCRIPTOR*   L1          = NULL;

        if (!L0->is_active()) setup_l0(virt, l1_table);
        L1 = &((TABLE_DESCRIPTOR *)L0->get_next_table())[l1_bits];
        

        if (!L1->is_active()) {
                L1->init();
                L1->activate(next_table);
        } else {
                if (next_table == L1->get_next_table()) {   // OK
                        return true;
                } else {
                        return false;
                }
        };
        return true;
};

static bool setup_l2(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO perms) {
        printf("Setup L2 Block! phy=0x%p, virt=0x%p\n", phy, virt);
        uintptr_t           l0_bits     = get_bits(virt, 47, 39);
        uintptr_t           l1_bits     = get_bits(virt, 38, 30);
        uintptr_t           l2_bits     = get_bits(virt, 29, 21);

        uintptr_t           l2_table    = (uintptr_t)cmalloc(1);

        TABLE_DESCRIPTOR*   L0          = &L0_table[l0_bits];
        TABLE_DESCRIPTOR*   L1          = NULL;
        PAGE_DESCRIPTOR*    L2          = NULL;

        if (!L0->is_active()) setup_l1(virt, l2_table);
        L1 = &((TABLE_DESCRIPTOR *)L0->get_next_table())[l1_bits];
        
        if (L1->is_active() && !L1->is_table())
                return false;           // a 1 GiB block is already in the way
        
        if (!L1->is_active()) setup_l1(virt, l2_table);
        L2 = &((PAGE_DESCRIPTOR *)L1->get_next_table())[l2_bits];

        if (!L2->is_active()) {
                L2->init();
                L2->activate(perms, phy);
        } else {
                if (!(phy == L2->get_output_addr())) {
                        return false;
                }
        };
        return true;
};

extern "C" bool map_pages_2M(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO page_info) {       /* BLOCK hehe */
        return setup_l2(phy, virt, page_info);
};

