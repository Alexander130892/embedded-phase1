cmake_minimum_required(VERSION 3.22)
project(CppSensorLogger LANGUAGES C CXX ASM)
enable_testing()
add_library(fixed_format
    Src/fixed_format.cpp
)
target_include_directories(fixed_format PUBLIC Inc)

target_compile_options(fixed_format PRIVATE
    -Wall -Wextra -Wconversion
)

if(CMAKE_CROSSCOMPILING)
    add_executable(firmware)
    set_target_properties(firmware PROPERTIES SUFFIX ".elf")

    target_sources(firmware PRIVATE
        Startup/startup_stm32f446retx.s
        Src/syscalls.c
        Src/sysmem.c
        Src/systick.c
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
endif()