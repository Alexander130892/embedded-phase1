# Embedded Systems — Phase 1: C Foundations + Bare-Metal STM32

A structured, self-directed embedded systems curriculum built around one rule: **no HAL, no CubeMX — register writes only.**

This repo documents my progression from C fundamentals to bare-metal peripheral programming on STM32, working through ~10 hours/week over 3 months. Everything is written against the reference manual with explicit register-field comments.

**Hardware:** STM32 Nucleo-F446RE
**Toolchain:** `arm-none-eabi-gcc` · `OpenOCD` · `GDB` · `make`  
**Reference:** [Mastering STM32](https://leanpub.com/mastering-stm32) · A Book on C (Kelley & Pohl) · STM32 Reference Manual · [Barr Group Embedded C Standard](https://barrgroup.com/sites/default/files/barr_c_coding_standard_2018.pdf)

---

## Progress

| Week | Topic | Status |
|------|-------|--------|
| 01 | Environment setup + C basics | ✅ Done |
| 02 | Functions + arrays | ✅ Done |
| 03 | Pointers + bit manipulation | ✅ Done |
| 04 | Structs + memory layout | ✅ Done |
| 05 | C consolidation + Makefile + toolchain | ✅ Done |
| 06 | STM32 architecture + GPIO (bare-metal blink) | ✅ Done |
| 07 | UART TX (bare-metal, polling) | ✅ Done |
| 08 | UART RX + command parser | ✅ Done |
| 09 | Timers — PSC/ARR, millis(), timer-based delays | ✅ Done |
| 10 | Interrupts + NVIC | ✅ Done |
| 11 | PWM + full integration | ✅ Done |
| 12 | Deliverable + documentation | ✅ Done |

---

## What's in this repo

### Block 1 — C Foundations (Weeks 1–5)

Working through core C from first principles before touching hardware. Key exercises:

- `week02/` — `strlen`, `strcpy`, `memset` from scratch; neural net mapping 2 inputs → 5 outputs (logic gates)
- `week03/` — Pointer arithmetic, bitwise operators, sensor value pack/unpack using shifts and masks
- `week04/` — Hardware register modelling using bitfields; custom stack implementation (push/pop); bit manipulation macros (`SET_BIT`, `CLEAR_BIT`, `TOGGLE_BIT`, `READ_BIT`)
- `week05/` — Multi-file C project with Makefile; UART command parser skeleton using a function-pointer dispatch table (supports `led on/off`, `pwm <0–100>`, `status`)

### Block 2 — Bare-Metal STM32 (Weeks 6–12)

No HAL. No CubeMX. Every peripheral is configured by writing directly to memory-mapped registers with comments on each field.

- `week06/` — RCC clock enable → GPIO MODER → ODR toggle. Bare-metal blink + simple alarm (two inputs, one output) tested on breadboard
- `week07/` — USART2 configured bare-metal: RCC, GPIO alternate function, BRR calculation, CR1. Implements `uart_send_char()` and `uart_send_string()` polling on TXE flag
- `week08/` — UART RX polling on RXNE flag; circular receive buffer; newline-terminated string parser; full command dispatch (`led on`, `led off`, `status`) tested interactively via serial terminal
- `week09/` — TIM2 in up-counting mode; PSC/ARR set for 1 Hz tick; `millis()` equivalent using CNT; timer-based delays replacing busy-wait loops
- `week10/` — External interrupts (EXTI): NVIC configuration, ISR structure, debounce strategy; TIM2 update interrupt; interrupt-driven LED control replacing polling loops
- `week11/` — TIM2 in PWM mode 1: alternate function GPIO, CCR/ARR duty cycle formula, OC1PE preload enable, CC1E channel output; LED fade via CCR stepping with millisecond timer
- `week12/uart_pwm_commander/` — Phase 1 capstone. Interrupt-driven UART RX with circular buffer; live PWM duty cycle control via serial commands (`pwm <0-100>`); 1 Hz hardware blink via TIM2 ISR with CC1E toggle preserving duty cycle; modular structure: `gpio.c`, `uart.c`, `timer.c`, `parser.c`

---

**Requirements:**
- `arm-none-eabi-gcc` (ARM GNU Toolchain)
- `openocd` (for flashing + debug)
- `arm-none-eabi-gdb` (included with the ARM toolchain)

Connect the Nucleo board via USB — OpenOCD picks up the onboard ST-Link automatically.

---

## Design decisions

**No HAL.** The HAL abstracts exactly the things worth understanding: clock configuration, register layouts, flag polling. Working against the reference manual directly means every line of initialisation code has a reason.

**Polling before interrupts.** UART RX and timers are implemented in polling mode first, then refactored to interrupt-driven in week 10. Understanding the flags before hiding them behind ISRs makes the interrupt model easier to reason about.

**Incremental integration.** Each week's code stands alone. Week 12 pulls everything together (GPIO, UART, timers, PWM, interrupts) into a single modular project with `gpio.c`, `uart.c`, `timer.c`, `main.c`.

---

## Resources

| # | Resource |
|---|----------|
| R1 | A Book on C — Kelley & Pohl (ISBN 978-0805316247) |
| R2 | [Embedded Systems: Shape the World — UT Austin / edX](https://www.edx.org/course/embedded-systems-shape-the-world-microcontroller-i) |
| R3 | [Mastering STM32 — Leanpub](https://leanpub.com/mastering-stm32) |
| R4 | [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) |
| R5 | [OpenOCD](https://openocd.org/) |
| R7 | [Barr Group Embedded C Coding Standard](https://barrgroup.com/sites/default/files/barr_c_coding_standard_2018.pdf) |

See [`PHASE1_TRACKER.md`](./PHASE1_TRACKER.md) for the full week-by-week study plan and milestone breakdown.

---

## About

I'm an Electrical Engineer (M.Sc., Ghent University) pivoting from 10 years in automotive manufacturing into embedded systems engineering. This repo is part of a structured self-study programme to build practical embedded depth.

→ [LinkedIn](https://www.linkedin.com/in/alexandervindelinckx/) · [GitHub profile](https://github.com/Alexander130892)
