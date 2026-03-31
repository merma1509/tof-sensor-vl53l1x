/**
 * @file  syscalls.c
 * @brief System calls for embedded systems
 */

/* _exit implementation */
void _exit(int status) {
    while (1) {
        // Infinite loop - system halt
    }
}

/* write implementation */
int _write(int file, const void *ptr, int len) {
    // Stub implementation
    return len;
}

/* read implementation */
int _read(int file, void *ptr, int len) {
    // Stub implementation
    return 0;
}

/* fstat implementation */
int _fstat(int file, void *st) {
    // Stub implementation
    return 0;
}

/* isatty implementation */
int _isatty(int file) {
    // Stub implementation
    return 1;
}

/* lseek implementation */
int _lseek(int file, int ptr, int dir) {
    // Stub implementation
    return 0;
}

/* close implementation */
int _close(int file) {
    // Stub implementation
    return 0;
}

/* sbrk implementation */
void *_sbrk(int incr) {
    // Stub implementation
    extern char end;
    static char *heap_end = &end;
    char *prev_heap_end = heap_end;
    
    if (heap_end + incr > (char*)0x20000000) {
        return (void*)-1;
    }
    
    heap_end += incr;
    return (void*)prev_heap_end;
}
