/**
 * @file platform.c
 * @brief Platform abstraction implementation
 */

#include "platform.h"

#ifdef TARGET_PLATFORM_MIK32
extern const platform_interface_t mik32_platform_interface;
#endif

/* Platform initialization */
platform_status_t platform_init(void) {
    const platform_interface_t *interface = platform_get_interface();
    
    if (interface && interface->system_init) {
        return interface->system_init();
    }
    
    return PLATFORM_ERROR;
}

/* Platform deinitialization */
platform_status_t platform_deinit(void) {
    // Platform-specific cleanup if needed
    return PLATFORM_OK;
}

/* Get platform interface */
const platform_interface_t* platform_get_interface(void) {
#ifdef TARGET_PLATFORM_GD32
    return &gd32_platform_interface;
#elif defined(TARGET_PLATFORM_MIK32)
    return &mik32_platform_interface;
#else
    return NULL;
#endif
}
