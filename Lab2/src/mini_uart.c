#include "peripherals/mini_uart.h"

#include "peripherals/gpio.h"
#include "utils.h"

void uart_send(char c) {
    while (1) {  // bit 5 = 1, transmit empty,we can write to UART
        if (get32(AUX_MU_LSR_REG) & 0x20) break;
    }
    put32(AUX_MU_IO_REG, c);
}

char uart_recv(void) {
    while (1) {  // bit 0 = 1, Data ready, we can read from UART
        if (get32(AUX_MU_LSR_REG) & 0x01) break;
    }
    return (get32(AUX_MU_IO_REG) & 0xFF);
}

void uart_send_string(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        uart_send((char)str[i]);
    }
}

void uart_init(void) {
    unsigned int selector;

    selector = get32(GPFSEL1);
    selector &= ~(7 << 12);  // clean gpio14
    selector |= 2 << 12;     // set alt5 for gpio14
    selector &= ~(7 << 15);  // clean gpio15
    selector |= 2 << 15;     // set alt5 for gpio15
    put32(GPFSEL1, selector);

    put32(GPPUD, 0);
    delay(150);
    put32(GPPUDCLK0, (1 << 14) | (1 << 15));
    delay(150);
    put32(GPPUDCLK0, 0);

    put32(AUX_ENABLES,
          1);  // Enable mini uart (this also enables access to its registers)
    put32(AUX_MU_CNTL_REG, 0);  // Disable auto flow control and disable
                                // receiver and transmitter (for now)
    // *** 在這裡加上清除 FIFO 的指令 ***
    // 寫入 0b110 (0x06) 可以同時清除接收(bit 1)和傳送(bit 2)的 FIFO
    put32(AUX_MU_IIR_REG, 0x06);
    
    put32(AUX_MU_IER_REG, 0);     // Disable receive and transmit interrupts
    put32(AUX_MU_LCR_REG, 3);     // Enable 8 bit mode
    put32(AUX_MU_MCR_REG, 0);     // Set RTS line to be always high
    put32(AUX_MU_BAUD_REG, 270);  // Set baud rate to 115200

    put32(AUX_MU_CNTL_REG, 3);  // Finally, enable transmitter and receiver
}

void uart_hex(unsigned int d) {
    uart_send('0');
    uart_send('x');
    for (int c = 28; c >= 0; c -= 4) {
        unsigned int n = (d >> c) & 0xF;
        n += n > 9 ? 0x37 : 0x30;// convert HEX to printable ASCII code
        uart_send(n);
    }
}
void show_boot_menu(void){
    uart_send_string("Boot Menu:\r\n");
    uart_send_string(" [U] Upload kernel via mini-UART\r\n");
    uart_send_string(" [K] use build in kernel\r\n");
}

char get_user_choice(void){// the return value will be stored in x0
    uart_send_string("Enter choice: ");
    return uart_recv();
}
