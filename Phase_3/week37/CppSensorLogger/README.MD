# Week 37 — C++ Deliverable + Integration (`CppSensorLogger`)

## Goal

Rewrite the Phase 2 Week 17 STM32F446RE sensor logger (BMP280 over SPI, MPU-6050 over I2C, CSV over UART) in embedded C++. The constraints: no heap, no exceptions, no RTTI, a pure super-loop with no FreeRTOS, and a CMake build instead of STM32CubeIDE.

The result logs one CSV row per second, recovers from a stuck I2C bus without a reset, and accepts `log on` / `log off` / `status` commands over the UART. The architecture is *mixed C/C++*: the Week 17 C drivers stay at the bottom (with `extern "C"`), and C++ classes, templates and concepts sit on top. A tick-driven `SensorFsm` publishes `LogRecord`s through an observer (`Subject` + `etl::delegate`) to a `UartLogger`.

**Status:**
- 80 host unit tests (Unity + CTest)
- verified on hardware, including SDA-disconnect recovery
- cppcheck clean on the firmware and host builds
- clang-tidy clean on production code (7 accepted exceptions)
- not yet tagged

> 🏁 **M19 checkpoint:** C++ sensor logger running on STM32. CMake build. Static analysis clean. ✅ *Running on hardware, CMake build, cppcheck clean; clang-tidy production code clean, test code and fw-only files not yet triaged. `v1.0-phase3-cpp` tag pending.*

## Scope

- **CMake cross-compilation:** toolchain file, `Generic` system, presets (`fw` / `fw-size` / `host`), `PRIVATE`/`PUBLIC`/`INTERFACE`, transitive usage requirements, `compile_commands.json`, vendored dependencies in `third_party/`
- **Embedded C++:** `-fno-exceptions -fno-rtti -fno-threadsafe-statics`, `constinit` + `constexpr` constructors, `enum class` with a fixed underlying type, `[[nodiscard]]`, deleted copy operations, rule of five, reference members and member initializer lists
- **Compile-time polymorphism:** templates + C++20 **concepts** (`I2cBusLike`, `SensorHw`) instead of virtual interfaces or function pointers
- **Mixed C/C++:** `extern "C"` guards, name mangling, keeping C headers valid C
- **ISR ↔ main:** SPSC ring buffer, `volatile`, `std::atomic_signal_fence`, `friend` for the ISR
- **Design patterns:** observer (`Subject<Event, N>` over `etl::delegate`), state machine (function per state), ports and adapters (`SensorHw` port, `BoardHw` / `FakeHw` adapters), pure core / thin shell (`format_csv` + `UartLogger`)
- **I2C:** Timeout vs NACK, 9-clock bus recovery, the effect of pull-up placement
- **Testing:** host tests with scripted fakes, one test per FSM table row, testing contracts (truncation, capacity, empty slots)
- **Static analysis:** cppcheck and clang-tidy, triage into fix / accept / false positive, documented suppressions

## Architecture

### Runtime data flow

```
                       ┌────────────── super-loop (main.cpp) ───────────────┐
 SysTick (1 ms) ──────►│ every 1000 ms (last += 1000) ──► fsm.handle(Tick)  │
 USART2 RX ISR ─► ring_buffer ─► Uart::receive_line ─► "log on/off/status"  │
                       │                                ──► fsm.handle(LogOn/LogOff)
                       └─────────────────────────────┬──────────────────────┘
                                                     ▼
                         ┌──────────── SensorFsm<BoardHw, 2> ────────────┐
                         │  Init ─► Logging ◄──► Idle                    │
                         │            ▲  │ Timeout                       │
                         │            │  ▼                               │
                         │          Recovering ─► Idle                   │
                         └──────┬───────────────────────────┬───────────┘
              SensorHw port     │                           │ publish(LogRecord)
         (concept, 5 functions) ▼                           ▼
 ┌──────────────── BoardHw (main.cpp) ──────────┐   Subject<LogRecord, 2>
 │ imu_init/imu_read → Mpu6050<I2c1Bus>  (C++)  │        │ etl::delegate
 │ env_read  → bmp280_spi_read_*()   (Week 17 C)│        ▼
 │ bus_reset → i2c_bus_reset()       (Week 17 C)│   UartLogger::on_record
 │ now_ms    → systick_get_ms()      (Week 17 C)│        │ format_csv() (pure)
 └───────┬──────────────────────┬───────────────┘        ▼
         ▼                      ▼                   Uart::send_string ─► USART2 TX
   I2c1Bus ─► i2c.c        spi.c ─► bmp280_spi.c / bmp_core.c
   (I2C1, PB8/PB9)         (SPI1, PA5/6/7, CS PB6)
```

### FSM transition table

| # | State | Event | Guard | Action | Next |
|---|---|---|---|---|---|
| 1 | Init | `start()` | `imu_init` = Ok | — | Logging |
| 2 | Init | `start()` | `imu_init` ≠ Ok | — | Recovering |
| 3 | Logging | Tick | `imu_read` ≠ Timeout | read env + IMU, publish (Nack → `imu_valid = false`) | Logging |
| 4 | Logging | Tick | `imu_read` = Timeout | publish with `imu_valid = false` | Recovering |
| 5 | Logging | LogOff | — | — | Idle |
| 6 | Recovering | Tick | `bus_reset` && `imu_init` Ok | read env, publish env-only record | Logging |
| 6b | Recovering | Tick | recovery failed | read env, publish env-only record | Recovering |
| 7 | Recovering | LogOff | — | — | Idle |
| 8 | Idle | LogOn | — | — | Logging |
| 9 | Idle | Tick | — | nothing | Idle |
| – | Logging / Recovering | LogOn | — | — | same (Recovering does **not** skip the reset) |

### Project layout

```
CppSensorLogger/
├── CMakeLists.txt  CMakePresets.json  STM32F446RETX_FLASH.ld
├── .clang-tidy  cppcheck-suppressions.txt   ← every exception with its reason
├── cmake/arm-none-eabi.cmake
├── Inc/
│   ├── C++:  bus.hpp  i2c_bus.hpp  mpu6050.hpp  sensor_hw.hpp  sensor_fsm.hpp  subject.hpp
│   │         log_record.hpp  csv_format.hpp  uart_logger.hpp  uart.hpp  ring_buffer.hpp
│   │         fixed_format.hpp  bsp.hpp
│   └── C (Week 17, + extern "C"):  i2c.h  spi.h  bmp280.h  systick.h
│              (C-only, no guards):  bmp280_internal.h  status.h  stm32f446re.h
├── Src/  main.cpp  uart.cpp  bsp.cpp  fixed_format.cpp  csv_format.cpp
│         i2c.c  spi.c  bmp280_spi.c  bmp_core.c  systick.c  syscalls.c  sysmem.c
├── Startup/startup_stm32f446retx.s
├── test/  test_fixed_format (14)  test_ring_buffer (12)  test_mpu6050 (7)
│          test_subject (12)  test_csv_format (14)  test_sensor_fsm (21)      = 80 tests
└── third_party/
    ├── unity/  src/ + LICENSE.txt + VERSION.txt   (commit 2b80d1a)
    └── etl/    include/etl/ + LICENSE.txt + VERSION.txt   (ETL 20.49.0)
```

### CMake targets

| Target | Kind | Links | Used by |
|---|---|---|---|
| `fixed_format` | static lib | — | `csv_format`, firmware, tests |
| `csv_format` | static lib | `PRIVATE fixed_format` | firmware, `test_csv_format` |
| `ring_buffer`, `mpu6050` | INTERFACE | — | firmware, tests |
| `etl` | INTERFACE (`third_party/etl/include`) | — | via `subject` |
| `subject` | INTERFACE | `INTERFACE etl` | `sensor_fsm`, `test_subject` |
| `sensor_fsm` | INTERFACE | `INTERFACE subject` | firmware, `test_sensor_fsm` |
| `firmware` | executable (fw presets only) | `fixed_format ring_buffer csv_format sensor_fsm` | — |
| `unity` + `test_*` | static lib + executables (host only) | — | CTest |

### Week 17 (C) vs Week 37 (C++)

| Aspect | Week 17 | Week 37 |
|---|---|---|
| Build | STM32CubeIDE project | CMake + presets, toolchain file, `compile_commands.json` |
| Main loop | `snprintf` + busy-wait to the next second; `bool log_active` | `SensorFsm` (4 states); 1 s tick on a fixed grid, wrap-safe |
| Output formatting | `"%ld.%02ld"`: `-0.50 °C` printed as `0.-50`; float `printf` for the IMU | `format_fixed` / `format_csv`: fixed-point, sign handled once, `snprintf`-style contract, never truncated |
| Sensor → output coupling | `main` reads, formats and sends | observer: FSM publishes `LogRecord`, `UartLogger` subscribes; producers don't know consumers |
| Error handling | `status_t` returned, then mostly ignored (`mpu6050_init` unchecked) | `[[nodiscard]] enum class BusStatus`; per-sensor `valid` flags; Timeout → bus recovery, Nack → mark invalid |
| Failed sensor | garbage data or silent zeros | empty CSV fields, env logging continues, automatic recovery |
| UART RX | full buffer looked empty; `uart_receive_string` ignored `maxlen` (stack overflow) | SPSC `ring_buffer` (drop-new), `Uart` owns the line, oversized lines discarded |
| I2C bus recovery | present but 4 bugs (would lock the bus) | fixed, hardware-tested on the normal path |
| Polymorphism | none (direct calls) | templates + concepts, zero runtime cost |
| Tests | none | 80 host tests, including every FSM transition |
| Static analysis | none | cppcheck clean; clang-tidy production code clean |

## Key design decisions

**Mixed C/C++ instead of a full rewrite.** The BMP280, SPI, I2C and systick drivers are Week 17 C, unchanged apart from `extern "C"` guards and the `i2c_bus_reset` fix. The MPU-6050 became a C++ class over the C `i2c.c`. I deliberately did **not** wrap the BMP280 in a C++ class: change as little working code as possible. The asymmetry is intentional. The two BMP280 calls live in `BoardHw::env_read`, the one place in `main.cpp` that knows the hardware.

**Template + concept for the bus (`Mpu6050<Bus>`, `I2cBusLike`).** The choice was compile-time over runtime polymorphism. A virtual `IBus` would cost a vtable plus indirect calls, and a C struct of function pointers would block inlining. With a concept, `bus_.read_burst()` is a direct call, and a missing function fails at the point of use with a readable error. The price is one instantiation per bus type (`I2c1Bus` on the board, `FakeBus` in tests).

**Physical units in fixed point (`ImuSample` in mg / mdps, `EnvSample` in 0.01 °C / Pa).** The driver converts once (`to_mg`: widen to `int32`, multiply before dividing). Consumers only format. `be16` builds the 16-bit value from **unsigned** operands. See the clang-tidy lesson below.

**Observer with `etl::delegate` instead of `std::function` or virtual observers.** `etl::delegate` is two pointers (8 B on the M4, 16 B on the host): an object pointer and a compile-time-generated stub. That means no heap, no exceptions (`std::function` throws `bad_function_call`), and no base class for observers. `Subject<Event, N>` is a fixed array, and `[[nodiscard]] bool subscribe()` rejects empty delegates and a full list. `publish()` only calls the first `count_` slots, because an empty slot is a null function pointer (a HardFault).

**Option B: the FSM publishes one combined `LogRecord` per tick.** Option A (one subject per sensor) would have pushed the "join" into the logger: buffering, choosing when to print, stale data. The FSM already owns "one tick", so it builds the record. Per-sensor `env_valid` / `imu_valid` flags keep good data when the other sensor fails. Invalid sensors produce **empty** CSV fields (pandas → NaN, Excel → blank), never zeros.

**Pure core, thin shell for output.** `format_csv(out, cap, record)` is a pure function with the `format_fixed` contract (returns the length; when it doesn't fit it writes `""` and returns 0, never a truncated row). It's fully host-tested. `UartLogger::on_record` is two lines of glue. That's simpler than templating the logger over a sink type. The timestamp is printed as **unsigned**, because `format_fixed` takes `int32` and would go negative after 2³¹ ms (about 24.8 days).

**`SensorFsm<Hw, N>` behind a `SensorHw` port.** The FSM never calls a driver. Its five needs (`imu_init`, `imu_read`, `env_read`, `bus_reset`, `now_ms`) form a concept. `BoardHw` implements it with real drivers, and `FakeHw` with scripted results and call counters. Each state is its own function (`on_logging`, `on_idle`, `on_recovering`) that **returns** the next state. That fits a table where the guard depends on the action's result (read first, then decide). A data-driven table can't express that without extra machinery. `handle()` switches over `enum class State` without `default`, so `-Wswitch` catches a forgotten state. Each `on_*` has a `return` after its switch, so out-of-range enum values are defined behaviour.

**Timeout → Recovering, Nack → stay in Logging.** Timeout means the bus itself is stuck, so a bus reset can help. Nack means the bus works but nobody answered, and resetting every second wouldn't help. Recovery is attempted **once per tick**: no tight loop, and UART commands stay responsive. `LogOn` during Recovering stays in Recovering, because jumping to Logging would skip the reset.

**Tick on a fixed grid.** `if (systick_get_ms() - last >= 1000) { last += 1000; … }`. The unsigned subtraction survives the 49.7-day wrap, and `last += 1000` (instead of `last = now`) prevents drift. On hardware, every row ends in `…013` across the bus-reset ticks.

**Vendored dependencies with a namespace directory.** Unity moved from `test/` to `third_party/unity/src/`. All of ETL's `include/etl/` is vendored (ETL 20.49.0, commit `7d604f2`), rather than cherry-picking `delegate.h`, which pulls in 66 ETL headers. The include path points at `third_party/etl/include`, so code writes `"etl/delegate.h"`. ETL ships its own `string.h`, `limits.h`, `memory.h`, … that would otherwise shadow the C library's headers. Header-only means unused headers cost nothing in flash.

**Static analysis exceptions are recorded, not ignored.** `cppcheck-suppressions.txt` and `.clang-tidy` list every accepted finding with its reason. The narrowest available option was chosen each time (e.g. `AllowMissingMoveFunctionsWhenCopyIsDeleted` instead of `AllowMissingMoveFunctions`), and `NOLINT` is scoped to the named checks around `LineWriter`. With `--error-exitcode=1`, a *new* finding fails the run instead of drowning among known ones.

*(Earlier decisions are unchanged and described in the commit history: CMake toolchain split, pinned C++20 / gnu11, `format_fixed`, SPSC `ring_buffer` with signal fences, `constinit Uart` with a `constexpr` constructor, `friend` ISR, `Uart` owning the partial line, the register struct with `static_assert`, keeping the runtime `base_` member.)*

## Hardware results

| Test | Result |
|---|---|
| UART echo, oversized-line discard, burst input | ✓ |
| MPU-6050 WHO_AM_I | `0x68` (104) with the sensor connected; `Timeout` without it (no pull-ups → bus can't work) |
| IMU samples (uncalibrated) | \|a\| = 0.95–1.02 g in several orientations; gyro offset < 1–3 dps at rest |
| BMP280 | 19.71 °C / 1011.40 hPa (Fri), 1020.5 hPa (Thu, real weather change) |
| `i2c_bus_reset` (fixed) on a working bus | `imu` → `i2creset` → `imu` gives a valid sample |
| Full system | CSV at 1 Hz on a fixed grid; SDA unplugged → empty IMU columns, env still logged, `# state: Recovering`; SDA replugged → back to Logging automatically |
| MPU-6050 VCC unplugged | **Timeout, not Nack**: the module's on-board pull-ups lose power and the whole bus looks stuck |

## Measurements

`-Os` (`fw-size`), GCC 15.3.1.

| Build | text | data | bss | Note |
|---|---|---|---|---|
| Empty `for(;;)` main (Debug) | 816 | 0 | 1568 | vector table 452 B, stack 1024 + heap 512 |
| + `format_fixed` | 1032 | 0 | 1576 | `format_fixed` = 206 B |
| + `constinit Uart g_uart2` | 1092 | 152 | 1576 | object in `.data` (non-zero `base_`) |
| UART fully used + `bsp_init` | 1576 | 152 | 1568 | UART driver ≈ 480 B flash, 152 B RAM |
| **Final firmware** (UART + I2C + SPI + MPU-6050 + BMP280 + FSM + observer + CSV) | **5276** | **152** | **1600** | flash 5428 B (≈ 1 % of 512 KB); RAM 1752 B incl. 1024 B stack + 512 B heap reservation |

**Reading the final numbers:**
- **data stayed at 152 B**: `g_uart2` is still the only initialised global. The FSM, subject, logger, `Mpu6050` and `BoardHw` are **locals of `main`**, so they live on the stack and don't show up in `size` at all.
- **bss +32 B** compared with the UART build: Week 17's `comp_params` (24 B, BMP280 calibration), `tick_ms` (4 B) and alignment.
- **text +3700 B** for both sensor drivers, the I2C/SPI C drivers, the FSM, the observer, `format_csv` and the logger. No float formatting: `printf` and `-u _printf_float` aren't linked at all.
- **`size` doesn't measure stack usage.** The deepest path (Tick → `publish` → `on_record` → `format_csv` with a 128 B line buffer, plus the objects of `main`) has to fit in the 1024 B stack. That's not measured yet (open item).

## Lessons Learned

### Mixed C/C++ and linking
- **A C header that a `.cpp` includes needs `extern "C"` guards, and only those headers do.** `i2c.h`, `spi.h`, `bmp280.h` and `systick.h` got guards; `bmp280_internal.h` didn't, because only `.c` files include it. Without a guard, the call compiles but the linker looks for a mangled name. In practice: `systick.h` only needed guards once `main.cpp` became the first C++ caller.
- **Anything in a header shared with C must be valid C.** Putting `struct EnvSample { std::int32_t … }` into `bmp280.h` broke the C build (`expected specifier-qualifier-list before 'std'`). A type belongs with the code that produces it.
- **Read the linker error's symbol.** `format_csv(char*, unsigned int, LogRecord const&)` with a full signature means both sides are C++ and the **object file is missing** (the library wasn't in `target_link_libraries`). It's not a linkage-language problem. `undefined reference to 'main'` plus `g_uart2` meant `main.cpp` had dropped out of the source list, while a **stale `.obj`** in the build tree still showed `D g_uart2` in `nm`. CMake doesn't delete objects of removed sources.
- **An anonymous namespace or `static` gives internal linkage.** The ISR reaches `g_uart2` through `extern Uart g_uart2;`, so the global has to stay at file scope, outside `namespace { }`.
- **Templates and inline classes have no `.cpp`.** The firmware source list looks C-heavy, but most of the C++ is compiled through `main.cpp`'s includes.

### C++ language
- **Concepts are compile-time yes/no questions about a type.** The `requires(H h, …)` parameters are imaginary, and each `{ expr } -> std::same_as<T>` must compile and return exactly `T`. The benefit is an error at the point of use instead of deep inside the template.
- **`::` vs `.`:** `Type::f()` only works for `static` members. `Mpu6050<I2c1Bus>::init()` doesn't compile because `init()` works on *one* sensor (`imu.init()`).
- **Reference members can only be bound in the member initializer list.** There's no "empty then assign". The constructor makes an FSM without hardware impossible, unlike a C `init()` you can forget to call.
- **`= delete` on copy also suppresses moves.** Copying the FSM would give two state machines driving one bus. Reference members already delete copy *assignment* implicitly, but not the copy *constructor*.
- **`enum class : std::uint8_t`:** scoped names, no implicit `int` conversion (comparing a `State` with an `Event` is a compile error), fixed size on every platform. Unity compares integers, so assertion messages show `Expected 3 Was 1`, meaning Recovering vs Logging.
- **A `switch` over an `enum class` that returns in every case still needs a `return` after it.** An enum can hold any value of its underlying type. Falling off the end of a non-void function is UB, and GCC warns with `-Wreturn-type`.
- **Integer promotion:** in `be16`, `hi << 8` promotes `uint8_t` to **signed** `int`. That's harmless with 32-bit `int`, but `255 << 8` overflows a 16-bit `int` (AVR, MSP430) → UB for every negative reading. Cast to `unsigned` before shifting. clang-tidy's `bugprone-signed-bitwise` found it, in the same week I widened to `int32` "for AVR portability".

### Architecture
- **Dependencies point from concrete to abstract.** `bus.hpp` (the generic `BusStatus` + concept) still included `i2c.h`, a leftover from before the split. So every host test compiled the whole STM32 register map, which showed up as 748 clang-tidy findings in C headers. Removing one `#include` made them all disappear, without a config change. A per-*file* count of findings revealed it; a per-*check* count hid it.
- **Ports make the FSM testable**: 21 tests fire 100 ticks in microseconds, script "reset fails twice, then works", and count calls (`bus_reset` exactly once per tick, no `imu_init` after a failed reset thanks to `&&` short-circuiting).
- **When mapping status codes, unknown means failure.** `bus_reset()` first returned Ok for "anything except `I2C_TIMEOUT`". The safe form only treats `STATUS_OK` as success, the same rule as `to_bus_status`'s `default: → Unknown`.
- **A test that checks a sequence depends on several functions.** Row 8 (`Idle → LogOn → Tick`) stayed red after `on_idle` was correct, because the Tick then reached the still-empty `on_logging`. The failing line *moved*, which is progress.

### Hardware
- **Week 17's `i2c_bus_reset` had four bugs and had probably never run.** SCL and SDA were swapped (it watched PB8 = SCL and toggled PB9 = SDA). Switching to output with `ODR = 0` drove both lines low. The pins were never returned to AF4, so I2C was dead after any reset. And the manual STOP had no SDA-low phase. The SDA replug test passed *because* BUSY wasn't set, so the buggy path was never taken. The stuck-SDA path of the fix follows UM10204 §3.1.16 but is not verified on hardware.
- **Pull-ups on the slave's board mean a dead slave kills the bus.** Unplugging the MPU's VCC gave Timeout, not Nack: the module's 4.7 kΩ pull-ups lost their supply and the unpowered chip clamps the lines. In a product, put the pull-ups on the master's supply.
- **The BMP280's SPI pins keep their I2C labels** (SCL = SCK, SDA = MOSI, SDO = MISO). CSB must be wired before power-up, or the chip starts in I2C mode.

### Static analysis
- **A tool reporting 0 findings is a result to verify.** The first clang-tidy run "found 0" because `run-clang-tidy` couldn't find the keg-only Homebrew `clang-tidy` and I was in the wrong directory. The errors went to the file and `grep 'warning:'` counted nothing. Look at `head` before trusting a count.
- **Triage per check first, then per file, and de-duplicate:** a header finding is reported once per translation unit that includes it (61 findings → 9 unique lines in `mpu6050.hpp`).
- **Accepted ≠ ignored.** Every suppression has a reason. Examples: newlib fixes `_write`'s signature, defensive status checks on functions that return OK *today*, and interface methods staying non-static by design (the fakes have state). Pick the narrowest suppression available.
- **Suppression patterns match the reported path.** `*:*/third_party/*` didn't match `third_party/unity/src/unity.c` (no leading `/`). `*:*third_party/*` does. I tested it before trusting it.
- **Unity's macros trigger C++ checks in the test files** (C-style casts, `do {} while(0)`, `setUp(void)`). That's not test code to fix, but it needs its own scoped configuration.
- **cppcheck found one real issue the compiler didn't:** `idr` in `i2c_bus_reset` is read-only and should be `const volatile`. clang-tidy found two: the signed shift in `be16`, and the inverted include dependency (indirectly).

### Git and tooling
- **`.gitignore` doesn't untrack.** `build/` had been committed earlier and kept showing as modified until `git rm -r --cached build`. Commit that as its own change.
- **zsh doesn't treat `#` as a comment** in an interactive shell (`setopt interactivecomments`). `wc` got `# expect 443` as file names.
- **cppcheck's suppression file is whitespace-sensitive.** Indentation copied from a Markdown block turned the ID into `"   constParameterPointer"`.
- `git add -A` vs `git add .`: the same thing from the repo root since Git 2.0. `.` is limited to the current directory.

## Open Items

- [ ] `git tag v1.0-phase3-cpp` and the `NOTES.md` retrospective
- [ ] Compare the final size (5276 / 152 / 1600) with Week 17 on the same compiler (CubeIDE GCC 14.3 vs Arm GNU 15.3.1; `snprintf` + `-u _printf_float` vs `format_fixed`)
- [ ] Measure worst-case stack use (`-fstack-usage` per function, or fill the stack with a pattern at start-up and check the high-water mark) against the 1024 B `_Min_Stack_Size`
- [ ] clang-tidy on the firmware-only files (`main.cpp`, `uart.cpp`, `bsp.cpp`), which needs the ARM sysroot or a `--target` setup
- [ ] `test/.clang-tidy` (`InheritParentConfig: true`) that disables the Unity-macro checks; triage the remaining test findings
- [ ] Known clang-tidy exceptions in production code (7): pointer arithmetic + `do`/`while` in `LineWriter` (to be scoped with `NOLINTBEGIN/END`), one `else-after-return`
- [ ] Hardware: a real Nack test (AD0 → VCC while logging; check whether the MPU-6050 picks up the address change at runtime) and `log off` / `log on` on the board
- [ ] Hardware: verify the stuck-SDA recovery path (clocking SCL while a slave holds SDA low)
- [ ] I2C pull-ups on the master side (or enable the STM32 internal pull-ups as a fallback) so a dead slave can't take the bus down
- [ ] Gyro offset calibration (average at start-up); accel offset per axis (±50 mg datasheet tolerance)
- [ ] Optional: `Fault` state after N failed recoveries; `std::get<N>` for the constant `buf` indices in `Mpu6050::read`
- [ ] Optional: a readable `to_string(State)` in the FSM test assertions instead of numeric enum comparisons
- [ ] From the earlier list, still open: remove `sysmem.c` / decide on the 512 B heap reservation; FPU enable before `__libc_init_array`; warnings on firmware sources; `testPresets` for `ctest --preset host`; RM0390 §25.3.2 USART enable sequence; `receive_line` host test