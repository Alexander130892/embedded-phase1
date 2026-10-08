# Week 37 — C++ Deliverable + Integration (`CppSensorLogger`)

## Goal

Rewrite the Phase 2 Week 17 STM32F446RE sensor logger (BMP280 over SPI, MPU-6050 over I2C, CSV over UART) in embedded C++. The C++ version has no heap, no exceptions and no RTTI, runs as a pure super-loop with no FreeRTOS, and is built with CMake instead of STM32CubeIDE.

Done so far:
- An IDE-independent CMake build with three presets: firmware Debug, firmware size-optimized, and host tests.
- Pinned language standards (C++20 / gnu11).
- `format_fixed`, which replaces `printf("%f")`.
- An interrupt-safe SPSC `ring_buffer`.
- The `Uart` driver class (`init`, polled TX, interrupt-driven RX, line assembly), which fixes three Week 17 UART bugs.
- 26 host unit tests (Unity + CTest).
- Measured flash and RAM costs for every new component.

Still to do: `bsp_init()` (GPIO pin mux), flashing and verifying the UART on hardware, the sensor driver classes, `SensorFsm`, the observer wiring, static analysis, and the `v1.0-phase3-cpp` tag.

> 🏁 **M19 checkpoint:** C++ sensor logger running on STM32. CMake build. Static analysis clean. ⏳ *In progress (build, test infrastructure, formatting and UART driver done; not yet on hardware)*

## Scope

- CMake cross-compilation: toolchain file, `CMAKE_SYSTEM_NAME Generic`, `CMAKE_CROSSCOMPILING`, presets, `CMAKE_CXX_STANDARD` / `CMAKE_C_STANDARD`, `compile_commands.json`
- Target scope: `PRIVATE`, `PUBLIC`, and `INTERFACE` (for header-only libraries)
- Multilib selection, CPU flags on the link line, embedded C++ flags (`-fno-exceptions -fno-rtti -fno-threadsafe-statics`)
- Section GC, linker scripts, map files, vector table, startup sequence, `.data` vs `.bss` vs `.init_array`
- Constant initialization: `constexpr` constructors, `constinit`, and why `reinterpret_cast` can't be a constant expression
- ISR ↔ `main` sharing: lost updates, the single-writer SPSC design, `volatile`, compiler reordering, `std::atomic_signal_fence`
- `extern "C"` linkage, `friend` access for an ISR, deleted copy operations
- Register-level driver design: register struct + `static_assert`, "construct cheaply, initialize explicitly"
- Fixed-point formatting, `char` signedness, casting through `uint8_t`
- Host unit testing with a vendored Unity, registered in CTest
- Measuring cost: `size`, `nm -S`, map files, static-archive linking semantics

## Architecture

### Project layout

```
CppSensorLogger/
├── CMakeLists.txt              ← targets, sources, linker script, map, tests, C++20/gnu11
├── CMakePresets.json           ← fw / fw-size / host
├── STM32F446RETX_FLASH.ld
├── cmake/arm-none-eabi.cmake   ← toolchain file: compiler + CPU flags
├── Inc/   fixed_format.hpp  ring_buffer.hpp  uart.hpp  systick.h
├── Src/   fixed_format.cpp  uart.cpp  main.cpp  systick.c  syscalls.c  sysmem.c
├── Startup/ startup_stm32f446retx.s
└── test/
    ├── test_fixed_format.cpp   ← 14 Unity tests
    ├── test_ring_buffer.cpp    ← 12 Unity tests
    └── unity/                  ← vendored: unity.c/.h, unity_internals.h, LICENSE.txt, VERSION.txt
```

### Build targets

```
 cmake --preset fw / fw-size  (ARM GCC 15.3.1)        cmake --preset host  (AppleClang 21)
             │                                                    │
 ┌───────────┴───────────┐                       ┌────────────────┴───────────────┐
 │ firmware.elf + .map   │                       │ test_fixed_format  test_ring_buffer │
 └───┬────────┬──────────┘                       └──────┬─────────────────┬─────────┘
     │ links  │ links                          links │                 │ links
     ▼        ▼                                       ▼                 ▼
 fixed_format  ring_buffer  ◄───────────────── fixed_format    ring_buffer (+ unity)
 (static lib)  (INTERFACE,                           (same sources, compiled twice)
               header-only)
```

`uart.cpp` is compiled into the firmware only. It touches hardware registers, so it has no host build.

### UART data flow

```
 USART2 RX pin ──► DR ──► USART2_IRQHandler (extern "C", vector table)
                              │  friend of Uart
                              ▼
                        Uart::on_irq()          ISR context — producer
                          read DR once (clears RXNE)
                          rx_.push(byte) ──fail──► dropped_ = dropped_ + 1
                              │
                    ring_buffer<char, 64>       head: written by ISR only
                              │                 tail: written by main only
                              ▼
 main loop ──► Uart::receive_line(out, cap)     main context — consumer
                  drain rx_ → line_[] (object owns partial line)
                  on '\r'/'\n': empty → skip | too long / > cap → discard | else copy → true

 main loop ──► Uart::send_string() ──► send_char(): poll SR.TXE (timeout) → DR
```

### Object lifecycle

```
 constinit Uart g_uart2{0x4000'4400u};   compile time: constexpr ctor stores address only
                                         → object image in .data, no .init_array entry
 Reset_Handler: copy .data → RAM         g_uart2 valid before main, no code ran
 main: clock_init() → bsp_init() → g_uart2.init()   hardware touched only here
```

## Key design decisions

**CMake instead of STM32CubeIDE; toolchain file vs `CMakeLists.txt`.** The build system is part of the deliverable. CPU and compiler facts go in `cmake/arm-none-eabi.cmake`, and project policy goes in `CMakeLists.txt`: the linker script, nano.specs, the map file, warnings, and the language standards. The optimization level comes from `CMAKE_BUILD_TYPE` in the presets. See the commit history for the step-by-step reasoning (`Generic`, gcc as the ASM driver, `STATIC_LIBRARY` try-compile, `MCU_FLAGS` on the link line).

**Pinned language standards for both languages.** `CMAKE_CXX_STANDARD 20` (extensions off) and `CMAKE_C_STANDARD 11` (extensions on, matching Week 17's `gnu11`) are set right after `project()`. Without them, each compiler uses its own default: ARM GCC 15 defaults to `gnu23` for C, while AppleClang uses `gnu17`. That would compile the same C files under different standards on the target and the host. `compile_commands.json` confirms `-std=c++20` and `-std=gnu11` in both build trees.

**Fixed-point `format_fixed` instead of `printf("%f")`.** It avoids newlib's float printf (a likely `malloc` path) and fixes the Week 17 negative-number bug. It handles the sign once on a `uint32_t` magnitude, so `INT32_MIN` causes no UB. It has a `snprintf`-style contract (null-terminated output, returns the length, writes `'\0'` on failure) and costs **206 B** of flash at `-Os`.

**`ring_buffer`: single-producer/single-consumer instead of a locked buffer with `count`.** The Week 33 C++ port had a `count` written by both `push` (in the ISR) and `pop` (in `main`), which is a lost-update race (`main` loads `count`, the ISR increments it, `main` stores a stale value). It also wrote `read_idx` from `push` when full. Instead of adding a critical section (option A: PRIMASK, blocking IRQs on every `pop`), I went back to Week 17's design (option B): no `count`, `head` written only by the ISR, `tail` only by `main`. Fullness uses the one-empty-slot rule `(head + 1) % N == tail`, so capacity is `N - 1`. When full, `push` **drops the new element** and returns `false`. This fixes the Week 17 bug where 64 unread bytes wrapped `head` onto `tail` and the full buffer looked empty.

**`volatile` indices plus `atomic_signal_fence` instead of `std::atomic`.** `volatile` stops the compiler from caching `head` in a polling loop. It doesn't order the *non-volatile* `arr` accesses around the indices, so `pop` could read a stale slot or read after handing the slot back. `std::atomic_signal_fence` (acquire/release) is a compiler-only barrier for an ISR on the same core, and it emits **no instructions**. `std::atomic` with acquire/release would emit `DMB`, which a single-core Cortex-M4 doesn't need. Indices are `uint32_t`: there's no 256-element limit, they're native register width, and an aligned 32-bit access is single-copy atomic on ARMv7-M. `static_assert(N >= 2)` blocks the degenerate sizes. `kN` converts `size_t` `N` once, because `size_t` is 64-bit on the host.

**Global `constinit Uart g_uart2` with a `constexpr` constructor.** The vector table can only call a plain function, so the ISR needs a fixed object. With a `constexpr` constructor and `constinit`, the object is **constant-initialized**: the compiler computes its bytes, it gets no `.init_array` entry, and it's valid before `main`. `constinit` makes that a compile error if it ever regresses. Because `reinterpret_cast` isn't allowed in a constant expression, the constructor stores the address as `std::uintptr_t`, and `regs()` converts it at use time.

**Construct cheaply, initialize explicitly.** The constructor runs at compile time, so it can't touch registers (it would also run before the clocks are configured). Hardware setup is in `Uart::init()`, which `main` calls after `clock_init()` and `bsp_init()`. GPIO pin muxing stays **out** of `Uart`, because which pins USART2 uses (PA2/PA3 or PD5/PD6) is a board decision.

**`friend` for the ISR instead of a public `on_irq()`.** `protected` would only open the member to derived classes, and a free function isn't one. `friend void ::USART2_IRQHandler(void);`, preceded by an `extern "C"` declaration, lets only the ISR call `on_irq()`. I verified that a call from `main` fails to compile (`'void Uart::on_irq()' is private`). That makes the single-producer rule compiler-enforced. The trade-off is that the class names its handler, so a second UART needs another `friend` line.

**`Uart` owns the partial line; oversized lines are discarded (option B).** Week 17's `uart_receive_string` assembled lines in the **caller's** buffer with a `static index` and checked `UART_MAX_LENGTH - 1` (63) instead of `maxlen` (50), so a long line overflowed `main`'s `input_str` on the stack. Now `line_`, `line_len_` and `line_overflow_` are members. `receive_line(out, cap)` drains `rx_`, copies only complete lines, bounded by `cap`, and throws away lines that are too long or don't fit instead of returning them truncated. A truncated command is a string the user never sent, and with arguments it could become a valid command with the wrong value. Empty lines (the `'\n'` of `"\r\n"`) are skipped, and the function returns one line per call.

**Register struct with `static_assert`.** `UsartRegs` replaces Week 17's `*((volatile uint32_t*)(BASE + OFFSET))`. `static_assert(sizeof(UsartRegs) == 0x1C)` catches a missing or extra field before it can shift every register. The members are `volatile` because the hardware changes `SR`/`DR`, and reading `DR` clears `RXNE`. The *address* is fixed, but the *value* isn't, so `constexpr` would be wrong.

**Kept the runtime `base_` member instead of a template address (measured, decided not to change).** Because `base_ = 0x40004400` is non-zero, all of `g_uart2` (152 B) goes in **`.data`**, which costs 152 B of RAM **and** 152 B of flash for its initial image, 148 bytes of which are zeros. With an all-zero object it would be in **`.bss`**: 152 B of RAM and **0 B** of flash. The idiomatic fix is `template <std::uintptr_t Base> class Uart`, which moves the address into the type (`Uart<0x4000'4400u>`). The compiler then encodes it as an immediate constant, so there's no flash image, no load of `base_`, and slightly smaller code. The costs: the whole class moves to the header, there's one code copy per instantiated peripheral, and the type names get more verbose. Setting `base_` in `init()` instead would reintroduce an invalid-until-initialized object. **I'm keeping the simpler design**: 148 B is 0.03% of the F446's 512 KB of flash. On a 16–32 KB part, or with many peripheral instances, the template version would be the right choice. That's why libraries like modm and Kvasir pass addresses as template parameters.

**Non-copyable `Uart`.** `Uart(const Uart&) = delete; operator=(const Uart&) = delete;`. A copy would be a second object driving the same hardware with its own RX buffer. Deleting the copy operations also suppresses the implicit move operations. Only the copy operations are deleted; the address constructor is unaffected.

**Host tests with a vendored Unity.** Pure logic (`format_fixed`, `ring_buffer`, and the `receive_line` algorithm) is tested natively with sanitizers. The repo builds offline, `VERSION.txt` records upstream commit `2b80d1a`, `add_test(NAME … COMMAND <target>)` lets CMake substitute the real executable path, and `ring_buffer` is an `INTERFACE` target because it has no `.cpp`.

## Measurements

All `-Os` (`fw-size`) unless noted, GCC 15.3.1.

| Build | text | data | bss | Δ |
|---|---|---|---|---|
| Baseline: empty `for(;;)` main (Debug) | 816 | 0 | 1568 | |
| + `format_fixed` called from `main` | 1032 | 0 | 1576 | `format_fixed` = **206 B** (`nm -S`), helpers inlined; bss +8 = `volatile g_sink` + `ALIGN(8)` pad |
| + `Uart g_uart2` (`constinit`) + `uart.cpp` | 1092 | 152 | 1576 | text **+60** (ISR path only), data **+152** (the object) |

**`g_uart2` = 152 B** on the 32-bit target: `base_` 4 + `rx_` 72 (64 + head 4 + tail 4) + `line_` 64 + `line_len_` 4 + `line_overflow_` 1 (+3 padding) + `dropped_` 4. It's in `.data` because `base_` is non-zero (see the design decision above). There's no `.init_array` growth.

**text +60** is only `USART2_IRQHandler` → `on_irq()` → `rx_.push()`, all inlined together. That code is kept because the vector table references the handler. `init`, `send_*` and `receive_line` aren't called yet, so `--gc-sections` removed them.

**Baseline breakdown:** text 816 = vector table 452 + crtbegin 140 + `Reset_Handler` 80 + `__libc_init_array` 72 + `SysTick_Handler` 28 + `main` 6 + rest. bss 1568 = heap 512 + stack 1024 + `.bss` 32.

## Lessons Learned

### Toolchain and CMake
- `gcc` on macOS is AppleClang targeting `arm64-apple-darwin`. The target triple is `arm-none-eabi`, and the executable is triple + tool.
- Flags need their leading `-`. Negative `-f` options are `-fno-<name>`. CPU flags must reach the **linker** too, or the driver links the default multilib (ARM state, soft-float).
- `--gc-sections` needs `-ffunction-sections` to discard anything useful. The vector table survives only because of `KEEP()`.
- `CXX` is CMake's name for C++. `${PWD}` is an empty CMake variable. `-T` passes a linker script, `-L` adds a library path, and `-Map` needs `-Wl,`.
- Use one build tree per compiler (the compiler is cached in `CMakeCache.txt`). A target defined in one `if` branch doesn't exist in the other: linking `unity` into `firmware` would have become `-lunity`.
- `CMAKE_CXX_STANDARD` and friends only apply to targets created **after** they're set, so put them right after `project()`. **Without `CMAKE_C_STANDARD`, the C files had no `-std` at all**, so GCC 15 and Clang compiled them as different C standards. I expected `compile_commands.json` to show a C `-std` flag, and it showed none.

### C / C++ language
- The Week 17 `"%ld.%02ld"` formatting breaks for negatives (`-150` → `-1.-50`, `-5` → `0.-5`). Negating `INT32_MIN` as a signed value is UB, so go through `uint32_t`. A `while` digit counter returns 0 for zero, so use `do`/`while`.
- `std::size_t` comes from `<cstddef>`, not `<cstdint>`. A transitive include may hide that on one compiler and not the other.
- **A `constexpr` constructor runs at compile time**, so it can't write hardware registers (`volatile` accesses aren't constant expressions). And **`reinterpret_cast` isn't allowed in a constant expression**: `constinit Uart g{reinterpret_cast<UsartRegs*>(addr)}` doesn't compile, so store `uintptr_t` and convert at use.
- **Without `extern "C"`, the C++ compiler mangles `USART2_IRQHandler`**, the `.weak` symbol in the startup file is never overridden, and the interrupt silently goes to `Default_Handler`.
- **`protected` grants access to derived classes, not to free functions.** `friend` is the tool for "this one outside function". The `extern "C"` declaration has to come before the class, or the `friend` line declares a new mangled function.
- **`volatile` is about the value, not the address.** Register members are `volatile` because the hardware changes them and reads have side effects. Without it, a `TXE` poll gets hoisted into an infinite loop, back-to-back `DR` writes get merged, and an unused `DR` read gets removed, leaving `RXNE` set.
- **`volatile` only orders volatile accesses relative to each other.** An ordinary `arr[tail]` load can move across a volatile `head`/`tail` access. On the ISR side this is harmless (an ISR runs to completion before `main` resumes). On the `main` side it can return a stale byte, so it needs fences.
- **`dropped_++` on a `volatile` is deprecated in C++20** (`-Wvolatile`); write `dropped_ = dropped_ + 1`. Bitwise compound assignment (`|=`, `&=`) was un-deprecated, and GCC doesn't warn on `regs().CR1 |= …`.
- **Templates defined in a header are fine; globals and non-inline functions aren't.** The Week 33 `rb`/`kick()` measurement harness in `ring_buffer.hpp` would have caused `multiple definition` errors as soon as two `.cpp` files included it.
- **`uint8_t` indices silently wrap at 256**, which corrupts a `ring_buffer<char, 300>`. And `std::size_t` is 64-bit on the host: `(head + N - tail) % N` narrowed to `uint32_t` only produced `-Wconversion` warnings there.
- **`{}` means value-initialization** (zero for scalars). Use it on every member the constructor doesn't set, and on locals such as `char c{}`, so nothing is ever read uninitialized. `base_` has none because the constructor always sets it.
- **Copy constructor vs copy assignment:** `Uart u2 = g;` and pass-by-value use the copy constructor, while `u2 = g;` uses copy assignment. Deleting both makes `Uart` reference-only.
- **`kName` and the trailing `_`** are Google-style conventions (constants that can't collide with `ALL_CAPS` macros, and private members). They aren't language rules.
- **Digit separators** (`0x4002'3800u`, `100'000`) are C++14 and ignored by the compiler. They aren't available in the `gnu11` C files.

### Driver code
- **`reg(addr)` vs `regs()`:** `reg` takes an address and returns one register. `regs()` takes nothing and returns the `UsartRegs` struct, so you access fields as `regs().CR1`. Writing `regs(BRR)` and `g_uart2.BRR` were both wrong. Inside a member function, use *this* object's `regs()`, never the global, or a second instance would reconfigure USART2.
- **`UE` last isn't required on the F4.** RM0390 §25.3.2 (as far as I recall; check it) sets `UE` first and writes the configuration bits afterwards. "Configure, then enable" is still a portable habit, because newer families (F7, L4, G0, H7) write-protect `M`/`OVER8` while `UE = 1`. An early RX interrupt can't happen anyway, since `RXNE` needs both `RE` and `UE` and an incoming byte.
- **`char` → register: cast through `uint8_t`.** `char` is unsigned on ARM and signed on the Mac host, so `'é'` (0xE9) widens to `0xFFFFFFE9` on the host. `static_cast<std::uint8_t>(c)` pins the raw byte on both. On RX, the mirror: `static_cast<std::uint8_t>(regs().DR)`, then `static_cast<char>`. Reading `DR` into a `const` local makes the single read (which clears `RXNE`) visible.
- **`s` vs `*s`:** `while (s != '\0')` compares the pointer (GCC warns), and `send_char(s++)` passes the pointer. The idiom is `send_char(*s++)`, and you must check and propagate its return value.
- **`out = line_;` copies nothing.** `out` is a local copy of the caller's pointer. You have to write *through* it (`memcpy(out, …)`, then `out[len] = '\0'`).
- **Reset state unconditionally at every line end.** The first `receive_line` cleared `line_overflow_` but not `line_len_`, so a rejected line's characters would have prefixed the next one. The cleaned-up version reads `discard`/`len` into `const` locals, resets once, then acts, and uses an early `continue` for ordinary characters instead of an `if / else if` chain.

### Testing and measuring
- On macOS's case-insensitive filesystem, `test/unity/VERSION` shadowed the standard header `<version>` through `-I`. Don't put extensionless files in include directories.
- Make a new test fail on purpose first. `UNITY_END()` returns the failure count, and that exit code is what CTest and CI read. CTest counts **executables**, not Unity tests (use `-V`). `ctest` doesn't build, so always run `cmake --build … && ctest …`.
- **The compile succeeded but the link failed with `_main` undefined**: the test file on disk was truncated. An empty `.cpp` compiles without errors. Then `no member named 'capacity'` meant the test was compiling against the old header.
- Linking a static archive adds nothing until something references a member. Use a `volatile` sink to keep a measured call alive under LTO. Debug sizes aren't costs. Measure single functions with `nm -S --size-sort -C`.
- **`.data` costs RAM *and* flash; `.bss` costs RAM only.** One non-zero member puts a whole object, zeros included, into `.data`.

## Open Items

- [ ] `bsp_init()`: GPIO PA2/PA3 alternate function AF7 (port Week 17's `uart_gpio_init`)
- [ ] Call `g_uart2.init()` from `main`, echo `receive_line()` back over `send_string()`, and flash with `openocd` (first hardware test of the C++ UART)
- [ ] Measure the full UART cost once `init`/`send_*`/`receive_line` are called (they're currently GC'd)
- [ ] Host test for `receive_line()`: the algorithm is hardware-free, but `rx_` is private. Decide how to feed it (a test seam, or splitting line assembly into its own class)
- [ ] Read RM0390 §25.3.2 to confirm the USART enable sequence, and update the `init()` comment
- [ ] Optional: a `nullptr` guard in `send_string()`
- [ ] Remove the `volatile g_sink` harness from `main.cpp` once the UART consumes `format_fixed` output
- [ ] Port the sensor drivers to classes (`Bmp280`, `Mpu6050`); `UartLogger` with an `etl::delegate` callback; observer mechanism; `SensorFsm` events (including sensor errors from the `status_t` returns that Week 17 ignored); decide whether a sequencer is needed
- [ ] Choose the fixed-point scales and types for accel and gyro (milli-g, milli-deg/s)
- [ ] Remove `sysmem.c` and rebuild to see who references `_sbrk`; decide on the 512 B heap reservation
- [ ] Move the FPU enable (CPACR) before `__libc_init_array`; find out how the linker resolves the `bl` to the undefined weak `SystemInit`
- [ ] C-vs-C++ size comparison on the same compiler (CubeIDE 14.3 vs Arm GNU 15.3.1); Week 17 `snprintf` + `-u _printf_float` vs the 206 B `format_fixed`
- [ ] Warnings on `firmware`'s own sources; host-test flags (`-funsigned-char`, sanitizers in the `host` preset)
- [ ] Optional: `"inherits"` in presets; `testPresets` so `ctest --preset host` works
- [ ] `.clang-tidy` config, then `cppcheck` + `clang-tidy`; document suppressions in `NOTES.md`
- [ ] Final README: class diagram, C vs C++ comparison table, `git tag v1.0-phase3-cpp`, `NOTES.md` retrospective