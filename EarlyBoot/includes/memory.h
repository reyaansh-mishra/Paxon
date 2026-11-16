/*
 * includes/memory.h
 * By Reyaansh Mishra
 */

#include <uefi.h>

#define MAX_BLOCK_ALLOCATOR_PAGES       4000
#define PAGE_SIZE                       4000

uintptr_t       alloc_pages(size_t pages);

void            enable_cmalloc(uintptr_t base, size_t number_pages);
void*           cmalloc(size_t pages);          // custom malloc

