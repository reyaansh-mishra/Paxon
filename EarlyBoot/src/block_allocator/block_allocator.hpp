/*
 * src/block_allocator/block_allocator.h
 * By Reyaansh Mishra
 */

#include <stdint.h>

class BlockAllocator {
        public:
                void*   malloc(size_t pages);
                void    init(uintptr_t base, size_t pages);

                size_t  get_max_pages();

        private:
                uintptr_t       base;
                size_t          no_of_pages;

                uintptr_t       next;
};

typedef size_t page_id;

struct PAGE {
        page_id  id;
        bool     is_allocated; 
};

extern BlockAllocator allocator;
