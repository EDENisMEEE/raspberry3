#include "stdint.h"
#include "stddef.h"
#include "string.h"
#include "cpio_parse.h"
#include "mini_uart.h"

// 讀取 ASCII hex 8 字元，轉成整數
static uint32_t read_hex32(const char *s) {
    uint32_t v = 0;
    for (int i = 0; i < 8; i++) {
        char c = s[i];
        v <<= 4;
        if (c >= '0' && c <= '9') v += c - '0';
        else if (c >= 'A' && c <= 'F') v += c - 'A' + 10;
        else if (c >= 'a' && c <= 'f') v += c - 'a' + 10;
    }
    return v;
}

int file_count = 0;
struct file_entry files[MAX_FILES];


// bare-metal initramfs parser
void parse_newc(uint8_t *initramfs_start) {
    uint8_t *ptr = initramfs_start;

    while (1) {
        struct cpio_newc_header *h = (struct cpio_newc_header *)ptr;
        ptr += sizeof(struct cpio_newc_header);

        if (memcmp(h->c_magic, NEWC_MAGIC, 6) != 0)
            break;

        uint32_t namesize = read_hex32(h->c_namesize);
        uint32_t filesize = read_hex32(h->c_filesize);

        char *name = (char *)ptr;
        ptr += namesize;
        ptr = (uint8_t *)(((uintptr_t)ptr + 3) & ~3);

        if (strcmp(name, "TRAILER!!!") == 0)
            break;

        uint8_t *filedata = ptr;
        ptr += filesize;
        ptr = (uint8_t *)(((uintptr_t)ptr + 3) & ~3);

        // 記錄檔名與內容
        files[file_count].name = name;
        files[file_count].data = filedata;
        files[file_count].size = filesize;
        file_count++;

    }
}