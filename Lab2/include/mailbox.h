#ifndef MAILBOX_H
#define MAILBOX_H

#include "types.h"
// Mailbox base address for Raspberry Pi 3B
#define MMIO_BASE       0x3F000000UL
#define MAILBOX_BASE    (MMIO_BASE + 0xB880)

//mailbox 0 READ
#define MAILBOX_READ    ((volatile uint32_t*)(MAILBOX_BASE + 0x00))
//mailbox 0 stastus
#define MAILBOX_STATUS  ((volatile uint32_t*)(MAILBOX_BASE + 0x18))
//mailbox1 1 write
#define MAILBOX_WRITE   ((volatile uint32_t*)(MAILBOX_BASE + 0x20))

// Status flags
#define MAILBOX_EMPTY   0x40000000  // bit 30
#define MAILBOX_FULL    0x80000000  // bit 31

#define MAILBOX_SUCCESS 0
#define MAILBOX_FAIL   -1

int mailbox_call(volatile unsigned int *buffer, uint8_t channel);

#endif