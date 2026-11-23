/*
 * build_table_1G.cpp
 * Paxon
 * By Reyaansh Mishra on 23/11/26
 */

#include "descriptors/descriptors.hpp"

extern "C" {
        #include <memory.h>
        #include "translation_tables.h"
        #include <utils.h>
};

TABLE_DESCRIPTOR __attribute__((aligned(PAGE_SIZE))) L0_table[512];

bool setup_l0(uintptr_t virt, uintptr_t next_table) {
        uintptr_t           l0_bits = get_bits(virt, 47, 39);
        TABLE_DESCRIPTOR*   L0      = &L0_table[l0_bits];
        if (!L0->is_active()) {  // Setup L0 table
                L0->init();
                L0->activate(next_table);
        } else {
                if (next_table == L0->get_next_table()) {   // OK
                        return true;
                } else {
                        return false;
                }
        }
        return true;
};


static bool setup_l1(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO page_info) {
        printf("SETUP L1\n");
        uintptr_t           l0_bits     = get_bits(virt, 47, 39);
        uintptr_t           l1_bits     = get_bits(virt, 38, 30);

        uintptr_t           l1_table    = (uintptr_t)cmalloc(1);

        TABLE_DESCRIPTOR*   L0          = &L0_table[l0_bits];
        PAGE_DESCRIPTOR*    L1          = NULL;

        if (!L0->is_active()) {
                if (!setup_l0(virt, l1_table)) return false;
        }
        L1 = &((PAGE_DESCRIPTOR *)L0->get_next_table())[l1_bits];
    
        if (!L1->is_active()) {
                L1->init();
                L1->activate(page_info, phy);
        } else {
                if (phy == L1->get_output_addr()) {   // OK
                        return true;
                } else {
                        return false;
                }
        };
        return true;
};

extern "C" bool map_pages_1G(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO page_info) {       /* BLOCK hehe */
        return setup_l1(phy, virt, page_info);
};
