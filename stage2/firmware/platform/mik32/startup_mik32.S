/* MIK32 Startup Assembly File */

    .section .vectors, "ax"
    .global _start
    .global reset_handler

/* Vector Table */
_vectors:
    .word reset_handler          /* Reset vector */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */
    .word 0                      /* Reserved */

    .text
    .align 2

/* Entry point */
_start:
    j reset_handler

/* Reset handler */
reset_handler:
    /* Initialize stack pointer */
    la sp, _estack
    
    /* Clear BSS section */
    la a0, _sbss          /* Start of BSS */
    la a1, _ebss          /* End of BSS */
    li a2, 0              /* Zero */
    
bss_clear_loop:
    beq a0, a1, bss_clear_done
    sb a2, 0(a0)
    addi a0, a0, 1
    j bss_clear_loop
    
bss_clear_done:
    /* Call main function */
    call main
    
    /* If main returns, loop forever */
hang:
    j hang

/* Default interrupt handlers */
    .weak reset_handler
    .global reset_handler
