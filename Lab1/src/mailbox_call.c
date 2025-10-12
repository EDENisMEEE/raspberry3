#include "mailbox.h"
#include "mini_uart.h"

int mailbox_call(volatile uint32_t *buffer, uint8_t channel) {
    uint32_t buffer_addr = (uint32_t)((uintptr_t)buffer);

    if (buffer_addr & 0xF) {
        uart_send_string("Error: mailbox buffer not 16-byte aligned\n");
        return MAILBOX_FAIL;
    }

    // Wait until we can write to mailbox
    while (*MAILBOX_STATUS & MAILBOX_FULL);

    *MAILBOX_WRITE = (buffer_addr & 0xFFFFFFF0) | (channel & 0xF);

    // Wait for response
    while (1) {
        while (*MAILBOX_STATUS & MAILBOX_EMPTY);
        uint32_t response = *MAILBOX_READ;

        if ((response & 0xF) == channel &&
            (response & 0xFFFFFFF0) == (buffer_addr & 0xFFFFFFF0)) {

            if (buffer[1] == 0x80000000) {
                return MAILBOX_SUCCESS;
            } else {
                uart_send_string("GPU returned failure code\n");
                return MAILBOX_FAIL;
            }
        }
    }
}