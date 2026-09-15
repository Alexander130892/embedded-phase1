# Week 35 — Advanced C++: Templates + Compile-Time

## Goal

Theory review + hands-on exercises covering Ch.8–11 of Part III: templates and
CRTP, type safety, lambdas and `std::function`, and `constexpr`/`consteval`.
Goal was to build the embedded C++ pattern vocabulary needed before ETL
(deferred to Week 36) and to ground every claim in real compiler/linker/objdump
output rather than assumption — several claims turned out to be more subtle
than the book chapter summary suggested.

## Scope

- CRTP (compile-time polymorphism) vs. virtual dispatch vs. function pointer
  tables — when each is actually applicable
- Nominal typing and `explicit` constructors as the mechanism behind strong
  types (`SensorReading<Unit>`)
- `static_cast` (value conversion) vs. `reinterpret_cast` (bit reinterpretation)
  and why raw `reinterpret_cast` on a `float` value doesn't even compile
- Lambda capture semantics (by value vs. by reference) as compiler-synthesized
  closure structs
- `std::function` Small Buffer Optimization (SBO) and the non-reentrancy
  danger of heap allocation inside an ISR
- `constexpr` vs. `consteval` — optional vs. mandatory compile-time evaluation,
  and that `consteval` only applies to functions, never variables
- Compile-time sine table generation via a hand-written Taylor series with
  range reduction, verified to land in `.rodata` (Flash) via `objdump` on the
  real `arm-none-eabi-g++` / Cortex-M4 toolchain

## Architecture

Nothing runs on target this week — all exercises are host/cross-compiled
standalone `.cpp` files exercising language mechanics, verified via compiler
output, `static_assert`, `objdump`, and `size` rather than on-device testing.

```
Exercise 1: SensorReading<Unit>
  Celsius{} / Pascal{}  (empty tag types)
        |
        v
  SensorReading<Unit>  (template, holds float, explicit ctor, member operator+)
        |
        +-- SensorReading<Celsius> + SensorReading<Celsius>  -> compiles
        +-- SensorReading<Celsius> + SensorReading<Pascal>   -> compile error
        +-- SensorReading<Celsius> bad = 20.0f;              -> compile error

Exercise 2: constexpr sine LUT
  factorial(n)              [constexpr]
        |
  mySin(x)                  [constexpr, 4-term Taylor series, accurate only near 0]
        |
  constexprFmod(x, m)       [constexpr, hand-written — std::fmod not constexpr pre-C++23]
        |
  sinReduced(x)              [constexpr, folds x into [0,2*PI), quadrant symmetry -> mySin]
        |
  buildSinLUT()               [constexpr, 256-entry table via sinReduced()]
        |
  g_sinTable  ->  arm-none-eabi-g++ -O2  ->  objdump -h  ->  lands in .rodata (1024 B), 0 B .data/.bss

Exercise 3: UART dispatch — function pointer vs. std::function
  commands[]  { name, handler }
        |
        +-- void(*)(const char*)                 -> 319 B .text (ARM), 0 B .bss, no libstdc++ needed
        +-- std::function<void(const char*)>      -> requires <functional> (libstdc++) —
                                                       NOT AVAILABLE on this bare
                                                       arm-none-eabi-g++ toolchain
                                                       (host comparison: ~2.1x .text, +120 B .bss,
                                                        0 heap allocations — fits SBO)
```

## Key design decisions

**Tag types over storing unit info at runtime.** `Celsius`/`Pascal` are empty
structs used purely as template parameters — the compiler distinguishes
`SensorReading<Celsius>` from `SensorReading<Pascal>` via C++'s *nominal*
typing (name/identity, not structural shape), and `explicit` on the
constructor closes the one loophole (implicit `float` construction) that
would otherwise silently defeat the whole scheme.

**Hand-written Taylor series over `std::sin` in a `constexpr` context.**
Verified directly: `constexpr float val = std::sin(1.0f);` compiles under GCC
(even under `-pedantic-errors`) but fails identically under Clang with
`__builtin_sinf` flagged as "not valid in a constant expression." That's a
non-standard GCC extension, not a portable guarantee — even though the actual
target toolchain (`arm-none-eabi-gcc`) is GCC-based, portability was judged
worth the extra work of writing a standard-conforming `constexpr` sine
instead of relying on the extension.

**Range reduction before the Taylor series, not a higher-order polynomial.**
A raw 4-term series produces catastrophically wrong values beyond ~2 radians
(measured: `mySin(6.0)` returned `-20.7` against a real `sin(6.0)` of
`-0.28` — not even in the valid `[-1, 1]` range). Folding the input into
`[0, 2*PI)` and using quadrant symmetry to keep the series argument within
`[-PI/2, PI/2]` fixed this with the same 4-term polynomial, verified to match
`std::sin()` within ~0.0001 at several test points including near the `PI/2`
boundary.

**Kept the Phase 1 function pointer table over refactoring to
`std::function`.** Measured, not assumed: on the host toolchain, the
`std::function` version needed roughly 2.1x the `.text` size and introduced
120 bytes of `.bss` for constructor/destructor machinery the plain table
doesn't need. No handler in `parse_command` captures per-command state (the
shared globals `g_duty_cycle`/`g_CCR` would have to stay global regardless),
so `std::function`'s only real advantages — binding extra context, storing
heterogeneous captures — are unused here. On top of the size cost,
`std::function` requires `<functional>` (libstdc++) to even be present on the
target; this container's bare `arm-none-eabi-g++` doesn't have it at all,
which is itself a dependency-footprint argument independent of code size.

## Lessons Learned

- **`const` vs. `constexpr` is about a guarantee, not a possibility.**
  `const` only promises "never modified after init" — the initializer can
  still be an arbitrary runtime expression. `constexpr` demands the
  initializer be evaluable *at compile time*, which is why `constexpr float
  val = std::sin(1.0f);` is only accepted because GCC treats `std::sin`'s
  underlying `__builtin_sinf` as compiler-foldable — a GCC-specific
  extension, not standard C++ (confirmed failing under Clang with the
  identical code).
- **`consteval` applies only to functions, never variables** —
  `consteval int x = 5;` is a hard compile error
  (`a variable cannot be declared 'consteval'`), confirmed directly.
- **CRTP requires the concrete type to be knowable in source, not from
  runtime data.** `Base<ButtonHandler>` and `Base<TimerHandler>` are
  unrelated types with no common non-templated ancestor — there is no
  `std::vector<Base<???>*>` that can hold both. A command ID arriving over
  UART at runtime is exactly the case CRTP cannot substitute for; that's
  what virtual dispatch / function pointer tables exist for.
- **Type punning via `reinterpret_cast` on a value (not a pointer) doesn't
  even compile.** `reinterpret_cast<int>(f)` for a `float` `f` is illegal —
  `reinterpret_cast` reinterprets bit patterns through pointers/references,
  it doesn't convert values the way `static_cast` does. The correct (but
  still UB-under-strict-aliasing) pattern is `*reinterpret_cast<int*>(&f)`;
  the *legal* modern replacement is `std::bit_cast` or `memcpy`.
- **Lambda captures are just compiler-synthesized struct members.** `[x]`
  copies `x` into a hidden member at the moment of capture — confirmed by
  printing `10` even after mutating the original `x` to `99` post-capture.
- **`std::function`'s SBO threshold is a real, measurable line, not a
  vague "small captures are fine."** Instrumenting `operator new` directly
  showed 0 allocations for up to 4 captured `int`s (16 bytes) but a heap
  allocation at 8 captured `int`s (32 bytes) under libstdc++. Heap allocation
  inside an ISR is dangerous specifically because `malloc`/`new` are
  non-reentrant — an ISR preempting a half-finished heap mutation from
  main-line code corrupts the allocator's internal free-list structure.
- **A container's toolchain isn't necessarily the real target toolchain.**
  This session's `apt`-installed `arm-none-eabi-g++` ships no `newlib` and no
  `libstdc++` at all — not even `<string.h>` or `<functional>` are available.
  Worth checking whether the actual Homebrew-based toolchain used for the
  real project differs before assuming any conclusion here transfers
  directly (the SBO/allocation behavior does transfer, since it comes from
  libstdc++'s `std::function` implementation shared across host and target
  builds of the same GCC family; the *availability* of libstdc++ on-target
  does not transfer and needs separate verification).

## Open Items

- ETL (Ch.17) preview — read what replaces `std::vector`/`std::string` in
  embedded C++ — deferred to Week 36
- CRTP exercise (refactor a fixed-type-known-at-compile-time driver, e.g. a
  `GpioDriver`/`TimerDriver` pair, comparing code size against virtuals) —
  skipped as not clearly applicable to the current codebase; revisit if a
  genuine compile-time-fixed-type case shows up in later phases
- Verify whether the real Homebrew `arm-none-eabi-g++` toolchain (as opposed
  to this session's stripped `apt` package) actually ships `libstdc++`/
  `<functional>` for the target — needed before drawing any firm conclusion
  about `std::function`'s real on-device footprint, beyond the host-based
  numbers captured here
- Commit exercises to `week35/cpp_advanced/`