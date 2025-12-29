#include "parser.h" // 包含你提供的 struct 定義
#include "stdint.h"
#include "stddef.h"
#include "mini_uart.h"
#include "string.h"
// 全域變數儲存 cpio 位置
// CPIO 不是特定的資料型態
void *CPIO_DEFAULT_START = 0;


// 專門用來找 initramfs 的 Callback
void initrd_callback(char *name, const void *value, uint32_t len, int depth, void *userdata) {
    if (strcmp(name, "linux,initrd-start") == 0) {
        // 將大端序轉為小端序並存起來
        // 取出真正的value
        uint32_t start_addr = be32_to_cpu(*(uint32_t *)value);
        CPIO_DEFAULT_START = (void *)(uintptr_t)start_addr;
        uart_send_string(" Get initramsfs address\r\n");
    }
}

// 初始化函數：供 main 呼叫
void init_device_tree(void *dtb_ptr) {
    struct fdt_callbacks cb = {0};
    cb.property = initrd_callback;
    
    // 呼叫你在 parser.c 寫好的解析函數
    fdt_parse(dtb_ptr, &cb, NULL);
}