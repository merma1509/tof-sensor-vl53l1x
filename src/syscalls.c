/**
 * @file  syscalls.c
 * @brief System calls for embedded systems
 */

#include <stdint.h>
#include <errno.h>

#undef errno
int errno;

/* _exit implementation */
void _exit(int status) {
    while (1) {
        // Infinite loop - system halt
    }
}

/* _write implementation */
int _write(int file, char *ptr, int len) {
    // Simple implementation for UART output
    int todo;
    (void)file; // Suppress unused parameter warning
    
    for (todo = 0; todo < len; todo++) {
        // Wait for transmit buffer empty
        while (!(*(volatile uint32_t*)0x40011008 & (1 << 7))); // USART_STAT_TBE
        *(volatile uint32_t*)0x4001100C = *ptr++; // USART_DATA
    }
    
    return len;
}

/* _read implementation */
int _read(int file, char *ptr, int len) {
    (void)file; // Suppress unused parameter warning
    (void)ptr;  // Suppress unused parameter warning
    (void)len;  // Suppress unused parameter warning
    return 0;
}

/* _sbrk implementation */
void *_sbrk(int incr) {
    errno = ENOMEM;
    return (void *)-1;
}

/* _close implementation */
int _close(int file) {
    (void)file; // Suppress unused parameter warning
    return -1;
}

/* _isatty implementation */
int _isatty(int file) {
    (void)file; // Suppress unused parameter warning
    return 1;
}

/* _fstat implementation */
int _fstat(int file, void *st) {
    (void)file; // Suppress unused parameter warning
    (void)st;  // Suppress unused parameter warning
    return 0;
}

/* _lseek implementation */
int _lseek(int file, int ptr, int dir) {
    (void)file; // Suppress unused parameter warning
    (void)ptr;  // Suppress unused parameter warning
    (void)dir;  // Suppress unused parameter warning
    return 0;
}

/* Additional functions that might be needed */
int _kill(int pid, int sig) {
    (void)pid; // Suppress unused parameter warning
    (void)sig; // Suppress unused parameter warning
    errno = ENOSYS;
    return -1;
}

int _getpid(void) {
    return 1;
}
