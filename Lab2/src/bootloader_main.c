#include "mini_uart.h"
// --- 外部函式宣告 ---

// 來自 kernel.c，這是我們內建的核心
void kernel_main(void);

// 來自 uart_loader.c，這是透過 UART 載入新核心的功能
void uart_receive_kernel(void);

// Bootloader 的主函式
void main(void) {
    // 1. 初始化 UART，這樣才能和使用者溝通
    uart_init();

    // 2. 顯示開機選單
    show_boot_menu();
    while (1) {
        /* code */
         // 3. 取得使用者選擇
        char choice = get_user_choice();
        uart_send_string("\r\n");  // 換行，讓畫面整潔

        // 4. 根據選擇執行對應的動作
        if (choice == 'U' || choice == 'u') {
            // 使用者選擇 'U'，執行 UART 載入流程
            uart_receive_kernel();
        } else if (choice == 'K' || choice == 'k') {
            // 使用者選擇 'K'，執行編譯在 bootloader 內的 kernel_main
            uart_send_string("Booting built-in kernel...\r\n\r\n");
            kernel_main();
        } else {
            // 其他無效選擇
            uart_send_string("Invalid choice. Halting.\r\n");
        }

    }

   
    // 如果 kernel_main 或 uart_receive_kernel 返回 (正常不應該)
    // 或是使用者輸入了無效選項，就在此處卡住，避免執行未知程式碼。
    while (1) {
    }
}