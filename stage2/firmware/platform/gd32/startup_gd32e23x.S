/**
 * @file startup_gd32e23x.S
 * @brief GD32E23x startup file
 */

    .section .text
    .global _start
    .global reset_handler

_start:
reset_handler:
    /* Set stack pointer */
    ldr r0, =_estack
    mov sp, r0
    
    /* Copy .data section from flash to RAM */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0
    b copy_loop
copy_loop:
    cmp r0, r1
    bge copy_done
    ldr r4, [r2, r3]
    str r4, [r0, r3]
    add r3, r3, #4
    b copy_loop
copy_done:
    
    /* Zero .bss section */
    ldr r0, =_sbss
    ldr r1, =_ebss
    movs r2, #0
    b zero_loop
zero_loop:
    cmp r0, r1
    bge zero_done
    str r2, [r0]
    add r0, r0, #4
    b zero_loop
zero_done:
    
    /* Call main function */
    bl main
    
    /* If main returns, loop forever */
hang:
    b hang

    .end
