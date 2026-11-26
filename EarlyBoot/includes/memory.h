/*
 * includes/memory.h
 * By Reyaansh Mishra
 */

#pragma once

#include <uefi.h>

#define MAX_BLOCK_ALLOCATOR_PAGES       4000
#define PAGE_SIZE                       4096

uintptr_t       alloc_pages(size_t pages);

void            enable_cmalloc(uintptr_t base, size_t number_pages);
void*           cmalloc(size_t pages);          // custom malloc

efi_memory_descriptor_t* get_mem_map();
efi_status_t refresh_mem_map();
