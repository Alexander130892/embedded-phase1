cmake_minimum_required(VERSION 3.13)
project(week33_ringbuffer C CXX ASM)

add_executable(firmware
    startup_stm32f446retx.s
    system_stm32f4xx.c
    syscalls.c
    sysmem.c
    main.cpp
)

target_compile_definitions(firmware PRIVATE STM32F446xx)

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F446RETX_FLASH.ld)

target_link_options(firmware PRIVATE
    -T${LINKER_SCRIPT}
    -Wl,--gc-sections
    -Wl,-Map=firmware.map
)

set_target_properties(firmware PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
set_target_properties(firmware PROPERTIES SUFFIX ".elf")

add_custom_command(TARGET firmware POST_BUILD
    COMMAND arm-none-eabi-size $<TARGET_FILE:firmware>
)
add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg
            -c "program $<TARGET_FILE:firmware> verify reset exit"
    DEPENDS firmware
    COMMENT "Flashing firmware to STM32F446RE via ST-LINK"
)