/**
 * @file  startup_gd32e23x.c
 * @brief Startup file for GD32E23x
 */

/* Entry point */
extern int main(void);

/* Reset handler */
void Reset_Handler(void) {
    // Call main function
    main();
    
    // Infinite loop if main returns
    while (1) {
        // Do nothing
    }
}

/* Default interrupt handlers */
void Default_Handler(void) {
    while (1) {
        // Infinite loop
    }
}

/* Weak aliases for interrupt handlers */
void NMI_Handler(void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__ ((weak, alias("Default_Handler")));

/* Entry point symbol */
void _start(void) __attribute__ ((weak, alias("Reset_Handler")));
