#include "mini_uart.h"
#include "string.h"
#include "cpio_parse.h"
#include "shell.h"
#define CMD_BUF 64


void cmd_ls() {
    uart_send_string("\r\n");
    for (int i = 0; i < file_count; i++) {
        uart_send_string((files[i].name));
        uart_send_string(" ");
    }
    uart_send_string("\r\n");
}

void cmd_cat(){
    char filename[CMD_BUF];
    uart_send_string(" Filename: ");
    int cnt = 0;
    while (cnt < CMD_BUF- 1) {
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
                filename[cnt] = '\0';
                break;
            }
            filename[cnt++] = c;
            uart_send(c);
        }

    for (int i = 0; i < file_count; i++) {
        if (strcmp(files[i].name, filename) == 0) {
            // 印出內容
            uart_send_string("\r\n");
            for (uint32_t j = 0; j < files[i].size; j++) {
                uart_send(files[i].data[j]);
            }
            uart_send_string("\r\n");
            return;
        }
    }

    uart_send_string("File not found\r\n");
}