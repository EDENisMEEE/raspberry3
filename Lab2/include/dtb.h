#ifndef DTB_H
#define DTB_H

#include <stdint.h>

/* ============================================================
 * 1. 全域變數聲明
 * ============================================================
 * 使用 extern 讓其他檔案（如 main.c 或 cpio.c）可以存取 
 * 解析後得到的 CPIO 起始位址。
 */
extern void *CPIO_DEFAULT_START;

/* ============================================================
 * 2. 函式原型
 * ============================================================
 */

/**
 * 解析 Device Tree 並尋找必要的系統資訊（如 initramfs 位置）
 * @param dtb_ptr 指向 DTB 在記憶體中的起始位置（通常由 x0 寄存器傳入）
 */
void init_device_tree(void *dtb_ptr);

/**
 * (選擇性) 可以在此處定義 DTB 相關的 Callback 函式，
 * 若你想在其他地方自定義不同的解析行為。
 */
void initrd_callback(char *name, 
                     const void *value, 
                     uint32_t len, 
                     int depth, 
                     void *userdata);

#endif /* DTB_H */