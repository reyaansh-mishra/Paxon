//
//  descriptors.hpp
//  EarlyBoot
//
//  Created by Reyaansh Mishra on 19/11/26.
//

#pragma once

extern "C" {
        #include <stdint.h>
        #include <memory.h>
        #include "descriptor_formats.h"
}


class GenericDescriptor {
    public:
        void init();
        void reset();
        bool is_active() const;
        bool is_table() const;

        bool is_block() const;

    protected:
        uint64_t raw = 0;
};

class TABLE_DESCRIPTOR : public GenericDescriptor {
    public:
        bool        activate(uintptr_t ptr_to_next_table);        // One-Shot
        uintptr_t   get_next_table();
    private:
        void        set_next_table(uintptr_t ptr_to_next_table);
};

class PAGE_DESCRIPTOR : public GenericDescriptor {
    public:
        bool            activate(struct PAGE_DESCRIPTOR_INFO table, uintptr_t phy_addr);        // One-Shot
        void            disable_exec();
        void            set_mair();
        uintptr_t       get_output_addr();

    private:
        bool        set_out(uintptr_t phy_addr, int level);
};

extern TABLE_DESCRIPTOR __attribute__((aligned(PAGE_SIZE))) L0_table[512];
