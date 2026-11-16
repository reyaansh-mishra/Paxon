/*
 * src/memory/memory.c
 * By Reyaansh Mishra
 */

#include <memory.h>

uintptr_t alloc_pages(size_t pages) {
        efi_physical_address_t addr = 0;

        if (BS->AllocatePages(AllocateAnyPages, EfiLoaderData, pages, &addr) != EFI_SUCCESS)
                return 0;
        return (uintptr_t)addr;
};
