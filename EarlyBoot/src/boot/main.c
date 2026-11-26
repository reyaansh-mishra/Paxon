/*
 * src/boot/main.c
 * By Reyaansh Mishra
 */

#include <uefi.h>
#include <memory.h>
#include <TTs.h>
#include <utils.h>

void main(void) {
        uintptr_t       image_base = round_down((uintptr_t)LIP->ImageBase, PAGE_SIZE);
        size_t          image_size = round_up(LIP->ImageSize, PAGE_SIZE);
        
        printf("BASE: 0x%p\nSIZE: %d\n", image_base, image_size);
        
        // Step 1. Allocate a bunch of RAM
        uintptr_t malloc_ram_base = alloc_pages(MAX_BLOCK_ALLOCATOR_PAGES);
        
        
        // Step 2. Enable cmalloc, dont use yet
        enable_cmalloc(malloc_ram_base, MAX_BLOCK_ALLOCATOR_PAGES);
        
        // Step 3: Get Memory Map
        if (EFI_ERROR(refresh_mem_map()))
                printf("Mem Map Init Failed!\n");
        else
                printf("Mem Map Inited!\n");
        
        // Step 4: MMU BS
        printf("\nMAPPING EarlyBoot NOW!\n");
        printf("%d", map_pages(image_base, image_base, image_size, (enum PAGE_EXTRA_INFO)(PAGE_EXEC)));
        printf("\nMAPPING BLOCK ALLOCATOR NOW!\n");
        printf("%d", map_pages(malloc_ram_base, malloc_ram_base, MAX_BLOCK_ALLOCATOR_PAGES*PAGE_SIZE, (enum PAGE_EXTRA_INFO)(PAGE_EL1_RW)));
        printf("\nMAPPING UART...\n");
        printf("%d", map_pages(0x09000000, 0x09000000, PAGE_SIZE, PAGE_EL1_RW));

        // Last Step: Wait till eternity
        while (true)
                asm __volatile__ ("yield");
};
