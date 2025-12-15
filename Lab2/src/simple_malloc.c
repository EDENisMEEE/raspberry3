#include "stddef.h"
#include "mini_uart.h"

extern unsigned char bss_end[];
static unsigned char *current_ptr = bss_end;

void* simple_malloc(size_t size){
    if (size == 0)
        return NULL;
    
    //align to 8 bytes, make sure data type like uint64_t could be correctly acessed
    current_ptr = (unsigned char *)(((unsigned long)current_ptr + 7) & ~7);
    
    void* allocated_addr = (void*)current_ptr;
    current_ptr += size;
    
    uart_send_string("Allocated addr: ");
    uart_hex((unsigned long)allocated_addr);
    uart_send_string("\r\n");

    return allocated_addr;
}