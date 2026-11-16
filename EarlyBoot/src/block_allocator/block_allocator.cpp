/*
 * src/block_allocator/block_allocator.cpp
 * By Reyaansh Mishra
 */

extern "C" {
        #include <uefi.h>
        #include <memory.h>
}
#include "block_allocator.hpp"

BlockAllocator allocator;

size_t BlockAllocator::get_max_pages() {
        return BlockAllocator::no_of_pages;
};

void BlockAllocator::init(uintptr_t base, size_t pages) {
        BlockAllocator::base            = base;
        BlockAllocator::no_of_pages     = pages;
        next                            = BlockAllocator::base;
};

void* BlockAllocator::malloc(size_t pages) {
        if (next + (pages*PAGE_SIZE) > (base + no_of_pages*PAGE_SIZE)) {
                return NULL; // Bail Out Early
        };

        printf("I Am Allocating %d Pages at 0x%p\n", pages, next);

        void* to_ret = (void*)next;
        next += (pages*PAGE_SIZE);

        return to_ret;
};

void* cmalloc(size_t pages) {
        return allocator.malloc(pages);
};

extern "C" void enable_cmalloc(uintptr_t base, size_t number_pages) {
        allocator.init(base, number_pages);
};
