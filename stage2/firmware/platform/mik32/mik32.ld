/* MIK32 Linker Script */

MEMORY
{
    /* MIK32 Memory Map */
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 512K   /* Boot ROM */
    RAM   (rwx) : ORIGIN = 0x02000000, LENGTH = 16K    /* Main RAM */
}

/* Stack size definition */
STACK_SIZE = 0x800; /* 2KB stack */

/* Entry point */
ENTRY(_start)

/* Sections */
SECTIONS
{
    /* Code section */
    .text :
    {
        KEEP(*(.vectors))      /* Vector table */
        *(.text*)             /* Code */
        *(.rodata*)           /* Read-only data */
        *(.srodata*)          /* Small read-only data */
        . = ALIGN(4);
    } > FLASH

    /* Data section */
    .data :
    {
        _sdata = .;            /* Start of data section */
        *(.data*)             /* Initialized data */
        *(.sdata*)            /* Small initialized data */
        . = ALIGN(4);
        _edata = .;           /* End of data section */
    } > RAM AT > FLASH

    /* BSS section */
    .bss :
    {
        _sbss = .;            /* Start of BSS section */
        *(.bss*)              /* Uninitialized data */
        *(.sbss*)             /* Small uninitialized data */
        *(COMMON)             /* Common symbols */
        . = ALIGN(4);
        _ebss = .;            /* End of BSS section */
    } > RAM

    /* Stack section */
    .stack (NOLOAD) :
    {
        . = . + STACK_SIZE;
        _estack = .;          /* End of stack */
    } > RAM

    /* Heap section (optional) */
    _heap_start = .;
    _heap_end = ORIGIN(RAM) + LENGTH(RAM);

    /* Discard unused sections */
    /DISCARD/ :
    {
        *(.eh_frame)
        *(.ARM.exidx)
        *(.comment)
        *(.debug*)
    }
}
