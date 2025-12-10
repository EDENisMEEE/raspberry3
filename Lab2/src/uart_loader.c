// uart_loader.c
#include "mini_uart.h"   // 假設你已有 mini_uart.h 與對應 uart 初始化函式
#include "string.h"

#define KERNEL_LOAD_ADDR 0x80000

// 接收一個 32-bit 整數（little endian）
unsigned int uart_receive_uint32(void) {
    unsigned int value = 0;
    value |= uart_recv();         // byte 0 (LSB)
    value |= uart_recv() << 8;    // byte 1
    value |= uart_recv() << 16;   // byte 2
    value |= uart_recv() << 24;   // byte 3 (MSB)
    return value;
}

// 初始化 UART（若 mini_uart.h 已有 uart_init()，就用它）
void uart_init_wrapper(void) {
    uart_init();    // 初始化 UART，設定 baudrate, 8N1 等
}

// 從 UART 接收 kernel 並跳轉執行
void uart_receive_kernel(void) {
    char buffer[12];
    uart_send_string("=== UART Kernel Loader ===\r\n");
    
    // 等待並接收 kernel 大小
    uart_send_string("Waiting for kernel size...\r\n");
    unsigned int size = uart_receive_uint32();
    

    
    uart_send_string("Kernel size received:\r\n ");
    itoa_baremetal(size, buffer);
    uart_send_string(buffer);
    // 可選：顯示 size（需將整數轉成字串，簡單版本省略）
    
    uart_send_string("Receiving kernel image...\r\n");
    
    unsigned char *dest = (unsigned char *)KERNEL_LOAD_ADDR;
    for (unsigned int i = 0; i < size; i++) {
        dest[i] = uart_recv();  // 每次接收 1 byte
    }
    
    uart_send_string("Kernel received, jumping to kernel...\r\n");
    
    // 宣告函式指標並跳轉到新 kernel
    void (*new_kernel)(void) = (void (*)())KERNEL_LOAD_ADDR;
    new_kernel();
}

//可選：在 bootloader _start 裡呼叫這個函式
void bootloader_main(void) {
    uart_init_wrapper();
    uart_receive_kernel();
    
    // 如果 kernel 沒成功啟動，這裡可以加個無限迴圈避免執行亂資料
    while (1) { }
}
// void bootloader_main(void) {
//     uart_init();
//     uart_send_string("Bootloader is alive!\r\n");
//     // ... 執行 kernel.c 的簡單邏輯
//     while(1) {
//         uart_send_string("# ");
//         uart_recv(); // 隨意接收一個字元
//     }
// }