# Test Suite for Portable VL53L1 TOF Firmware

This directory contains test suites for validating the functionality of the portable VL53L1 TOF firmware architecture.

## Test Files

### test_common_utils.c
Tests the common utility functions including:
- String utilities (safe_strcpy, safe_strcat, safe_strlen, safe_memcpy, safe_snprintf)
- Math utilities (int_to_string, float_to_string)
- Delay utilities (delay_ms, delay_us, get_tick_ms, get_tick_us)
- Debug utilities (debug_print, debug_assert, debug_set_enabled)
- CRC utilities (crc8_calculate, crc16_calculate, crc32_calculate)
- Endian utilities (swap_uint16, swap_uint32, host_to_network_xxx)

### test_tof_service.c
Tests the TOF service functionality including:
- Service initialization and deinitialization
- Mode setting (short, medium, long range)
- Ranging start/stop operations
- Distance measurement
- Filter enable/disable
- Status reporting

### test_platform.c
Tests the platform abstraction layer including:
- Platform initialization
- UART operations (init, send, receive)
- I2C operations (init, read, write, register operations)
- GPIO operations (init, set, get)
- Timing functions (delay, tick)
- Platform macros

## Building Tests

### For GD32 Platform
```bash
# Build the test firmware
make TARGET_PLATFORM=GD32 TEST_BUILD=1

# Or build individual test
make TARGET_PLATFORM=GD32 test_common_utils
make TARGET_PLATFORM=GD32 test_tof_service
make TARGET_PLATFORM=GD32 test_platform
```

### For MIK32 Platform
```bash
# Build the test firmware
make TARGET_PLATFORM=MIK32 TEST_BUILD=1

# Or build individual test
make TARGET_PLATFORM=MIK32 test_common_utils
make TARGET_PLATFORM=MIK32 test_tof_service
make TARGET_PLATFORM=MIK32 test_platform
```

## Running Tests

### Hardware Requirements
- GD32E230 or MIK32 development board
- UART connection for debug output (115200 baud)
- VL53L1X sensor connected via I2C (for TOF service tests)
- Optional: GPIO loopback for GPIO tests

### Test Execution
1. Flash the test firmware to your target platform
2. Connect UART to see test output
3. Power on the board
4. Monitor the UART output for test results

### Expected Output
```
Starting common utilities test suite...
=====================================
Testing string utilities...
PASS: 5 == safe_strlen("Hello")
PASS: 0 == safe_strlen("")
PASS: 0 == safe_strlen(NULL)
...
All tests PASSED!
=====================================
```

## Test Framework

The tests use a simple custom test framework with these macros:
- `TEST_ASSERT(condition)` - Assert that a condition is true
- `TEST_ASSERT_EQUAL(expected, actual)` - Assert two values are equal

## Test Coverage

### Common Utilities Coverage
- String manipulation functions
- Memory operations
- Mathematical conversions
- Timing functions
- Debug output
- CRC calculations
- Endian conversions

### TOF Service Coverage
- Service lifecycle (init/deinit)
- Mode management
- Ranging operations
- Distance measurement
- Filter operations
- Status reporting
- WARNING: Sensor-specific functionality (requires actual hardware)

### Platform Coverage
- Platform initialization
- UART communication
- I2C communication
- GPIO operations
- Timing functions
- WARNING: Hardware-specific features (requires actual hardware)

## Adding New Tests

To add a new test:

1. Create a new test file: `test_new_feature.c`
2. Include necessary headers
3. Use the test framework macros
4. Add test functions with descriptive names
5. Add a main function that calls all test functions
6. Update the Makefile to include the new test

Example test function:
```c
static int test_new_feature(void) {
    debug_print("Testing new feature...\n");
    
    // Test basic functionality
    TEST_ASSERT_EQUAL(0, new_feature_init());
    
    // Test operations
    TEST_ASSERT_EQUAL(1, new_feature_do_something());
    
    // Cleanup
    TEST_ASSERT_EQUAL(0, new_feature_deinit());
    
    debug_print("New feature tests passed!\n");
    return 0;
}
```

## Troubleshooting

### Common Issues
1. **Build failures**: Check that all required headers are included
2. **Runtime failures**: Verify hardware connections and power
3. **Test timeouts**: Check that debug output is properly configured
4. **Sensor tests failing**: Verify I2C connections and sensor power

### Debug Tips
- Enable debug output: `debug_set_enabled(true)`
- Use platform-specific debug macros: `DEBUG_PRINT("Debug info\n")`
- Check return codes from all function calls
- Verify hardware connections with a multimeter

## Integration with CI/CD

These tests can be integrated into continuous integration systems:
- Unit tests can run on host systems with mock hardware
- Integration tests require actual hardware
- Test results can be parsed from UART output
- Failed tests should return non-zero exit codes

## Notes

- Tests are designed for both unit testing and integration testing
- Some tests require actual hardware to pass completely
- The test framework is minimal but sufficient for embedded testing
- All tests include comprehensive error reporting
