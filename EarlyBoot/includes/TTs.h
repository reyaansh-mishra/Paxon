/*
 * translation_tables.h
 * Paxon
 * By Reyaansh Mishra on 23/11/26
 */

#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define GiB *1073741824ULL
#define MiB *1048576ULL
#define KiB *1024ULL

enum PAGE_EXTRA_INFO {
        PAGE_DEVICE_MMIO        = 1 << 0,
        
        PAGE_EL1_RW             = 1 << 1,
        PAGE_EXEC               = 1 << 2,
};

bool map_pages(uintptr_t phy, uintptr_t virt, size_t size, enum PAGE_EXTRA_INFO page_info);
