set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_COMPILER   arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

set(MCU "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(COMMON_FLAGS "${MCU} -fdata-sections -ffunction-sections -Wall -Wextra -Wno-unused-parameter -Wno-address-of-packed-member")
set(CMAKE_C_FLAGS_INIT   "${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS_INIT "${COMMON_FLAGS} -Wno-register -fno-exceptions -fno-rtti -fno-threadsafe-statics")
set(CMAKE_ASM_FLAGS_INIT "${MCU}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)   # host
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)    # target sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)    # target sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)    # target sysroot