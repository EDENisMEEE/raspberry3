/*
 * Minimal Flattened Device Tree (FDT) parser
 * ------------------------------------------------------------
 * This parser:
 *  - Parses a DTB binary in memory (no DTS text involved)
 *  - Walks the structure block linearly
 *  - Triggers callbacks for BEGIN_NODE / PROPERTY / END_NODE
 *
 * Design goals (educational / OS assignment friendly):
 *  - No dynamic allocation required
 *  - No full tree construction
 *  - Binary parsing with explicit endianness & alignment handling
 *
 * This is suitable for early kernel / boot-stage environments.
 */

#include "stdint.h"
#include "stddef.h"

/* ============================================================
 * 1. FDT constants (from Devicetree specification)
 * ============================================================
 */

#define FDT_MAGIC      0xd00dfeed

/* Structure block tokens */
#define FDT_BEGIN_NODE 0x1
#define FDT_END_NODE   0x2
#define FDT_PROP       0x3
#define FDT_NOP        0x4
#define FDT_END        0x9

/* ============================================================
 * 2. On-disk FDT header (all fields are BIG-ENDIAN)
 * ============================================================
 */

struct fdt_header {
    uint32_t magic;           /* Magic number: FDT_MAGIC */
    uint32_t totalsize;       /* Total size of DTB */
    uint32_t off_dt_struct;   /* Offset to structure block */
    uint32_t off_dt_strings;  /* Offset to strings block */
    uint32_t off_mem_rsvmap;  /* Offset to memory reserve block */
    uint32_t version;         /* FDT version */
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings; /* Size of strings block */
    uint32_t size_dt_struct;  /* Size of structure block */
};

/* ============================================================
 * 3. Callback interface
 *    The parser itself does NOT understand devices.
 * ============================================================
 */

struct fdt_callbacks {
    /* Called when a node starts */
    void (*begin_node)(const char *name, int depth, void *userdata);

    /* Called for each property */
    void (*property)(const char *name,
                     const void *value,
                     uint32_t length,
                     int depth,
                     void *userdata);

    /* Called when a node ends */
    void (*end_node)(int depth, void *userdata);
};

/* ============================================================
 * 4. Helper functions
 * ============================================================
 */

/* Convert big-endian 32-bit integer to CPU endianness */
/* rpi3 is a little endian CPU */
static uint32_t be32_to_cpu(uint32_t be)
{
    return ((be & 0x000000FFU) << 24) |
           ((be & 0x0000FF00U) << 8)  |
           ((be & 0x00FF0000U) >> 8)  |
           ((be & 0xFF000000U) >> 24);
}

/* Advance pointer to next 4-byte aligned address */
static const uint8_t *align4(const uint8_t *p)
{
    uintptr_t v = (uintptr_t)p;
    v = (v + 3) & ~((uintptr_t)3);
    return (const uint8_t *)v;
}

/* ============================================================
 * 5. Main FDT parsing function
 * ============================================================
 */

int fdt_parse(const void *dtb,
              const struct fdt_callbacks *cb,
              void *userdata)
{
    const uint8_t *base = (const uint8_t *)dtb;
    // header is the start of  
    const struct fdt_header *hdr = (const struct fdt_header *)base;

    /* ---- Step 1: Validate header ---- */
    if (be32_to_cpu(hdr->magic) != FDT_MAGIC)
        return -1; /* Not a valid FDT */

    /* Locate structure and strings blocks */
    const uint8_t *struct_ptr = base + be32_to_cpu(hdr->off_dt_struct);
    const uint8_t *struct_end = struct_ptr + be32_to_cpu(hdr->size_dt_struct);
    const char *strings = (const char *)(base + be32_to_cpu(hdr->off_dt_strings));

    int depth = 0; /* Tracks current tree depth */

    /* ---- Step 2: Walk structure block token by token ---- */
    while (struct_ptr < struct_end) {
        uint32_t token = be32_to_cpu(*(const uint32_t *)struct_ptr);
        struct_ptr += 4;

        switch (token) {

        case FDT_BEGIN_NODE: {
            /* Node name is a NULL-terminated string */
            const char *name = (const char *)struct_ptr;

            if (cb && cb->begin_node)
                cb->begin_node(name, depth, userdata);

            /* Move past name + '\0', then align */
            struct_ptr += (size_t)(strlen(name) + 1);
            struct_ptr = align4(struct_ptr);

            depth++;
            break;
        }

        case FDT_END_NODE:
            depth--;
            if (cb && cb->end_node)
                cb->end_node(depth, userdata);
            break;

        case FDT_PROP: {
            /* Property format:
             *   u32 len
             *   u32 nameoff (offset into strings block)
             *   u8  value[len]
             */
            uint32_t len = be32_to_cpu(*(const uint32_t *)struct_ptr);
            struct_ptr += 4;

            uint32_t nameoff = be32_to_cpu(*(const uint32_t *)struct_ptr);
            struct_ptr += 4;

            const void *value = struct_ptr;
            const char *name = strings + nameoff;

            if (cb && cb->property)
                cb->property(name, value, len, depth - 1, userdata);

            /* Skip property value and align */
            struct_ptr += len;
            struct_ptr = align4(struct_ptr);
            break;
        }

        case FDT_NOP:
            /* Explicit no-op, just skip */
            break;

        case FDT_END:
            /* End of structure block */
            return 0;

        default:
            /* Unknown token: malformed DTB */
            return -2;
        }
    }

    return 0;
}

/* ============================================================
 * 6. Notes for driver authors
 * ============================================================
 * - The parser does NOT interpret property values.
 * - 'compatible', 'reg', 'interrupts' etc. are handled by drivers.
 * - Depth allows drivers to understand parent-child relationships.
 * - This design matches how real kernels walk the FDT.
 */
