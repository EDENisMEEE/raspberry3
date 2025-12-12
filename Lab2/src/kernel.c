#include "mini_uart.h"
#include "string.h"
#include "reboot.h"
#include "get_baord_revision.h"
#include "stdint.h"
#include "shell.h"
#include "cpio_parse.h"

#define BUFF_MAX 1000

void kernel_main(void) {
    
    uart_init();
    parse_newc((uint8_t*)0x20000000);
    char buffer[BUFF_MAX];
    int cnt = 0;
    while (1) {
        uart_send_string("# ");
        cnt = 0;
        while (cnt < BUFF_MAX - 1) {
            char c = uart_recv();
            if ((c == '\b' || c == 127)) {  // backspace or del
                if (cnt > 0) {
                    cnt--;
                    uart_send('\b');
                    uart_send(' ');
                    uart_send('\b');
                    continue;
                }else continue;
            }

            if (c == '\r' || c == '\n') {
                buffer[cnt] = '\0';
                break;
            }
            buffer[cnt++] = c;
            uart_send(c);
        }
        if (strcmp(buffer, "help") == 0) {
            uart_send_string(
                "\r\nhelp      : print this help menu\r\n"
                "hello     : print Hello World!\r\n"
                "reboot    : reboot the device\r\n");
        } else if (strcmp(buffer, "hello") == 0) {
            uart_send_string("\r\nHello, world!\r\n");
        } else if (strcmp(buffer, "") == 0) {
            uart_send_string("\r\n");
        } 
        else if(strcmp(buffer, "reboot") == 0){
            uart_send_string("\r\n");
            reset(1*16);
        } else if(strcmp(buffer, "revision") == 0){
            uart_send_string("\r\n");
            get_board_revision();
        }else if(strcmp(buffer, "ls") == 0){
            cmd_ls();
        }
        else if(strcmp(buffer, "cat") == 0){
            cmd_cat();
        }
        else {
            uart_send_string("\r\n");
            uart_send_string("Unknown command\r\n");
        }
    }
}