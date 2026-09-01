# Week 34 — C++ Fundamentals: Classes + Error Handling

## Goal

Port the Phase 2 MPU6050 driver (originally plain C, register-level, no HAL) into modern
C++ — classes, constructor-injected dependencies, and exception-free error handling — while
staying inside the embedded constraints established earlier: no heap, no exceptions, no RTTI,
fixed-width types, BARR-C style. Secondary goal: demonstrate `extern "C"` linkage with a real
ISR (`UartLogger`), and prove the design is host-testable with `fff` before it ever touches
target hardware.

## Scope

- `explicit` on single-argument (and effectively-single-argument) constructors
- Dynamic polymorphism cost (vptr, vtable indirection) vs. CRTP vs. policy-based design
- Namespaces, `extern "C"` linkage and name mangling, C/C++ interop for ISR handlers
- References vs. pointers, lvalue/rvalue distinction, move semantics without heap allocation
- `std::array` vs. raw array decay, `std::span` as a non-owning buffer view
- Exception-free error handling: `std::optional`, `std::expected<T, E>`, `assert` for
  precondition violations vs. propagated errors for environmental failures
- `const` vs. `constexpr`, `[[nodiscard]]`, `[[maybe_unused]]`
- Host-side unit testing with `fff` (Fake Function Framework) via a swappable transport policy

## Architecture

```
                    ┌─────────────────────────────┐
                    │   Mpu6050<I2CTransport>      │
                    │   (policy-templated class)   │
                    │                               │
                    │   init()                      │
                    │   read_data()                 │
                    │   read_who_am_i()              │
                    │                               │
                    │   dev_addr_                    │
                    │   transport_ : I2CTransport&   │
                    └───────────────┬───────────────┘
                                    │ calls
                    ┌───────────────┴───────────────┐
                    │  writeRegister() / readRegister()
                    │  readBurst()
                    │  → std::expected<T, ErrorCode>
                    └───────────────┬───────────────┘
              (compile-time swap, no vtable)
          ┌─────────────────────────┴─────────────────────────┐
          │                                                     │
┌─────────▼─────────┐                                ┌──────────▼──────────┐
│     RealI2c        │                                │      FakeI2c         │
│  wraps i2c.c        │                                │  wraps fff-faked      │
│  (real registers)   │                                │  free functions        │
│  → target hardware   │                                │  → host unit tests      │
└─────────────────────┘                                └───────────────────────┘

Separately — UartLogger (ISR-driven TX):

  log()  ──push bytes──▶  RingBuffer (std::array<uint8_t,50> +
                            atomic<uint16_t> write_idx/read_idx)
                                        │
                                        │ drainByte()
                                        ▼
                          extern "C" void USART2_IRQHandler(void)
                          (unmangled symbol, matches vector table;
                           calls into g_uart_logger, a global instance)
```

## Key design decisions

**Policy-based design over CRTP or virtual dispatch for the I2C transport.** The transport
(real hardware vs. fake) is known at compile time — one build targets the MCU, another runs
host-side tests — so there's no runtime "which implementation" decision to make. CRTP was
considered and rejected: it's a base-class/derived-class relationship (`SensorBase<Derived>`),
whereas `Mpu6050` merely *uses* a transport, it doesn't specialize one — that's composition,
not inheritance. `Mpu6050<I2CTransport>` holds the transport as a template parameter and a
reference member, giving zero-cost, no-vtable swapping between `RealI2c` and `FakeI2c`.

**`std::expected<T, ErrorCode>` over `std::optional<T>` for all transport and driver returns.**
`optional` collapses every failure into "nothing," which loses the distinction between, e.g.,
an I2C bus timeout (retry-worthy) and a calibration/logic problem (not retry-worthy). Every
layer (`RealI2c`, `FakeI2c`, `Mpu6050`) propagates a typed `ErrorCode` instead.

**`enum class ErrorCode : uint8_t` over reusing the C `status_t` enum.** Zero additional
runtime cost versus a plain C enum (no vtable, no allocation — `enum class` is just a
compile-time type-safety and scoping upgrade), but it prevents `ErrorCode` from being silently
compared against unrelated integer status codes, and the explicit `: uint8_t` keeps it
fixed-width per BARR-C convention.

**`assert(false && "message")` for logically-impossible `switch` defaults, not a swallowed
generic error.** When `RealI2c`/`FakeI2c` map a C `status_t` to `ErrorCode` and encounter a
status that should be impossible given the calling context (e.g., `UART_TIMEOUT` returned from
an I2C-only function), the `default` case asserts unconditionally rather than silently
returning a plausible-looking wrong `ErrorCode` — the goal is to fail loudly at the violation
site, not to let a logic bug disguise itself as a normal recoverable error further downstream.
A `return` still follows the assert, so the function stays well-defined in `NDEBUG` builds.

**Global `UartLogger` instance for the ISR, deliberately.** An ISR's signature is fixed by the
hardware vector table — it can't receive constructor-injected dependencies. A global object is
the accepted embedded C++ idiom for bridging normal code and interrupt context; the downside
(hidden coupling) is real but unavoidable here, and is contained by keeping `UartLogger`'s
public surface small (`log()`, `drainByte()`).

**`std::atomic<uint16_t>` for the ring buffer's `write_idx`/`read_idx`**, not a mutex. An ISR
cannot block/wait the way a thread can — attempting to acquire a lock held by preempted
lower-priority code would deadlock. Atomics give torn-free single-variable access with no
blocking. Known limitation: the *combination* of both indices (used in the full/empty check)
isn't atomic as a pair — acceptable here since the worst case is a one-iteration-stale
full/empty read, not corruption.

## Lessons Learned

- A constructor is a conversion candidate for *any* call with one argument, including
  multi-parameter constructors where all but one parameter has a default — `explicit` is
  needed there too, not just on strictly single-parameter constructors.
- A reference member (`I2CTransport&`) **must** be initialized in the constructor's member
  initializer list — it cannot be default-constructed and assigned in the body, since a
  reference can never exist in an unbound state, even transiently.
- `auto` deduction strips references and top-level `const` by default; `auto handle =
  getHandle();` on a function returning `const T&` silently produces a disconnected copy, not
  an alias — `const auto&` is required to preserve reference semantics.
- `std::span` gives decay-proof size information and a `const`-correct read-only view, but
  provides **zero lifetime protection** — a `span` returned from a function pointing at that
  function's local array dangles exactly like a raw pointer would.
- `extern "C"` controls linkage (symbol name / mangling) only — it does not restrict what's
  inside the function body to C syntax; a full C++ object's methods can be called from within
  an `extern "C"` function without issue.
- `i++` / `delay++` on a `volatile`-qualified variable is deprecated as of C++20; `i = i + 1`
  preserves the same "don't optimize this away" property without triggering the deprecation
  warning.
- `g++` on macOS is Apple Clang, and its default `-std=` predates C++23 — `std::expected`
  requires `-std=c++23` explicitly; compiler defaults lag the latest standard deliberately, for
  backward compatibility, even when the compiler fully supports newer features.
- `std::array<uint8_t, N>` avoids raw-array decay because decay only applies to arrays used
  directly as parameters — wrapping the array as a struct member sidesteps decay entirely,
  since decay never applies to structs/classes, regardless of what they contain.
- `std::expected<T, E>`/`std::optional<T>` overload `operator*` and expose `.error()`— both
  are unchecked accessors that assume the correct alternative is already active (checked via
  `has_value()`/boolean conversion first); calling either on the wrong alternative is
  undefined behavior, symmetric in both directions.
- `fff` fakes free functions, not C++ methods directly — `FakeI2c`'s methods are thin wrappers
  around `fff`-faked free functions, translating the fake's plain `int`/status-style return
  into `std::expected<void, ErrorCode>`. Kept deliberately free of independent logic ("dumb
  plumbing") so the tests exercise `Mpu6050`'s behavior, not a second untested implementation.
- Verified via test (`call_count == 1` on injected failure) that `init()` actually stops at the
  first failing register write rather than continuing through all four — confirms the
  early-return `std::expected` propagation pattern works at runtime, not just that it compiles.
- Running the host binary with `RealI2c` segfaults, as expected — `RCC_BASE_ADDR`/
  `GPIOB_BASE_ADDR`-style addresses are real STM32 peripheral addresses, meaningless (unmapped)
  on the host's own address space; this is exactly why `FakeI2c` exists for host-side testing.

## Open Items

- `RealI2c`'s failure-mode mapping only covers `I2C_TIMEOUT`/`I2C_AF`/`I2C_ARLO`; `FakeI2c`
  currently collapses all injected failures to `ErrorCode::UNKNOWN_ERROR` — could be extended
  with a settable "which error to simulate" field if per-variant `ErrorCode` testing is needed.
- `USART_CR1_OFFSET` and the exact `TXEIE` bit position are placeholders pending a datasheet
  check against `stm32f446re.h`.
- No test coverage yet for `readBurst` failure paths or for `UartLogger`'s ring buffer
  (full/empty edge cases, ISR/`log()` interleaving) — `fff`-based or host-side tests for
  `UartLogger` were not written this week.
- Cross-compilation with `arm-none-eabi-g++` (target build, as opposed to host `g++` used for
  testing) not yet verified for `-std=c++23` support or flag parity with the host toolchain.