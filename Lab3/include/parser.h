#ifndef FDT_PARSER_H
#define FDT_PARSER_H

#include <stdint.h>
#include <stddef.h>

/* ============================================================
 * 1. FDT 標記 (Tokens)
 * ============================================================
 */
#define FDT_MAGIC      0xd00dfeed
#define FDT_BEGIN_NODE 0x00000001
#define FDT_END_NODE   0x00000002
#define FDT_PROP       0x00000003
#define FDT_NOP        0x00000004
#define FDT_END        0x00000009

/* ============================================================
 * 2. FDT 結構體定義
 * ============================================================
 */

/* FDT Header 結構 */
struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

/* Callback 函式指標結構 */
struct fdt_callbacks {
    /* 當進入一個新節點時觸發 */
    void (*begin_node)(char *name, int depth, void *userdata);

    /* 當解析到屬性時觸發 */
    void (*property)(char *name, const void *value, uint32_t length, int depth, void *userdata);

    /* 當退出一個節點時觸發 */
    void (*end_node)(int depth, void *userdata);
};

/* ============================================================
 * 3. 函式原型 (Function Prototypes)
 * ============================================================
 */

/**
 * 解析記憶體中的 DTB 檔案
 * @param dtb      指向 DTB 在記憶體中的起始位置
 * @param cb       指向 callback 結構體，定義解析時的動作
 * @param userdata 使用者自定義資料，會傳遞給 callback
 * @return 0 成功, 負值表示錯誤
 */
int fdt_parse(const void *dtb, const struct fdt_callbacks *cb, void *userdata);
uint32_t be32_to_cpu(uint32_t be);
#endif /* FDT_PARSER_H */