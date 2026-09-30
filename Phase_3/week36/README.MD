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

## Architecture — BLE connection FSM (Ch.16, built and verified this session)

Same transition table implemented two ways:

```
IDLE          -- StartAdvertising --> ADVERTISING
ADVERTISING   -- ConnectionReq    --> CONNECTING
CONNECTING    -- ConnectionAck    --> CONNECTED
CONNECTING    -- TimeOut          --> DISCONNECTING
CONNECTED     -- LinkLost         --> DISCONNECTING
DISCONNECTING -- Disconnect       --> IDLE
(any other event in a given state: ignored, state unchanged)
```

**Switch-based** (`fsm_switch/`): one `enum class BleState` member, one
`handle_event()` with a `switch(state_)` and an `if`/`else if` per valid event
in that state, unconditional `break` after each case body.

**State pattern** (`fsm_state_pattern/`): abstract `BleState` with a pure-virtual
`on_event(BleEvent) -> BleState*`; five concrete classes (`Idle`, `Advertising`,
`Connecting`, `Connected`, `Disconnecting`), each a static global instance;
`BleConnection` holds `BleState* current_` and does `current_ = current_->on_event(ev)`.

## Architecture — Observer sensor publisher (Ch.15, built and verified this session)

```
Bmp280
    │  observers_: std::array<ISensorObserver*, MaxObservers>   (no heap, per Week 33)
    │  count_: size_t
    │
    ├── attach(obs) ──► bounds-checked append, returns false past MaxObservers
    │
    └── poll() ───────► update reading, then for i in [0, count_): observers_[i]->on_data(data)

ISensorObserver (abstract)
    virtual on_data(const Bmp280Data&) = 0

UartLogger : ISensorObserver     LedIndicator : ISensorObserver
    on_data() -> printf log line     on_data() -> threshold check, LED ON/OFF
```

`Bmp280` never names `UartLogger` or `LedIndicator` anywhere in its own source —
verified by running with both attached and confirming each fires independently
on every `poll()`, from a single shared `Bmp280Data` value.

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

**Switch-based over State pattern for this FSM's error surface.** State pattern
structurally eliminates cross-case fallthrough (each `on_event()` is its own
function — nothing to "fall into"), a bug class actually hit and fixed in the
switch version during this session. Neither design catches "wrote the wrong
event/target pair by hand" — that error class was hit in the State pattern
version too, independent of the mechanism. Chose State pattern for this FSM
specifically because the byte cost (196 → 556 bytes text, +20 bytes data for
vtable-pointer init) is immaterial at this call frequency (connection-state
changes, not a hot loop) and it removes a bug class already demonstrated to be
easy to hit by hand.

**Fixed-capacity `std::array` for the observer list, not `std::vector`.** Consistent
with Week 33's no-heap discipline — `attach()` bounds-checks against `MaxObservers`
and returns `false` rather than growing, so there's no allocation anywhere in
`Bmp280`'s lifetime. `virtual on_data()` dispatch itself is accepted deliberately:
`poll()` fires at sensor-sample rate, not in a hot loop, so the vtable indirection
is immaterial versus the decoupling it buys (new observer types addable with zero
changes to `Bmp280`).

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
- Switch-based FSM: a `case` body's `if` block with no `break` outside it falls
  through into the next `case` unconditionally on no-match — hit this with
  `Idle`'s handler falling through into `Advertising`'s condition check when an
  invalid event arrived, silently transitioning through two states instead of
  correctly staying put. Fix: every `case` needs an unconditional `break` reached
  regardless of whether its `if` matched, not nested inside the `if`.
- State pattern: transcribed three transitions wrong against my own table on
  first pass (`Connecting`'s `TimeOut` target, and `Connected`/`Disconnecting`'s
  trigger events swapped with each other) — all valid `BleEvent` enumerators, so
  the compiler accepted all of it silently. Confirms the State pattern adds no
  compile-time exhaustiveness/correctness guarantee over switch — it only
  removes the fallthrough failure mode specifically, not hand-transcription
  errors in general.
- Native `g++` (not `arm-none-eabi-g++`) used to verify FSM logic with real
  `printf` output, since the FSM touches no hardware registers — cross-compiled
  separately, `.o`-only (no link step), purely for the `arm-none-eabi-size`
  comparison between the two designs.
- Observer: `Bmp280` was initially defined before `ISensorObserver` in the file,
  even though `Bmp280` references `ISensorObserver*` as a member type — same
  "must be declared before referenced" constraint as the FSM's state classes,
  just missed here since there was no circular reference forcing an out-of-line
  split, only a straight reordering (`ISensorObserver` moved above `Bmp280`).
- Observer: range-`for` over the full `std::array<ISensorObserver*, MaxObservers>`
  instead of indexing `0..count_` would visit unused (null) slots and call
  through a null pointer — caught before running, fixed to an indexed loop
  bounded by `count_`.
- Observer: `observer.on_data(&fake_data)` had two separate mistakes at once —
  `.` instead of `->` on a pointer, and passing `&fake_data` (a pointer) where
  `on_data(const Bmp280Data&)` expects a reference (no `&` needed at the call
  site; a reference binds directly to the object).

## Open items

- Ch.12 exercise (`GpioPin`) complete and verified; `OSPEEDR`/`PUPDR` wrapper methods
  on `GpioPin` follow the same established `RegField` pattern and were left for the
  exercise write-up rather than walked through line-by-line.
- Ch.16 exercise (BLE FSM, both switch-based and State pattern) complete and
  verified — identical behavior confirmed via matching test-sequence output;
  code-size tradeoff measured and a deliberate choice made (State pattern).
- Ch.15 exercise (Observer sensor publisher) complete and verified — `UartLogger`
  and `LedIndicator` both fire independently from `Bmp280::poll()` with neither
  aware of the other, and `Bmp280` names neither concrete type anywhere.
- Ch.13 (RAII wrapper for `bmp280.c`/`mpu6050.c`, SPI adapter), Ch.14 (sequencer)
  — theory fully covered Socratically this session; implementation not yet started.
- Ch.17 (ETL, `etl::delegate`, Pigweed) — skim-only per plan, no implementation.
- Compile-time Observer via variadic templates — explicitly deferred to a standalone
  session, not part of Week 36.
- CMake not yet set up for `week36/cpp_applied/`; built so far via direct
  `arm-none-eabi-g++`/`g++` invocations. Decide before commit whether to wire in
  CMake to match Week 33's toolchain, or keep this block's exercises single-file.