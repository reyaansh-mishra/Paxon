/*
 * uefi_mem.c
 * Paxon
 * By Reyaansh Mishra on 26/11/26
 */

#include <uefi.h>

static efi_memory_descriptor_t* mem_map;

efi_status_t refresh_mem_map() {
        efi_status_t    status;
        uintn_t         MemMapSize = 0;
        uintn_t         MapKey;
        uintn_t         DescriptorSize;
        uint32_t        DescriptorVersion;
        
        efi_memory_descriptor_t* MemoryMap = NULL;
        
        status = BS->GetMemoryMap(
                        &MemMapSize,
                        MemoryMap,
                        &MapKey,
                        &DescriptorSize,
                        &DescriptorVersion
                );
        
        if (status != EFI_BUFFER_TOO_SMALL) return status;
        
        // Allocate with MORE memory than needed this time. This prevents accidental EFI_BUFFER_TOO_SMALL
        MemMapSize += 2 * DescriptorSize;
        
        status = BS->AllocatePool(EfiLoaderData, MemMapSize, (void **)&MemoryMap);
        if (EFI_ERROR(status)) {
            return status;
        }
        
        status = BS->GetMemoryMap(
                        &MemMapSize,
                        MemoryMap,
                        &MapKey,
                        &DescriptorSize,
                        &DescriptorVersion
                );
        
        if (EFI_ERROR(status)) return status;
        
        mem_map = MemoryMap;
        return status;
};

efi_memory_descriptor_t* get_mem_map() { return mem_map; };
