# Software UART — Bare-Metal ATmega328P

## Goal

Implement a full software UART (TX + RX) from scratch on the ATmega328P without using the hardware USART peripheral. The SW UART runs on PD7 (TX) and PD6 (RX) at 9600 baud, 8N1. Hardware UART (USART0) is reserved for terminal output only.

Communication partner: STM32F446RE Nucleo via UART4 (PA0/PA1).

---

## Architecture

```
STM32 UART4 TX (PA0) ──→ Arduino PD6 (SW RX)
STM32 UART4 RX (PA1) ←── Arduino PD7 (SW TX)

Arduino USART0 TX (PD1) ──→ USB ──→ PC Terminal
```

### State machine

```
         PCINT2 falling edge
IDLE ──────────────────────→ START_SAMPLE
  ↑                               │
  │         glitch detected       │ timer fires, PD6 low confirmed
  ├───────────────────────────────┘
  │                               ↓
  │                           RECEIVING
  │                               │ 8 data bits + stop bit
  ├───────────────────────────────┘
  │
  │    sendByte() called
  └──────────────────────→ TRANSMITTING
                               │ start + 8 data + stop
                               └──→ IDLE
```

### Timer1 usage

- **CTC mode**, `OCR1A` driven
- TX: `OCR1A = 1663` (104µs at 16MHz — one bit period)
- RX start sample: `OCR1A = 831` (half period — sample middle of start bit)
- RX data bits: reset to `OCR1A = 1663` after start bit confirmed

---

## Module structure

```
Inc/
  uart.h          — shared config (BAUD, ARR, MAX_LENGTH, return types, state enum)
  sw_uart.h       — SW UART public API
  hw_uart.h       — HW UART public API
Src/
  sw_uart.c       — SW UART implementation (ISRs, ring buffer, state machine)
  hw_uart.c       — HW UART implementation (polling TX to terminal)
  main.c          — application logic
build/            — compiled output
Makefile
```

---

## Public API

```c
// Buffer layer
bool                available(void);
uart_return_state_t readByte(uint8_t* data);
void                clearBuffer(void);

// Transport layer
uart_return_state_t sendByte(uint8_t data);
uart_return_state_t uart_receive_string(char* str);
```

---

## Key design decisions

**Timer1 reuse for TX and RX** — since SW UART is inherently half-duplex, TX and RX share Timer1. The state machine ensures only one direction is active at a time, making a second timer unnecessary.

**PCINT2 for start bit detection** — `INT0`/`INT1` are only available on PD2/PD3. PD6 requires the PCINT2 group (`PCMSK2`, `PCICR`). The ISR checks for a falling edge explicitly since PCINT fires on both edges.

**Half-period start bit sample** — after PCINT2 fires, Timer1 is started with `OCR1A = 831` (half period) to sample the middle of the start bit. If PD6 is still low, the start bit is valid. If high, it was a glitch. This prevents false triggering on noise.

**Single stop bit** — original implementation used `bit_counter` cases 9 and 10, producing two stop bit periods. The logic analyzer showed this as a parity bit + stop bit (PE flag). Fixed by merging cases 9 and 10 — stop timer immediately after asserting the stop bit.

**Half-duplex flow control** — both sides check the RX line is idle (high) before transmitting. This is a software CSMA approach — sense the line, transmit only if idle. STM32 acts as master to avoid simultaneous transmission collisions.

**Ring buffer with head/tail** — RX bytes are stored in a 64-byte circular buffer. `head` advances in the ISR, `tail` advances in main. `clearBuffer()` uses `cli()`/`sei()` to prevent a race condition between ISR write and main clear.

---

## Lessons learned

- **`sbi`/`cbi` require compile-time constants** — the compiler emits single-cycle bit instructions only when the pin number is known at compile time. Variable pin numbers force a read-modify-write sequence.
- **Declaring variables in header files** causes multiple definition linker errors. Use `extern` in `.h` and define once in `.c`, or keep internal variables out of headers entirely.
- **Operator precedence in bit extraction** — `PIND & (1u << PD6) >> PD6` is parsed as `PIND & ((1u << PD6) >> PD6)`. Always add explicit parentheses around bit manipulation expressions.
- **Variable declarations inside `switch` cases** require enclosing braces — C99 doesn't allow declarations in case labels without a block scope.
- **`PORTD` vs `PIND`** — `PORTD` reads the output latch, `PIND` reads actual pin state. Always use `PIND` for input sampling.
- **`uint8_t` loop counter overflow** — using `uint8_t d = 0; d < 10000` silently overflows at 255, making the delay far shorter than intended.
- **Two stop bit periods** — a state machine case that sets the stop bit and a separate case that stops the timer produces two high periods. The logic analyzer correctly flags this as a parity error.
- **Makefile `build` target ambiguity** — naming both a phony target and a directory `build` causes Make to drop the dependency. Solution: keep `build` as a real target, remove it from `.PHONY`.
- **`${fileDirname}` in VSCode tasks** — points to the open file's directory (`Src/`), not the project root. Use `${fileDirname}/..` for projects with a `Src/` subdirectory.

---

## Open items

- RX framing error handling — currently sets a flag but does not retry or discard the corrupted byte
- No timeout on `sendByte()` busy-wait — if the line never goes idle, main blocks forever
- Flow control is advisory only — simultaneous transmission still possible if both sides check at the same instant
- `uart_receive_string()` processes one byte per call — caller must loop until `STATUS_OK` to drain the buffer