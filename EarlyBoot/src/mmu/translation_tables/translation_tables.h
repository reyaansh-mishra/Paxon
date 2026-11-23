/*
 * translation_tables.h
 * Paxon
 * By Reyaansh Mishra on 23/11/26
 */

#pragma once

#include <TTs.h>

bool map_pages_4K(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO page_info);
bool map_pages_2M(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO page_info);
bool map_pages_1G(uintptr_t phy, uintptr_t virt, struct PAGE_DESCRIPTOR_INFO page_info);

bool setup_l0(uintptr_t virt, uintptr_t next_table);
