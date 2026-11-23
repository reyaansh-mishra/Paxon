/*
 * src/boot/main.c
 * By Reyaansh Mishra
 */

#include <uefi.h>
#include <memory.h>
#include <TTs.h>

void main(void) {
        // Step 1. Allocate a bunch of RAM
        uintptr_t malloc_ram_base = alloc_pages(MAX_BLOCK_ALLOCATOR_PAGES);
        
        // Step 2. Enable cmalloc, dont use yet
        enable_cmalloc(malloc_ram_base, MAX_BLOCK_ALLOCATOR_PAGES);
        printf("Hi\n");
        printf("Recvd: %p\n\n\n", cmalloc(3));
        
        // Step 3: MMU BS
        printf("%d", map_range(0x40000000, 0x40000000, 1 GiB, (enum PAGE_EXTRA_INFO)(PAGE_EL1_RW)));

        // Last Step: Wait till eternity
        while (true)
                asm __volatile__ ("yield");
};

