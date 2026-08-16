# Week 33 — C++ for Embedded Systems: Toolchain, Zero-Cost Abstraction, Hardware Bring-Up

## Goal

Ch. 1–4 of *C++ in Embedded Systems* (Mahmutbegović). Stand up a native macOS
cross-toolchain for the STM32F446RE (Nucleo), verify the Ch. 1 zero-cost
abstraction claim by measuring a C vs. C++ ring buffer in real object code, and
carry a linked firmware image through Renode simulation to actual hardware.
Went beyond the book's exercises by taking the same image all the way to a
physical LED blink on the Nucleo board.

## Scope

- Cross-compilation with `arm-none-eabi-g++` (Cortex-M4F, hard-float ABI)
- CMake toolchain files for bare-metal cross-builds
- `constexpr`, templates, and compile-time vs. runtime object initialization
- `.text` / `.data` / `.bss` / `.init_array` as physical evidence, not theory
- Freestanding C++: no heap growth via `malloc`/`new` by default, `-fno-exceptions -fno-rtti`
- Startup code, vector tables, linker scripts, `__libc_init_array`
- Renode simulation of an STM32F4-class CPU and GPIO peripheral
- Real hardware flashing via OpenOCD + ST-LINK

## Architecture

```
constexpr ring_buffer<uint32_t,16>  (main.cpp)
            |
       arm-none-eabi-g++  (cortex-m4.cmake toolchain file)
            |
   +--------+---------------------------------+
   |        |              |                  |
startup_*.s system_stm32f4xx.c  syscalls.c   sysmem.c
(vectors,   (SystemInit:        (_read/_write/ (_sbrk —
 Reset_     HSE->PLL->180MHz,   _exit/_kill/   real heap,
 Handler,   flash wait-states,  etc. — real,   bounded by
 .data/.bss FPU enable)         not stubs)     linker syms)
 copy loop)
   |
   +----------------> linked via STM32F446RETX_FLASH.ld
                       (FLASH 512K @ 0x08000000,
                        RAM 128K @ 0x20000000)
                                |
                                v
                         firmware.elf
                          /          \
                 Renode (sim)     OpenOCD + ST-LINK
                 stm32f4_disc-    (real Nucleo-F446RE,
                 kit.repl,        LD2 / PA5)
                 externalLed
                 on gpioPortA
```

`main()` touches the ring buffer once (push/pop) to force real instantiation,
enables the GPIOA clock, configures PA5 as output, and toggles it via BSRR
set/reset in an infinite loop with a busy-wait delay.

## Key design decisions

**`bool pop(T* out)` over sentinel returns.** The original C `rb_pop`
returned `0` on empty, which is indistinguishable from a legitimately stored
`0`. Standardized both the C and C++ implementations on an out-pointer +
bool-success contract so the size comparison wasn't secretly measuring a
signature difference, and so neither language carries the ambiguity forward.

**`operator[]` over `std::array::at()`.** `.at()` bounds-checks and throws
`std::out_of_range`. Under `-fno-exceptions` that either fails to compile or
drags in unwind-table machinery, which would have made the C++ `.text`
comparison meaningless. `arr[idx]` matches the C version's unchecked access
and keeps the comparison honest.

**RINGBUFFERSIZE as a power of two (16, not 10).** `% N` on a power of two
collapses to `AND #15` on the M4; a non-power-of-two size forces a real
`udiv`. Both C and C++ pay the cost equally either way, but choosing 16 kept
the disassembly readable and matched a real embedded reflex (confirmed in
`objdump` output: `and.w r3, r3, #15`).

**Toolchain file, not the book's Docker container.** The book's container
crashed on Apple Silicon — a Mono-based Renode 1.15.0 JIT aborting under
amd64 emulation (`amd64_is_imm32` assertion), not the glibc-2.31 mismatch it
first looked like. Standardized on a native `arm-none-eabi-g++` +
native-arm64 Renode (via Homebrew) instead, since it's reusable for future
projects and eliminates the emulation layer entirely.

**`-specs=nosys.specs` → real `syscalls.c`/`sysmem.c`.** Started with
`nosys.specs` stubs to get past the first bare-metal link (`_write`, `_exit`,
etc. undefined). For the hardware port, switched to CubeIDE's real
`syscalls.c`/`sysmem.c`, which provide a working `_sbrk`-based heap bounded
against `_estack - _Min_Stack_Size`. The two are mutually exclusive — mixing
them risks duplicate-symbol or shadowing issues, so `nosys.specs` came out
entirely once the real files were in the build.

**Wrote `SystemInit` by hand instead of skipping it.** No
`system_stm32f4xx.c` came with the startup files. Rather than leave
`SystemInit` as its default `.weak` no-op (chip stays at 16 MHz HSI, 0 wait
states), wrote the real HSE→PLL→180 MHz sequence, matching the ordering that
matters: flash latency raised *before* the `SYSCLK` switch, PLL lock
confirmed before switching, FPU enabled via `CPACR` before any float code
could run.

## Lessons Learned

- **`constexpr ring_buffer() = default;` silently fails to be constexpr if
  any member isn't constant-constructible.** `std::array<T,N> arr;` with no
  initializer left elements indeterminate, which is not a constant
  expression — so the compiler quietly fell back to an ordinary runtime
  constructor. No warning, no error. First build showed identical
  `.data`/`.init_array` between the "constexpr" and runtime-ctor variants
  (both 4 bytes / present) — only `arr{}` (value-initialization) fixed it.
  The only way to know `constexpr` actually took is to check the object
  file, not the source.

- **The proven zero-cost result: `.text` 84 (C++, constexpr, inlined) vs.
  122 (C, same logic as free functions), `.bss` 68 in both, `.data`/
  `.init_array` 0 in the constexpr version vs. 4 bytes + a real
  `.init_array` entry in the runtime-ctor version.** C++ wasn't just
  "equal" to C — it came in under, because `-Os` inlined the small member
  functions into their single caller, something the ordinary externally-
  linked C functions couldn't get for free. The *only* place C++ can cost
  something is a static object's non-constexpr constructor — and that cost
  is exactly one `.init_array` entry, not vague overhead.

- **A C struct's whole file can size to `.text`-only with `.bss` = 0.**
  Functions are emitted unconditionally in C (external linkage, can't prove
  no callers exist). Template member functions are lazily instantiated in
  C++ — with no forcing caller, a `static` instance and its methods can be
  entirely optimized away, producing a misleadingly tiny (10-byte) size
  reading. Fixed by forcing every method through an externally-linked
  `kick()` function and making the instance non-`static`, matching C's
  external-linkage guarantee.

- **`.fpu softvfp` in a CubeIDE-generated startup `.s` silently contradicts
  a hard-float toolchain.** The assembler directive doesn't override
  project-level `-mfpu=fpv4-sp-d16 -mfloat-abi=hard` flags passed on the
  command line — but it's a reminder that generated startup files assume
  the build system supplies the real ABI, not the file itself.

- **`-nostartfiles` and `__libc_init_array` conflict.** `Reset_Handler`
  calls `bl __libc_init_array`, which itself calls `_init()` — normally
  supplied by `crti.o`/`crtn.o`. `-nostartfiles` excludes those files
  regardless of whether `nosys.specs` is also in use, producing "undefined
  reference to `_init`" / "dangerous relocation." Since the vector table's
  `Reset_Handler` already bypasses the generic C startup path entirely
  (it's the literal reset vector), `-nostartfiles` bought nothing and had
  to come out.

- **CMake `project(CXX ASM)` silently drops C.** Adding `system_stm32f4xx.c`,
  `syscalls.c`, `sysmem.c` to `add_executable()` did nothing — no error at
  configure, no error at build — because the project never declared `C` as
  a language. CMake happily configured, then only ever compiled the `.cpp`
  and `.s` sources, reproducing the exact same "undefined reference to
  `_exit`/`_kill`/`_sbrk`" link failure three separate rebuilds in a row.
  Diagnosed by counting compile-step percentages in the build log (`[33%]
  ... [66%] ... [100%] Linking` — only 2 sources, not 5) before checking the
  actual file on disk.

- **`add_executable(firmware ...)` doesn't produce `firmware.elf`.** The
  target name becomes the literal output filename with no extension unless
  told otherwise — `set_target_properties(firmware PROPERTIES SUFFIX
  ".elf")` is needed if tools downstream (OpenOCD in particular) expect the
  `.elf` suffix explicitly.

- **BSRR over read-modify-write on ODR.** BSRR's two halves (`BSy` bits
  0–15 set, `BRy` bits 16–31 reset) are independently write-only — a plain
  `=` write is correct and atomic, with no need to know or preserve the
  pin's current state. `ODR |=` can only ever set bits, never clear them,
  and requires read-modify-write, which is unnecessary and unsafe once BSRR
  is available.

- **Renode has no `nucleo_f446re.repl` in this version (1.16.1).** Used
  `stm32f4_discovery-kit.repl` instead — same Cortex-M4F core and identical
  GPIOA/RCC peripheral addresses within the F4 family, so firmware logic
  transfers correctly, but board-specific LED/button wiring differs
  (`externalLed` on `gpioPortA`, rather than the Nucleo's actual PA5/LD2
  labeling). Confirmed the toggle was real by polling `.State` across
  multiple reads rather than trusting the peripheral name alone.

## Open Items

- `SystemInit`'s PLLM/PLLN/PLLP values and the 5-wait-state flash latency
  were taken from standard reference-project values for 180 MHz @ 3.3V, not
  re-derived from RM0390 — worth a real reference-manual check if hardware
  behavior ever looks off (boot instability, wrong-feeling timing).
- No UART output yet — `__io_putchar`/`__io_getchar` are declared weak in
  `syscalls.c` but never defined; needed before `_read`/`_write` do anything
  useful.
- Ring buffer's index type (`uint8_t`) is hardcoded rather than sized to the
  template's `N` parameter — fine for `N=16`, not general.
- Renode GUI analyzer (`ShowAnalyzer`) has no backend in this session/build;
  LED state was confirmed via `.State` polling instead — not investigated
  further since polling was sufficient.