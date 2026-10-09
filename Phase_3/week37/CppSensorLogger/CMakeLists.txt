cmake_minimum_required(VERSION 3.22)
project(CppSensorLogger LANGUAGES C CXX ASM)
enable_testing()
add_library(fixed_format
    Src/fixed_format.cpp
)
# Set to c++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set C Standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)    # gnu11, matching Week 17

target_include_directories(fixed_format PUBLIC Inc)

target_compile_options(fixed_format PRIVATE
    -Wall -Wextra -Wconversion
)
add_library(ring_buffer INTERFACE)
target_include_directories(ring_buffer INTERFACE Inc)

add_library(mpu6050 INTERFACE)
target_include_directories(mpu6050 INTERFACE Inc)

if(CMAKE_CROSSCOMPILING)
    add_executable(firmware)
    set_target_properties(firmware PROPERTIES SUFFIX ".elf")

    target_sources(firmware PRIVATE
        Startup/startup_stm32f446retx.s
        Src/syscalls.c
        Src/sysmem.c
        Src/systick.c
        Src/uart.cpp
        Src/bsp.cpp
        Src/i2c.c
        Src/main.cpp
    )
    target_include_directories(firmware PRIVATE
        Inc/
    )
    target_link_options(firmware PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld
        --specs=nano.specs
        -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
    )
    target_link_libraries(firmware PRIVATE fixed_format) 
else()
    # Unity test framework (vendored, C)
    add_library(unity
        test/unity/unity.c
    )
    target_include_directories(unity PUBLIC test/unity)

    # Host unit tests for fixed_format
    add_executable(test_fixed_format
        test/test_fixed_format.cpp
    )
    target_link_libraries(test_fixed_format PRIVATE fixed_format unity)
    add_test(NAME fixed_format_test COMMAND test_fixed_format)
    
     # Host unit tests for test_ring_buffer
    add_executable(test_ring_buffer
    test/test_ring_buffer.cpp
    )
    target_link_libraries(test_ring_buffer PRIVATE ring_buffer unity)
    add_test(NAME ring_buffer_test COMMAND test_ring_buffer)

    # Host unit tests for test_mpu6050
    add_executable(test_mpu6050
        test/test_mpu6050.cpp
    )
    target_link_libraries(test_mpu6050 PRIVATE mpu6050 unity)
    add_test(NAME mpu6050_test COMMAND test_mpu6050)

endif()