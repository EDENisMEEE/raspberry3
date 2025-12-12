#ifndef _CPIO_PARSE_H
#define _CPIO_PARSE_H
#include"stdint.h"

#define NEWC_MAGIC "070701"
#define TRAILER "TRAILER!!!"
#define MAX_FILES 64

void parse_newc(uint8_t *initramfs_start);

//file的儲存結構
struct file_entry {
    char* name;
    uint8_t *data;
    uint32_t size;
};

// newc header 結構（110 bytes）
struct cpio_newc_header {
    char c_magic[6];
    char c_ino[8];
    char c_mode[8];
    char c_uid[8];
    char c_gid[8];
    char c_nlink[8];
    char c_mtime[8];
    char c_filesize[8];
    char c_devmajor[8];
    char c_devminor[8];
    char c_rdevmajor[8];
    char c_rdevminor[8];
    char c_namesize[8];
    char c_check[8];
};

extern struct file_entry files[MAX_FILES];
extern int file_count;

#endif