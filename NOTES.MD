# Phase 1 Retrospective — Embedded Systems Self-Study

**Duration:** Months 1–3 (~10 hrs/week)  
**Focus:** C foundations + bare-metal STM32 (no HAL)  
**Background:** M.Sc. Electrical Engineering, ~10 years manufacturing/project engineering at Volvo Car

---

## What Phase 1 actually is

Three months ago I opened a C textbook with a solid hardware background but zero firmware experience. Phase 1 was about closing that gap — not just learning syntax, but learning to think like an embedded engineer.

---

## The single biggest shift

The real difference between embedded and application code is not the language — it's the relationship with the hardware.

In application code, the OS, runtime, and standard library abstract everything away. In bare-metal embedded, you *are* the abstraction layer. Every peripheral has to be clocked before you touch it. Every register bit has a specific meaning and a specific order in which it must be set. Memory is mapped — a `uint32_t*` pointing to `0x40020000` is not a variable, it is a hardware register, and writing to it changes the physical state of the chip.

That mental shift — from "variables hold data" to "addresses control hardware" — is what Phase 1 is really about.

---

## Hardest concept to crack: register setup and order of operations

Reading a reference manual for the first time is overwhelming. The challenge isn't finding the right register — it's understanding *why* the sequence matters.

The moment it clicked was working through the PWM setup:
- Clock the peripheral *before* touching its registers
- Configure the timer *before* enabling it
- Write PSC and ARR *before* triggering UG — because the hardware uses shadow registers, not your direct writes
- Enable the channel output (CC1E) and only then set CEN

Each step has a reason. Once the reasons were clear, the sequence became obvious rather than arbitrary.

---

## What I'm most satisfied with

The final project — `uart_pwm_commander` — running on real hardware with no HAL:

- Interrupt-driven UART RX with a circular buffer
- PWM duty cycle updated live from serial commands
- 1 Hz hardware blink via TIM2 ISR, duty cycle preserved via CC1E toggle
- Clean module structure: `gpio.c`, `uart.c`, `timer.c`, `parser.c`

Every register write in that project is deliberate and understood. That's the standard I was aiming for.

---

## What I'd refactor with more time

- The circular buffer deserves a proper abstraction — a `ring_buffer_t` struct with push/pop functions, rather than bare `head`/`tail` globals in `uart.c`
- A `line_ready` flag rather than scanning for newlines inside the receive function — cleaner separation between transport and parsing
- Interrupt flag clearing should consistently happen *before* acting on the event, not after

---

## Real bugs that taught real lessons

| Bug | Root cause | Fix |
|-----|-----------|-----|
| `while (fade >= 0)` infinite loop | `uint32_t` underflow wraps to 4 billion | `while (fade > 0)` + `--fade` pre-decrement |
| Multiple definition linker error | `head`/`tail` defined in `.h`, not `.c` | Define in `.c`, declare `extern` in `.h` |
| Partial strings lost between calls | `index` was local, reset every call | `static uint8_t index` |
| FPU crash on explicit `(float)` cast | FPU not initialised on target | Use `100.0` double literal instead |
| PWM glitch risk | CCR preload not enabled | Set OC1PE in TIMx_CCMR1 |
| Timer started with stale shadow registers | UG not written before CEN | Write UG=1 in TIMx_EGR first |

---

## Advice to Day 1 self

Go on — it is a really fun learning journey.

The reference manual looks intimidating. Read it anyway. The answers are all in there, and learning to navigate it is the skill. Every bug you chase down leaves a permanent mark on your understanding that no tutorial can replicate.

---

## Phase 2 direction

- SPI / I2C / UART protocol deep-dive
- FreeRTOS — task scheduling, queues, semaphores
- Raspberry Pi introductory Linux
- Blue Pill (STM32F103C8T6) as a bare board target
- Logic analyser in the loop for protocol debugging