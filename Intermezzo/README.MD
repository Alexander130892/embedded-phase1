# ATmega328P Bare-Metal Intermezzo

A short intermezzo applying STM32 bare-metal learnings to the Arduino Uno (ATmega328P).

## Goals

Apply bare-metal register-level programming concepts from STM32 to an 8-bit AVR architecture, without the Arduino runtime or any HAL.

## Toolchain

- **Compiler:** `avr-gcc`
- **Flasher:** `avrdude` (via the Uno's onboard optiboot bootloader — no ST-Link needed)
- **Build system:** Makefile
- **Editor:** VSCode with custom tasks for build, flash, clean, size, disasm

## What we explored

### GPIO registers
Direct register access maps cleanly to STM32 concepts:

| AVR | STM32 | Purpose |
|---|---|---|
| `DDRx` | `GPIOx->MODER` | pin direction |
| `PORTx` | `GPIOx->ODR` | write output |
| `PINx` | `GPIOx->IDR` | read input |

Key difference: ATmega328P only supports input/output — no alternate function or analog mode in the direction register. Also no internal pull-down resistors, only pull-ups (via `PORTx` when `DDRx=0`).

### Disassembly
Used `avr-objdump -d -S` to inspect generated assembly. Bare-metal register writes compile to single instructions (`cbi`, `sbi`) — the AVR ISA has dedicated bit manipulation instructions for I/O registers.

### Arduino overhead
Compared bare-metal `DDRB |= (1 << PB2)` (1 instruction) against Arduino's `pinMode()` (~20+ instructions). The overhead comes from runtime pin number translation via lookup tables, pin validation, and interrupt-safe read-modify-write.

### `_delay_ms()`
Provided by `avr-libc` as a compile-time calculated busy-loop based on `F_CPU`. Convenient but blocks the CPU — the proper approach is a hardware timer interrupt, same as on STM32.

### Hardware vs software PWM
Hardware PWM uses the timer peripheral (set up once, CPU-free). Software PWM bit-bangs a GPIO manually (CPU-intensive, jitter under load). Hardware PWM is limited to dedicated OCx pins; software PWM works on any pin.

## Key takeaway

The bare-metal concepts from STM32 transfer directly — memory-mapped registers, DDR/PORT/PIN vs MODER/ODR/IDR, hardware timers, ISRs. The ATmega328P is simpler (8-bit, flat I/O space, minimal peripheral complexity) which makes the register-level mechanics easier to see clearly.