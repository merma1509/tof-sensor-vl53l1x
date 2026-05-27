/**
 * @file syscalls.c
 * @brief Minimal system calls for bare-metal embedded systems
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include <stddef.h>

// Symbol definitions required by linker
extern char __bss_start__;
extern char __bss_end__;

// Provide weak symbol definitions that can be overridden by linker
__attribute__((weak)) char __bss_start__ = 0;
__attribute__((weak)) char __bss_end__ = 0;

// Minimal system call implementations
void _exit(int status) {
    (void)status;
    while (1) {
        // Infinite loop - embedded systems don't typically exit
    }
}

// Minimal sbrk for heap management (if needed)
void *sbrk(ptrdiff_t incr) {
    extern char end;
    static char *heap_end = &end;
    char *prev_heap_end = heap_end;
    
    if (heap_end + incr > (char *)0x20000000) {  // Simple heap limit
        return (void *)-1;
    }
    
    heap_end += incr;
    return (void *)prev_heap_end;
}

// Minimal write for printf redirection
int write(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return len;
}

// Minimal read
int read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

// Minimal close
int close(int file) {
    (void)file;
    return 0;
}

// Minimal lseek
long lseek(int file, long ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

// Minimal fstat
int fstat(int file, void *st) {
    (void)file;
    (void)st;
    return 0;
}

// Minimal isatty
int isatty(int file) {
    (void)file;
    return 1;
}
