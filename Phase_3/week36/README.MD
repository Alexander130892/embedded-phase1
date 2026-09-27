# Week 36 — Applied C++: HAL, Patterns + FSM

## Goal

Apply the C++ discipline built up in Weeks 33–35 (zero-cost abstractions, templates,
`constexpr`, no heap/exceptions/RTTI) to real embedded problems: a type-safe GPIO HAL,
wrapping existing C sensor drivers, a task sequencer, the Observer pattern for sensor
publishing, and finite state machines. Source: Mahmutbegović, *C++ in Embedded Systems*,
Ch.12–18.

## Scope

- Modeling memory-mapped peripherals as C++ structs with compile-time-verified layout
- Template-based, zero-cost bitfield access (`RegField<RegTag, Pin>`)
- `volatile` semantics and why the optimizer needs them for MMIO
- RAII for wrapping existing C driver lifecycles (`bmp280.c`, `mpu6050.c`)
- Adapter pattern for the transport layer (SPI/I2C), replacing `void* ctx` type erasure
- Sequencer pattern (cooperative super-loop) vs. FreeRTOS preemption — when each is safe
- Runtime Observer pattern for sensor-data publishing
- FSMs: switch-based vs. State pattern, applied to a BLE connection FSM
- SOLID (SRP, OCP, DIP) applied to the week's own code, including cases that looked
  like they satisfied a principle but didn't on closer inspection

## Architecture — `GpioPin` (Ch.12, built and verified this session)

```
GpioPin<PortBase, Pin>
    │
    ├── port() ──────────► GPIO_TypeDef& at PortBase
    │                       (volatile uint32_t members, offsets
    │                        static_assert'd against RM0390)
    │
    ├── set_mode(Mode m) ─► RegField<ModerTag, Pin>::set(port().MODER, m)
    │                       (read-modify-write, compile-time shift/mask)
    │
    ├── high() ──────────► port().BSRR = (1u << Pin)         [atomic, no read]
    ├── low()  ──────────► port().BSRR = (1u << (Pin + 16))  [atomic, no read]
    │
    └── read() ──────────► (port().IDR & (1u << Pin)) >> Pin

RegField<RegTag, Pin>
    shift      = RegTag::width * Pin
    field_mask = (1u << RegTag::width) - 1u
    mask       = field_mask << shift
    set(reg, value) = reg = (reg & ~mask) | ((value & field_mask) << shift)
```

Register tags (`ModerTag`, `OtyperTag`, `OspeedrTag`, `PupdrTag`) carry only
`static constexpr uint32_t width`, keeping the mask formula generic and
self-consistent — no per-tag hardcoded mask to get out of sync with width.

## Key design decisions

**Compile-time template parameters over runtime members.** `GpioPin<PortBase, Pin>`
and `RegField<RegTag, Pin>` take port/pin/tag as template parameters rather than
constructor arguments. This lets `Pin`'s bit position and mask fold to compile-time
constants — verified in the generated disassembly (`arm-none-eabi-objdump`), where
`shift`/`mask` never appear as runtime computation, only as pre-computed immediates.

**`BSRR` over read-modify-write on `ODR` for `high()`/`low()`.** `ODR |= (1<<Pin)` is
a load-modify-store sequence that races against an ISR touching the same register
between the load and the store — a lost-update bug. `BSRR` is a single atomic store
(set bit `Pin`, or bit `Pin+16` to clear) with no dependency on the register's prior
value, so there's nothing to race. Confirmed in disassembly: `pa5_high`/`pa5_low`
contain zero `ldr` instructions touching the register itself.

**`RegField`'s mask derived from `width`, never hardcoded per register.** Each tag
supplies only `width`; `field_mask = (1u << width) - 1u` is computed once, generically,
inside `RegField`. A hardcoded per-tag mask would be a second number that could silently
drift out of sync with `width` if either were edited independently.

**`static_assert(offsetof(...))` on every `GPIO_TypeDef` member.** Caught a real
layout gap between `PUPDR` (0x0C) and the intended `IDR` (0x10) placement — wait, more
precisely, this caught the actual gap between `BSRR` (0x18) and `AFRL` (0x20): `LCKR`
at 0x1C had been omitted, which would have silently misaligned `AFRL`/`AFRH` by 4 bytes
with no compiler warning. This is exactly the struct-padding hazard the chapter opens
with — `offsetof` static_asserts turn a silent runtime corruption into a build failure.

## Lessons learned

- `&&` (logical AND) vs `&` (bitwise AND) is a silent, compiling bug, not a syntax
  error — hit twice this session, in `RegField::set()` and again in `GpioPin::read()`.
  `value && field_mask` collapses to `0`/`1` instead of masking bits; the compiler
  gives no warning because both are valid expressions, just not the intended one.
- Operator precedence: `>>` binds tighter than `&`. `port().IDR & (1u << Pin) >> Pin`
  parses as `IDR & ((1u << Pin) >> Pin)`, which reduces to `IDR & 1` — always checking
  bit 0 regardless of `Pin`. Needed explicit parentheses:
  `(port().IDR & (1u << Pin)) >> Pin`.
- `volatile` is part of a type, not a hint. `port().MODER` has type `volatile uint32_t&`;
  passing it to a function expecting plain `uint32_t&` is a compile error (discards
  qualifiers), not a warning — the compiler refuses because accepting it would let the
  optimizer legally cache/reorder/eliminate accesses to a register that can change
  outside program control.
- Template type parameters vs. value parameters: `RegField<m, Pin>` (passing a runtime
  `Mode m` where a `typename RegTag` was expected) doesn't compile — a function
  parameter is a runtime value, a template argument needs to be resolvable at compile
  time. Needed `RegField<ModerTag, Pin>`, with `m` used only as the runtime `value`
  argument to `set()`.
- `GpioPin`'s methods are `static` — no `this`. `this->port` was both a missing-`()`
  bug (calling a method, not accessing a member) and a `this`-in-a-static-context bug.
- Struct member `uint32_t width = 2;` (non-static, with default initializer) is
  per-instance storage, not a compile-time constant — unusable inside another type's
  `static_assert`/`static constexpr` unless declared `static constexpr` itself.
- Verified in disassembly (not assumed) that `RegField`'s type-safety costs nothing:
  the MODER config path in `GpioPin` produces byte-for-byte the same instruction count
  and shape as the hand-written C macro baseline. `BSRR`-based `high()`/`low()` are
  additionally *smaller* than the C baseline's `ODR` read-modify-write — 52 bytes total
  vs. 76 for the C baseline (`arm-none-eabi-size`).

## Open items

- Ch.12 exercise (`GpioPin`) complete and verified; `OSPEEDR`/`PUPDR` wrapper methods
  on `GpioPin` follow the same established `RegField` pattern and were left for the
  exercise write-up rather than walked through line-by-line.
- Ch.13 (RAII wrapper for `bmp280.c`/`mpu6050.c`, SPI adapter), Ch.14 (sequencer), Ch.15
  (Observer sensor publisher), Ch.16 (BLE FSM, switch-based + State pattern) — theory
  fully covered Socratically this session; implementation not yet started.
- Ch.17 (ETL, `etl::delegate`, Pigweed) — skim-only per plan, no implementation.
- Compile-time Observer via variadic templates — explicitly deferred to a standalone
  session, not part of Week 36.
- CMake not yet set up for `week36/cpp_applied/`; built so far via direct
  `arm-none-eabi-g++` invocations. Decide before commit whether to wire in CMake to
  match Week 33's toolchain, or keep this block's exercises single-file.