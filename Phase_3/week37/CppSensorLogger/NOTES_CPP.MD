# NOTES_CPP — Week 37 retrospective (`CppSensorLogger`, Phase 3)

Retrospective on rewriting the Week 17 C sensor logger in embedded C++ (no heap, no exceptions, no RTTI, super-loop, CMake).
Details, measurements and the full lessons list are in `README.md`; this file is the short reflection.

## Verdict: C vs C++ for this project

**C++ can be considered safer, if you apply the correct practices.** The safety didn't come from "using C++". It came from specific tools, each of which removed a class of Week 17 bug:

| C++ tool | Week 17 problem it prevents |
|---|---|
| `enum class BusStatus` + `[[nodiscard]]` | `status_t` returned and then ignored (`mpu6050_init` was never checked) |
| `enum class State` + `switch` without `default` | comparing a state with an event; forgetting a state (`-Wswitch`) |
| constructors + reference members | an object used before its `init()` was called |
| `= delete` on copy | two objects driving the same peripheral |
| concepts (`I2cBusLike`, `SensorHw`) | hardware-coupled code that can't be tested without the board |
| `constinit` | a global that silently needs startup code |

Without those practices, C++ is just C with more syntax.

**The cost is syntax.** C++ code is more syntax-heavy: templates, concepts, rule-of-five declarations, `static_cast`s and attributes. That's the price I noticed.

**Memory, size and speed: no measured cost in the parts I checked.**
- `constinit Uart` → no `.init_array` entry, no startup code
- `etl::delegate` = 8 B on the M4, no heap
- `std::atomic_signal_fence` emits no instructions
- templates and concepts resolve to direct calls, with no vtables
- the complete logger is **5276 B text / 152 B data / 1600 B bss**, about 1% of the F446's flash

*Not yet verified:* a like-for-like size comparison with the Week 17 C build on the same compiler, any speed measurement, and worst-case stack usage. Until those are done, "no cost" is "no cost found", not "no cost".

## What I found most interesting

**Separation of concerns, and templating for host vs firmware.** The same `Mpu6050<Bus>` and `SensorFsm<Hw, N>` code runs:
- on the board, with `I2c1Bus` / `BoardHw` (real registers and Week 17 C drivers)
- on the Mac, with `FakeBus` / `FakeHw` (scripted results and call counters)

The FSM was finished and covered by 21 tests, one per transition-table row, before it ever touched hardware. On the board it then worked on the first full run, including recovery from an unplugged SDA line.

The same separation exposed a mistake: `bus.hpp`, the generic interface, still included the C `i2c.h` driver. That's a dependency pointing the wrong way, and clang-tidy surfaced it indirectly as 748 findings in STM32 headers that had no business being in the host build.

## Decisions I made this week

- **Mixed C/C++ instead of a full rewrite.** The Week 17 C drivers (BMP280, SPI, I2C, systick) stay, behind `extern "C"`. I changed existing code as little as possible: no C++ wrapper for the BMP280.
- **Option B for the observer.** The FSM publishes one `LogRecord` per tick with per-sensor `valid` flags, instead of one subject per sensor with the join pushed into the logger.
- **Pure `format_csv` + thin `UartLogger`**, instead of templating the logger over a sink.
- **Timeout → Recovering, Nack → stay Logging; `LogOn` in Recovering stays Recovering.**
- **Static analysis stopped at "known and documented".** cppcheck is clean on both builds, and clang-tidy production code is down to 7 accepted findings. Every exception has its reason in `cppcheck-suppressions.txt` or `.clang-tidy`.

## Things that went wrong (and what they taught me)

- **My own Week 17 `i2c_bus_reset` had four bugs** (SCL/SDA swapped, lines driven low on entry, pins never returned to AF4, no real STOP) and had probably never run. Code that's never exercised isn't verified, however long it has existed.
- **A dead slave can take the whole bus down** when the pull-ups sit on its board: unplugging the MPU's VCC gave Timeout, not Nack.
- **`be16` had a signed-promotion shift**, which is UB on 16-bit-`int` targets, in the same week I widened types "for AVR portability".
- **Tooling traps:** `build/` tracked in git, `main.cpp` dropped from the source list (with a stale `.obj` hiding it), a library built but not linked, a C header with `std::` in it, cppcheck suppressions broken by leading spaces and by a path pattern, and a clang-tidy run that "found 0" because it never ran. In each case, check what the tool actually did before trusting its result.

## C vs C++: how I'd decide for a new application

The language features rarely decide it. These four questions do:

1. **What does the platform give me?** Vendor HALs, RTOS kernels and BSPs are almost all C. With C++, I live on an `extern "C"` boundary (fine, but permanent). On a small or exotic MCU, a good C++ compiler may not exist.
2. **What does certification demand?** Functional safety (ISO 26262, IEC 61508, DO-178C) needs a qualified toolchain and a coding standard: MISRA C, or MISRA C++:2023 / AUTOSAR C++14. C has the longest track record. C++ is accepted as a restricted subset, often exactly the one used here (no heap, no exceptions, no RTTI).
3. **Who maintains it?** C++ is only safer with the right practices. A strong C team writes better C than a weak C++ team writes C++.
4. **How big and long-lived is the application layer?** In a small driver, C++ adds little. In a large application (state machines, protocol stacks, many interchangeable devices, host-testable logic), type safety, RAII and compile-time interfaces pay off.

**The default I'd reach for: this week's architecture.** C at the bottom (vendor and driver code), restricted C++ for the application layer, and a deliberate boundary between them.

**Is the future C++?** Partly.
- **C stays.** Linux, Zephyr, FreeRTOS, vendor SDKs and decades of certified code are C, and rewriting working, certified code is costly and risky. Maintaining C will remain a real job.
- **C++ is established in larger embedded systems**, such as automotive (AUTOSAR Adaptive), industrial and medical application layers.
- **The real shift is toward memory safety, and there Rust is the challenger.** It's in the Linux kernel and Android, and government guidance (CISA/NSA) pushes memory-safe languages. Embedded Rust's ecosystem, certified toolchains and talent pool are still much smaller than C's. *(State of knowledge mid-2026; check current job postings in the Ghent region.)*

**The skill that matters is the combination:** read and fix legacy C, wrap it cleanly, build a testable modern layer on top, and know what each choice costs. In the Rust phase, `extern "C"` becomes FFI, and the boundary questions stay the same.
