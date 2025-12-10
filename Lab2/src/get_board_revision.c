#include "get_baord_revision.h"
#include "mailbox.h"
#include "mini_uart.h"

void get_board_revision() {
    __attribute__((aligned(16))) volatile unsigned int mailbox[7];

    mailbox[0] = sizeof(mailbox);     // total size in bytes
    mailbox[1] = REQUEST_CODE;        // 0x00000000

    mailbox[2] = GET_BOARD_REVISION;  // 0x00010002
    mailbox[3] = 4;                   // value buffer size
    mailbox[4] = 0;                   // tag request code
    mailbox[5] = 0;                   // value buffer (response here)
    mailbox[6] = END_TAG;             // 0x00000000

    if (mailbox_call(mailbox, 8) == 0) {
        uart_send_string("Board revision: 0x");
        uart_hex(mailbox[5]);
        uart_send_string("\r\n");
    } else {
        uart_send_string("Mailbox call failed.\n");
    }
}