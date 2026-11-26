/*
 *  src/mmu/translation_tables/translation_tables.cpp
 * Paxon
 * By Reyaansh Mishra on 23/11/26
 */

#include "descriptors/descriptors.hpp"

extern "C" {
        #include <TTs.h>
        #include "translation_tables.h"
        #include <utils.h>
};


bool map_once(uintptr_t phy, uintptr_t virt, size_t step, struct PAGE_DESCRIPTOR_INFO page_info) {
        if (step == 1 GiB) {
                page_info.level = LEVEL_L1;
                return map_pages_1G(phy, virt, page_info);
        } else if (step == 2 MiB) {
                page_info.level = LEVEL_L2;
                return map_pages_2M(phy, virt, page_info);
        } else if (step == 4 KiB) {
                page_info.level = LEVEL_L3;
                return map_pages_4K(phy, virt, page_info);
        }
        
        else return false;
};

bool map_pages(uintptr_t phy, uintptr_t virt, size_t size /* PAGE SIZED ALIGNED */, enum PAGE_EXTRA_INFO page_info) {
        if ((phy | virt | size) & (PAGE_SIZE - 1))
                return false;           // enforce what the comment promised
       
        enum ATTR_INDEX attr = ATTR_NORMAL_WB;
        enum AP_FLAGS   ap   = AP_RO_EL1;
        bool            exec = false;
        
        if (page_info & PAGE_DEVICE_MMIO) {
                attr = ATTR_DEVICE_nGnRE;
        };
        if (page_info & PAGE_EL1_RW) {
                ap = AP_RW_EL1;
        };
        if (page_info & PAGE_EXEC) {
                exec = true;
        };
        
        struct PAGE_DESCRIPTOR_INFO info = {
                LEVEL_L0,
                attr,
                ap,
                SHARE_INNER,
                exec
        };

        while (size) {
                size_t step;

                if (!((phy | virt) & (1 GiB - 1)) && size >= 1 GiB)
                        step = 1 GiB;   // L1 block
                else if (!((phy | virt) & (2 MiB - 1)) && size >= 2 MiB)
                        step = 2 MiB;   // L2 block
                else
                        step = PAGE_SIZE; // L3 page

                if (!map_once(phy, virt, step, info))
                        return false;

                phy += step; virt += step; size -= step;
        };
        return true;
};
