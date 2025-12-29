#include "mm.h"

.section ".text.boot"

.globl _start
_start:
	
    adrp  x1, dtb_ptr_glob
    add   x1, x1, :lo12:dtb_ptr_glob
    str   x0, [x1]    // 把原始的 x0 (DTB 位址) 存進去
	//運行時位址 The real address of start_(0x80000)
	adr x0, _start
	//連結時位址 This is where the linker wants _start is on(defined in linker file)
	ldr x1, =_start
	//
	cmp x0, x1
	b.eq  _check_proc_id
	b.ne  _prep

_prep:
	ldr x2, =_data_end
	//get the size of the whole dataloader
	sub x2, x2, x1
	b _copy_loop

_copy_loop:
	//load data from the memory address stores in x1 to x3, automatically add 8 to address in x0
	ldr x3, [x0], #8
	//store x3 to the memory address stores in x1,  automatically add 8 to address in x1
	str x3, [x1], #8
	subs x2, x2, #8
	b.gt _copy_loop
	ldr x3, =_check_proc_id
	//force jump, this will move PC to new address
	br x3

_check_proc_id:

	mrs	x0, mpidr_el1		
	and	x0, x0,#0xFF		// Check processor id
	cbz	x0, master		// Hang for all non-primary CPU
	b	proc_hang


proc_hang: 
	b 	proc_hang

master:
	adr	x0, bss_begin
	adr	x1, bss_end
	sub	x1, x1, x0
	bl 	memzero

	mov	sp, #LOW_MEMORY
	mov x0, x20
	bl  main
	
	b 	proc_hang		// should never come here
