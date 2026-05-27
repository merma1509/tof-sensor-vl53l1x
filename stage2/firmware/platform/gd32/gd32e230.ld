/**
 * @file gd32e230.ld
 * @brief GD32E230 linker script
 */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 64K
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 8K
}

/* Stack size definitions */
_Min_Heap_Size = 0x100;
_Min_Stack_Size = 0x400;

/* Entry point */
ENTRY(_start)

/* Sections */
SECTIONS
{
    .text :
    {
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
        *(.glue_7)
        *(.glue_7t)
        *(.eh_frame)

        KEEP (*(.init))
        KEEP (*(.fini))

        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data :
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH
    
    /* Data section load address */
    _sidata = LOADADDR(.data);

    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > RAM

    /* Stack section */
    ._user_stack_stack :
    {
        . = ALIGN(8);
        . = . + _Min_Stack_Size;
        . = ALIGN(8);
    } > RAM

    /* Heap section */
    ._heap :
    {
        . = ALIGN(8);
        . = . + _Min_Heap_Size;
        . = ALIGN(8);
    } > RAM

    /* Stack top */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
