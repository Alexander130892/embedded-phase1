# cmake/arm-none-eabi.cmake

# Target system: bare metal, no OS
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Compilers
set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Compiler check: build a static library, no link (no startup/linker script yet)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# CPU flags: must reach the linker too, so the right multilib is picked
set(MCU_FLAGS "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb")

# Per-function/data sections, paired with --gc-sections at link time
set(SECTION_FLAGS "-ffunction-sections -fdata-sections")

set(CMAKE_C_FLAGS_INIT   "${MCU_FLAGS} ${SECTION_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${MCU_FLAGS} ${SECTION_FLAGS} -fno-exceptions -fno-rtti -fno-threadsafe-statics")
set(CMAKE_ASM_FLAGS_INIT "${MCU_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${MCU_FLAGS} -Wl,--gc-sections")