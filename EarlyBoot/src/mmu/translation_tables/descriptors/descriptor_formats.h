/*
 * descriptor_formats.h
 * Paxon
 * By Reyaansh Mishra on 21/11/26
 *
 * My brain is fried right now. Expect bad names.
 */

#pragma once

#include <stdint.h>

#define GiB *1073741824ULL
#define MiB *1048576ULL
#define KiB *1024ULL

/* Stage 1 Table descriptor, EL1&0, 4K granule (L0-L2 only):
 * [63]:    NSTable (only meaningful in Secure state, else ignored)
 * [62:61]: APTable[1:0]  hierarchical AP override for the whole subtree
 *            00 = no effect
 *            01 = no EL0 access
 *            10 = read-only (EL1 and EL0)
 *            11 = read-only, and no EL0 access
 * [60]:    UXNTable (1 = everything below is EL0 execute-never)
 * [59]:    PXNTable (1 = everything below is EL1 execute-never)
 * [58:51]: IGNORED (software use)
 * [50:48]: RES0 (48-bit PA; address bits with LPA2/52-bit)
 * [47:12]: Next-level table address (4K aligned)
 * [11:2]:  IGNORED
 * [1]:     1 (Table)
 * [0]:     1 (Valid)
 */

typedef uint64_t flag_t;   // 1-bit flag; accepts true/false just fine

enum ACCESS_TABLE : uint64_t {
        ACCESS_TABLE_NO_EFFECT       = 0b00,
        ACCESS_TABLE_NO_EL0          = 0b01,
        ACCESS_TABLE_READ_ONLY       = 0b10,
        ACCESS_TABLE_RO_NO_EL0       = 0b11,
};

struct TABLE_DESCRIPTOR_BITS {
        flag_t                  is_valid        : 1;    // [0]          Set: true
        flag_t                  is_table        : 1;    // [1]          Set: true; // Side Note: Im VERY poud of this bool for no reason
        uint64_t                ignored0        : 10;   // [2:11]
        uint64_t                next_table      : 36;   // [12:47]
        uint64_t                res0            : 3;    // [48:50], RES0
        uint64_t                ignored1        : 8;    // [51:58]
        flag_t                  is_PXN          : 1;    // [59]         Set: false
        flag_t                  is_UXN          : 1;    // [60]         Set: true
        enum ACCESS_TABLE       access_table    : 2;    // [61:62]      Choose: ACCESS_TABLE_NO_EFFECT
        uint64_t                NSTable         : 1;    // [63]         Set: 0
};
static_assert(sizeof(struct TABLE_DESCRIPTOR_BITS) == 8, "");

/* Stage 1 Block/Page descriptor, EL1&0, 4K granule:
 * [63]:    IGNORED (NSTable is table-descriptor only)
 * [62:59]: IGNORED / PBHA
 * [58:55]: Reserved for software use
 * [54]:    UXN (unprivileged execute-never)
 * [53]:    PXN (privileged execute-never)
 * [52]:    Contiguous
 * [51]:    DBM (Dirty Bit Modifier)
 * [50]:    GP (Guarded Page, FEAT_BTI; else RES0)
 * [49:48]: RES0 (OA[49:48] with LPA2)
 * [47:12]: Output address (block: aligned to level size)
 * [11]:    nG (Not Global)
 * [10]:    AF (Access Flag)
 * [9:8]:   SH (Shareability)
 * [7:6]:   AP[2:1]  00 = RW EL1 only, 01 = RW EL1+EL0,
 *                   10 = RO EL1 only, 11 = RO EL1+EL0
 * [5]:     NS (RES0 if non-secure only)
 * [4:2]:   AttrIndx[2:0]
 * [1]:     L0-L2: 0 = Block, 1 = Table (ie dont choose 1 if you are at L0/1/2)
 *          L3:    1 = Page
 * [0]:     Valid
 */

/* Slots in MAIR_EL1. Descriptor bits [4:2] pick one of these.
 * Order is arbitrary, but MAIR_EL1 must be programmed to match. */
enum ATTR_INDEX : uint64_t {
        ATTR_DEVICE_nGnRnE      = 0,    /* MAIR byte 0x00 */
        ATTR_DEVICE_nGnRE       = 1,    /* MAIR byte 0x04 */
        ATTR_NORMAL_NC          = 2,    /* MAIR byte 0x44 */
        ATTR_NORMAL_WB          = 3,    /* MAIR byte 0xFF */
};

enum SHAREABILITY : uint64_t {
        SHARE_NONE              = 0,
        SHARE_OUTER             = 2,
        SHARE_INNER             = 3,
        
        // NOTE: Skipping 0b01 as it is RESERVED, we dont want to touch it by ANY chance.
};

enum AP_FLAGS : uint64_t {
        AP_RW_EL1       = 0b00,
        AP_RW_EL1_EL0   = 0b01,
        AP_RO_EL1       = 0b10,
        AP_RO_EL1_EL0   = 0b11,
};

enum BLOCK_OR_PAGE : uint64_t {
        TYPE_BLOCK = 0,
        TYPE_PAGE  = 1
};

enum LEVEL : uint64_t {
        LEVEL_L0 = 0,
        LEVEL_L1 = 1,
        LEVEL_L2 = 2,
        LEVEL_L3 = 3,
};

struct PAGE_DESCRIPTOR_BITS {
        flag_t                  is_valid        : 1;    // [0]
        enum BLOCK_OR_PAGE      type            : 1;    // [1]          For BLOCK: Choose TYPE_BLOCK; For PAGE: Choose TYPE_PAGE
        enum ATTR_INDEX         attr_index      : 3;    // [2:4]        Choose: ATTR_NORMAL_WB for regular cases
        flag_t                  is_not_secure   : 1;    // [5]          Set: false
        enum AP_FLAGS           access          : 2;    // [6:7]        Choose: AP_RW_EL1
        enum SHAREABILITY       shareability    : 2;    // [8:9]        Choose: SHARE_INNER
        flag_t                  is_accessible   : 1;    // [10]         Set: true
        flag_t                  is_not_global   : 1;    // [11]         Set: false
        uint64_t                output_addr     : 36;   // [12:47]
        uint64_t                res0            : 2;    // [48:49]
        flag_t                  is_guarded      : 1;    // [50]         Set: false
        flag_t                  set_dirty_bit   : 1;    // [51]         Set: false
        flag_t                  is_contiguous   : 1;    // [52]         Set: false
        flag_t                  is_PXN          : 1;    // [53]         Set: false
        flag_t                  is_UXN          : 1;    // [54]         Set: true
        uint64_t                reserved        : 4;    // [55:58]      Set: 0
        uint64_t                ignore0         : 4;    // [59:62]      Set: 0
        uint64_t                ignore1         : 1;    // [63],        Set: 0
};
static_assert(sizeof(struct PAGE_DESCRIPTOR_BITS) == 8, "");

struct PAGE_DESCRIPTOR_INFO {
        enum LEVEL              level;
        enum ATTR_INDEX         attr_index;     // [2:4]        Choose: ATTR_NORMAL_WB for regular cases
        enum AP_FLAGS           access;         // [6:7]        Choose: AP_RW_EL1
        enum SHAREABILITY       shareability;   // [8:9]        Choose: SHARE_INNER
};
