/**
 * @file startup_gd32e23x.c
 * @brief GD32E23x startup implementation in C
 * @author VL53L1 TOF Project
 * @date 2026
 */

#include <stdint.h>

// Forward declarations
void Reset_Handler(void);
void Default_Handler(void);

// External symbols defined by linker
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

// External function declarations
extern int main(void);
extern void SystemInit(void);

// Stack top symbol (defined in linker script)
extern uint32_t _estack;

// Vector table
__attribute__((section(".vectors"))) void (*const g_pfnVectors[])(void) = {
    (void (*)(void))(&_estack),       // Initial stack pointer
    (void (*)(void))Reset_Handler,    // Reset handler
    (void (*)(void))Default_Handler,  // NMI handler
    (void (*)(void))Default_Handler,  // Hard fault handler
    (void (*)(void))Default_Handler,  // Memory management fault
    (void (*)(void))Default_Handler,  // Bus fault
    (void (*)(void))Default_Handler,  // Usage fault
    (void (*)(void))Default_Handler,  // Reserved
    (void (*)(void))Default_Handler,  // Reserved
    (void (*)(void))Default_Handler,  // Reserved
    (void (*)(void))Default_Handler,  // Reserved
    (void (*)(void))Default_Handler,  // SVCall handler
    (void (*)(void))Default_Handler,  // Debug monitor
    (void (*)(void))Default_Handler,  // Reserved
    (void (*)(void))Default_Handler,  // PendSV handler
    (void (*)(void))Default_Handler,  // SysTick handler
    // External interrupts would go here
};

// Reset handler - entry point
void Reset_Handler(void) {
    // Initialize data section
    uint32_t *pSrc = &_sidata;
    uint32_t *pDest = &_sdata;
    
    while (pDest < &_edata) {
        *pDest++ = *pSrc++;
    }
    
    // Zero out BSS section
    pDest = &_sbss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }
    
    // Initialize system
    SystemInit();
    
    // Call main function
    main();
    
    // Should never reach here
    while (1) {
        // Infinite loop
    }
}

// Default exception handlers
void Default_Handler(void) {
    while (1) {
        // Infinite loop
    }
}

// Weak aliases for default handlers
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
// Note: SysTick_Handler is implemented in platform_gd32.c for accurate timing

// Minimal SystemInit
void SystemInit(void) {
    // Basic system initialization
    // This would typically include clock configuration
    // For now, just a placeholder
}

// Entry point symbol for linker
__attribute__((alias("Reset_Handler"))) void _start(void);
