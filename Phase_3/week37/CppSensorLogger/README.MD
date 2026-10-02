# Week 37 — C++ Deliverable + Integration (`CppSensorLogger`)

## Goal

Rewrite the Phase 2 Week 17 STM32F446RE sensor logger (BMP280 over SPI, MPU-6050 over I2C, CSV over UART) in embedded C++. The C++ version has no heap, no exceptions and no RTTI, runs as a pure super-loop with no FreeRTOS, and is built with CMake instead of STM32CubeIDE.

This session laid the foundation: an IDE-independent CMake build that produces a linking `firmware.elf` with a verified baseline size. The C++ driver classes, `SensorFsm`, the observer wiring, static analysis and the `v1.0-phase3-cpp` tag are still to do.

> 🏁 **M19 checkpoint:** C++ sensor logger running on STM32. CMake build. Static analysis clean. ⏳ *In progress (build system done)*

## Scope

- CMake cross-compilation: a toolchain file, `CMAKE_SYSTEM_NAME Generic`, and `CMAKE_CROSSCOMPILING`
- GCC target triples (`arm-none-eabi`) and the gcc driver as the front end for C, C++ and ASM
- Multilib selection (`-print-multi-lib`) and why CPU flags must reach the linker
- Embedded C++ compiler flags: `-fno-exceptions`, `-fno-rtti`, `-fno-threadsafe-statics`
- Section garbage collection: `-ffunction-sections -fdata-sections` with `-Wl,--gc-sections`
- Linker scripts, map files, the vector table, and the startup sequence (`Reset_Handler` → `__libc_init_array` → `main`)
- Fixed-point output formatting and the C semantics of negative integer division
- Host-based versus on-target testing; portability differences between x86/arm64 hosts and Cortex-M

## Architecture

### Build system

```
CppSensorLogger/
├── CMakeLists.txt              ← what to build (targets, sources, linker script, map)
├── STM32F446RETX_FLASH.ld      ← memory layout (from Week 17)
├── cmake/
│   └── arm-none-eabi.cmake     ← which compiler + CPU (toolchain file)
├── Inc/   systick.h
├── Src/   main.cpp  systick.c  syscalls.c  sysmem.c
└── Startup/ startup_stm32f446retx.s

            ┌──────────────── one source tree ────────────────┐
            │                                                 │
 cmake -B build/fw                                   cmake -B build/host
 -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake    (native compiler)
            │                                                 │
 CMAKE_CROSSCOMPILING = TRUE                         CMAKE_CROSSCOMPILING = FALSE
            │                                                 │
            ▼                                                 ▼
   firmware.elf + firmware.map                       test_fixed_format (Unity)
   (arm-none-eabi-g++, thumb/v7e-m+fp/hard)          (planned)
```

### Boot path (verified in the map file)

```
Reset ─► Reset_Handler (startup .s, 80 B)
           ├─ set SP = _estack
           ├─ bl SystemInit          (.weak, undefined → linker turns bl into NOP)
           ├─ copy .data, zero .bss
           ├─ bl __libc_init_array   (libg_nano.a, 72 B) ← C++ global constructors run here
           └─ bl main                (main.cpp, 6 B: for(;;);)

Unhandled IRQs / HardFault ─► Default_Handler @ 0x080002A0 (infinite loop)
SysTick ─► SysTick_Handler (systick.c) ─► tick_ms++
```

## Key design decisions

**CMake instead of STM32CubeIDE.** Week 17 was built entirely in the IDE's managed build. For this project the build system is part of the deliverable. The `-fno-exceptions -fno-rtti` rules should be visible as lines in a reviewed file, not hidden in `.cproject` XML. `clang-tidy` needs the `compile_commands.json` that CMake generates. The Weeks 39–44 GitHub Actions CI needs a headless build. CubeIDE can still serve as a GDB front end for the CMake-built ELF.

**Toolchain file versus `CMakeLists.txt`.** The rule: settings about the CPU or compiler go in `cmake/arm-none-eabi.cmake`, and settings about this firmware go in `CMakeLists.txt`. The CPU flags and C++ language flags went in the toolchain file. The linker script, the map file and `--specs=nano.specs` went in `CMakeLists.txt` (nano.specs is a judgment call, since a different project on the same chip could want full newlib). I put section GC in the toolchain file because every Cortex-M build here wants it. The optimization level goes in neither file and comes from `CMAKE_BUILD_TYPE`.

**`arm-none-eabi-gcc` as the ASM compiler, not `arm-none-eabi-as`.** The gcc driver accepts the same flag set as C and C++, so it accepts `-ffunction-sections`, `-D` and `-I` without errors. It also forwards the matching CPU and FPU flags to `as`, so the startup object's ABI attributes match the C++ objects and the link doesn't fail with "uses VFP register arguments". And it runs the preprocessor on `.S` files if the startup file ever needs to share constants with headers.

**`CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY`.** CMake's compiler check links a test program. On bare metal that link fails because there's no linker script, no startup code and no syscall stubs. Building a static library instead tests only the compiler, and link problems show up in the real `firmware.elf` build, where the project supplies all three.

**`-fno-threadsafe-statics`.** C++11 guarantees that a function-local static is initialized exactly once across threads, and the compiler implements it with `__cxa_guard_acquire` and `__cxa_guard_release`. On a single-core super-loop that locking does nothing and adds libsupc++ code, and it doesn't protect against the real hazard, an ISR interrupting a construction in progress. The rule I follow instead: construct everything from `main` before enabling interrupts, and never let an ISR be the first caller of a function with a local static.

**Fixed-point instead of `printf("%f")`.** Week 17 linked `-u _printf_float`. Newlib-nano's float formatting is a likely path into `malloc`, which conflicts with the no-heap rule. Sensor values will be fixed-point integers, for example milli-g, formatted by a free function `format_fixed(char* out, size_t cap, int32_t value, uint8_t frac_digits)` in its own unit, which has no hardware dependencies.

**Host tests with Unity for pure logic.** `format_fixed` will be tested natively with Unity as a separate CMake target. It runs in milliseconds, needs no board, works on any CI runner, and can use sanitizers. On-target and Renode testing stay as options for hardware-dependent code. The host build should use `-funsigned-char -Wall -Wextra -Wformat=2 -Wconversion` to remove host/target differences I already know about.

**Output named `firmware.elf` through a property.** `set_target_properties(firmware PROPERTIES SUFFIX ".elf")` keeps the CMake target name `firmware` separate from the file name.

## Baseline measurement

First successful build: an empty `for(;;);` main, GCC 15.3.1, Debug.

| | text | data | bss |
|---|---|---|---|
| `firmware.elf` | 816 | 0 | 1568 |

- **text 816** = vector table 452 (0x1C4, 16 core + 97 STM32F446 entries) + crtbegin.o 140 + `Reset_Handler` 80 + `__libc_init_array` 72 + `SysTick_Handler` 28 + `main` 6 + `Default_Handler`, padding and crti/crtn stubs
- **bss 1568** = `._user_heap_stack` 1536 (heap 0x200 + stack 0x400) + `.bss` 32 (crtbegin.o `object.0` 24 + `completed.1` 1 + padding 3, `tick_ms` 4)
- The map file confirms the libraries come from `thumb/v7e-m+fp/hard/` (multilib correct), `libg_nano.a` (nano specs active), and that `.eh_frame` is 0 bytes (`-fno-exceptions` effective)

## Lessons Learned

- **Plain `gcc` is the host compiler.** On my Mac, `gcc -dumpmachine` prints `arm64-apple-darwin25.5.0`. It contains "arm", but it targets AArch64 with a Darwin OS. The triple has three parts (arch-vendor-os), and the STM32 target is `arm-none-eabi`, where plain `arm` already means 32-bit. The compiler executable is the triple plus a tool name: `arm-none-eabi-gcc`, not `arm-none-eabi`.
- **Compiler flags need their leading `-`.** I first wrote `mcpu=cortex-m4 ...`, without the dash. gcc would treat that as an input file name. I also pulled in `-o` from the CubeIDE line by mistake. It's the output-file option, not a CPU flag.
- **Negative forms of `-f` flags are `-fno-<name>`, with a hyphen**: `-fno-exceptions`, not `-fnoexceptions`.
- **The linker picks the C library at link time from the flags it receives.** Without `MCU_FLAGS` on the link line, the driver falls back to the default multilib (`.`), which is ARM-state and soft-float. Best case is a VFP-ABI link error; worst case is ARM instructions in the image that HardFault the first time a Thumb-only Cortex-M4 executes them. The `-print-multi-lib` row that matches my flags is `thumb/v7e-m+fp/hard`.
- **`--gc-sections` alone does almost nothing.** The linker can only discard whole sections, and by default one file's functions share one `.text`. `-ffunction-sections` makes the pieces small enough to discard. The vector table survives only because the linker script marks it with `KEEP()`.
- **CMake's language name for C++ is `CXX`, not `CPP`.** It's the same name used in `CMAKE_CXX_COMPILER`.
- **`${PWD}` in CMake is an empty CMake variable, not the shell's working directory.** The link runs in the build directory, so a relative linker script path breaks. Use `${CMAKE_SOURCE_DIR}`. A relative map-file path is fine because generated output belongs in the build directory, or use `${CMAKE_BINARY_DIR}` to be explicit.
- **`-T` passes a linker script; `-L` adds a library search path. `-Map` is an `ld` option and needs `-Wl,`**, while `--specs` goes to the gcc driver itself.
- **`CMakeLists.txt` must sit in the source directory given to `-S`.** CMake doesn't search subfolders such as `cmake/`.
- **An unused `#include` still has to resolve.** `systick.h` included `status.h` but used nothing from it. Rather than copying the file, I removed the include.
- **`main` must never return on bare metal.** With no OS to return to, control would fall into `LoopForever` in the startup file.
- **"Unknown destination type (ARM/Thumb)" and "dangerous relocation" were knock-on errors** from `undefined reference to 'main'`. A Thumb `bl` can't be resolved to a symbol that doesn't exist.
- **The Week 17 `"%ld.%02ld", v/100, v%100` formatting is wrong for negatives.** I verified on the host: `-150` prints as `-1.-50` and `-5` as `0.-5`. In C99, division truncates toward zero and `%` takes the sign of the dividend. Wrapping the fraction in `abs()` still loses the sign for `-5`. The fix is to emit the sign once and format the magnitude, using `uint32_t m = 0u - (uint32_t)v` so that `INT32_MIN` is handled without signed-overflow UB.
- **Host and target types differ.** Plain `char` is signed on x86 and unsigned on ARM (AAPCS). `long` is 64-bit on x86-64 Linux and 32-bit on Cortex-M. In newlib for ARM, `int32_t` is `long`, so Week 17's `%ld` is correct on the STM32 and undefined behaviour on the host. Use `PRId32` and fixed-width types in interfaces.
- **Some C++ costs come from the toolchain, not my code.** `crtbegin.o` adds about 140 B of text and 28 B of bss for destructor-at-exit and unwind-registration support, even with `-fno-exceptions` and a `main` that never returns.
- **Linking switched from `C` to `CXX`** as soon as the first `.cpp` was added. CMake chooses the linker language from the sources, so the C++ runtime support libraries now take part in the link.

## Open Items

- [ ] `CMakePresets.json` with `fw` and `host` presets
- [ ] Host test target: `format_fixed` and Unity, run with `ctest`. The test-case table (0, ±150, −5, −100, `INT32_MIN`, `INT32_MAX`, `frac_digits = 0`, `cap` too small) still needs hand-computed expected values
- [ ] Remove `sysmem.c` and rebuild to see whether anything references `_sbrk`. Repeat after dropping `-u _printf_float`
- [ ] Move the FPU enable (CPACR write) before `__libc_init_array`, into `SystemInit` or `Reset_Handler`, so global constructors can use floats safely
- [ ] Find out what the ARM linker does with a `bl` to the undefined weak `SystemInit`
- [ ] Port the drivers to C++ classes: `Bmp280`, `Mpu6050`, and `UartLogger` with an `etl::delegate` callback
- [ ] Decide on the observer mechanism (`etl::delegate` list inside each sensor, or something separate) and the events `SensorFsm` handles, such as sensor errors from the `status_t` returns that Week 17 ignores
- [ ] Settle the sequencer question: is it needed with a single 1 Hz rate?
- [ ] Choose the scale and type for the fixed-point values (milli-g and milli-deg/s with overflow checks; a bare `int32_t` or a unit-carrying type)
- [ ] Use the same compiler for the C-vs-C++ comparison: CubeIDE used GNU Tools for STM32 14.3, the CMake build uses Arm GNU 15.3.1. Report the fixed-point/printf change in a separate row from the C++ change
- [ ] Decide whether to keep the 512 B heap reservation in the linker script, given the no-heap design
- [ ] Write a `.clang-tidy` config, then run `cppcheck` and `clang-tidy`; document suppressions in `NOTES.md`
- [ ] Flash with `openocd` and document it in the build instructions
- [ ] Final README sections: class diagram, C vs C++ comparison table. Then `git tag v1.0-phase3-cpp` and the `NOTES.md` retrospective