# 🛠️ Embedded Systems — Phase 1 Study Tracker
**C Foundations + Bare-Metal STM32 | Months 1–3 | ~10 hrs/week**

> **Goal:** Get comfortable writing and debugging C on real hardware without IDE abstractions.
> **Strategy:** Solid C first (Weeks 1–5), then bare-metal STM32 (Weeks 6–12).

## 📅 Weekly Schedule

---

### 🔵 BLOCK 1 — C Foundations (Weeks 1–5)

---

#### Week 1 — Environment Setup + C Basics
> ~10 hrs | R1 Chapters 1–4, R4 R5 R6

**Setup tasks**
- [x] Install ARM GCC toolchain (R4) and verify with `arm-none-eabi-gcc --version`
- [x] Install OpenOCD (R5) and confirm it detects your STM32 board --> NEED STM Board --> Ordered
- [x] Set up GDB (R6) and step through a simple Hello World on host
- [x] Configure editor (VS Code recommended: C/C++ extension + Cortex-Debug)
- [x] Create GitHub repo `embedded-phase1` with initial README

**Study tasks**
- [x] R1 Ch.1 — An Overview of C: compilation model, first program
- [x] R1 Ch.2 — Lexical elements, operators, the C system
- [x] R1 Ch.3 — Fundamental data types: integers, chars, floats, type conversion
- [x] R1 Ch.4 — Flow of control: if/else, switch, for/while/do-while
- [x] Write 3 small programs: factorial (loop), FizzBuzz, simple calculator

> 🏁 **M1 checkpoint:** GCC, OpenOCD, and GDB all verified working.

---

#### Week 2 — Functions + Arrays
> ~10 hrs | R1 Chapters 5–6

- [x] R1 Ch.5 — Functions: prototypes, scope, call stack, recursion (draw it out)
- [x] R1 Ch.6 — Arrays, pointers, and strings: declaration, initialization, pointer decay
- [x] Exercise: implement `strlen`, `strcpy`, `memset`, EXTRA: strcat from scratch
- [x] EXTRA: NeuralNet -- map 2 inputs to 5 outputs (logic gates)
- [x] Commit all exercises to GitHub under `week02/`

---

#### Week 3 — Pointers + Bit Manipulation
> ~10 hrs | R1 Chapter 6 +7 (deep dive)
- [x] R1 Ch.6 — Revisit: pointer arithmetic, multi-dimensional arrays, pointers to pointers
- [x] R1 Ch.7 — Bitwise operators and enumerations: `&`, `|`, `^`, `~`, `<<`, `>>`
- [x] **R7 (Barr Group)** — Read sections on `stdint.h` fixed-width types (`uint8_t`, `uint32_t`, etc.) — these are ubiquitous in embedded C and not covered in Kelley & Pohl
- [x] GDB practice: set breakpoints, inspect variables, step through pointer code
- [x] Exercise: pack/unpack a sensor value into a byte using shifts and masks
- [x] Exercise: swap two integers using pointers, without a temp variable
- [x] Commit exercises to GitHub under `week03/`

---

#### Week 4 — Structs + Memory Lay-out
> ~10 hrs | R1 Chapters 8 + 9
- [x] R1 Ch.8 — The preprocessor: `#define`, `#include`, macros with arguments
- [x] R1 Ch.9 — Structures and list processing: declaration, nested structs, struct pointers
- [x] R1 Ch.10 Draw memory layout diagrams: stack, heap, BSS, data, text segments
- [x] Study `static` — file scope vs function scope meanings
- [x] Study `volatile` keyword — why it matters in embedded C
- [x] Practice: set bit N, clear bit N, toggle bit N, read bit N — write macros
- [x] Exercise: model a hardware register using a struct with bitfields
- [x] Exercise: implement a simpledgfd stack (push/pop) using a fixed-size array + pointer
- [x] Commit exercises to GitHub under `week04/`

---

#### Week 5 — C Consolidation + Toolchain Deep Dive
> ~10 hrs | R1 Ch.10-11-12 review + R7 
- [x] R1 Ch.11 File handling
- [x] Learn `Makefile` basics: targets, rules, variables, `$(CC)`, `$(CFLAGS)`
- [x] Write a Makefile that compiles a multi-file C project on the host
- [x] R1 Ch.12 Advanced applications
- [x] Review any chapters that felt weak (common gaps: pointer-to-pointer, function pointers, `const` correctness)
- [x] R2 - C6 Microcontroller Ports — different chip (TM4C123 vs STM32) but the concept of direction registers, memory-mapped I/O, and GPIO programming is identical.
- [x] R2 - C7 Design and Development Cycle
- [x] R2 - C8 Switches and LEDs — breadboard wiring, switch interfaces, LED current limiting. Quick read, practical lab.
- [x] Build `week05/command_parser/` — host-side UART command parser skeleton in plain C: parses `led on/off`, `pwm <0-100>`, `status` commands using a function pointer dispatch table; multi-file project with Makefile

> 🏁 **M2 checkpoint:** C fundamentals solid. Can write, compile, and debug multi-file C projects from the command line.

---

### 🔴 BLOCK 2 — Bare-Metal STM32 (Weeks 6–12)

> From here: no HAL, no CubeMX. Register writes only. Reference: STM32 Reference Manual + R3.

---

Week 6 — STM32 Architecture + GPIO

~10 hrs | R3 Ch.1, Ch.6 + STM32 Reference Manual (GPIO + RCC chapters)


 - [x] R3 Ch.1 — ARM/Cortex-M architecture: core registers, memory map, interrupts overview, CMSIS
 - [x] R3 Ch.2-5 — Skim only: STM32CubeIDE setup (you need the toolchain, skip the CubeMX workflow)
 - [x] R3 Ch.6 — GPIO Management: peripheral mapping, MODER, ODR, alternate function — read the register descriptions, ignore HAL calls
 - [x] Study the memory map from Ch.1: locate Flash, SRAM, and peripheral base addresses for your specific MCU
 - [x] Reference Manual: RCC chapter — understand how to enable GPIO clocks before touching any GPIO register
 - [x] Exercise: bare-metal blink — enable RCC clock → set MODER → toggle ODR, hex values only with comments on every register field
 - [x] Exercise: bare-metal simple alarm - two inputs / one output, tested on breadboard
 - [x] Commit to week06/gpio_blink/


🏁 M3 checkpoint: LED blinking via direct register writes.


Week 7 — UART (Transmit)

~10 hrs | R3 Ch.8 + Reference Manual USART section

 - [x] R3 Ch.8.1 — UART/USART introduction: baud rate formula, frame format (start/stop/parity), TX/RX lines
 - [x] R3 Ch.8.2 — UART initialization fields: understand what BRR, CR1, CR2 each control — ignore HAL_UART_Init(), use the field descriptions
 - [x] R3 Ch.8.3 — Polling mode: understand the status flags (TXE, TC, RXNE) — this maps directly to your bare-metal approach
 - [x] Reference Manual: USART section — confirm BRR calculation for your clock speed
 - [x] Exercise: configure USART2 bare-metal — enable RCC, set GPIO to AF mode, write BRR and CR1 directly
 - [x] Exercise: implement uart_send_char() and uart_send_string(), polling on TXE flag
 - [x] Verify output with a serial terminal (minicom / PuTTY / screen)
 - [x] Commit to week07/uart_tx/


Week 8 — UART (Receive) + Command Parser

~10 hrs | R3 Ch.8 (remainder) + original code


 - [x] R3 Ch.8.4 — Interrupt mode section: read to understand RXNE interrupt mechanics even though you poll first
 - [x] R3 Ch.8.5 — Error management: understand ORE (overrun), FE (framing), NE (noise) flags — know how to clear them
 - [x] Exercise: implement uart_receive_char() — poll on RXNE flag
 - [x] Exercise: build a receive buffer (circular buffer or fixed array + index)
 - [x] Exercise: parse newline-terminated strings into command + argument
 - [x] Commands to support: led on, led off, status
 - [x] Test interactively via serial terminal
 - [x] Commit to week08/uart_rx/


🏁 M4 checkpoint: UART TX and RX working; basic command echo functional.


Week 9 — Timers (Basic)

~10 hrs | R3 Ch.10, Ch.11 + Reference Manual TIM section


 - [x] R3 Ch.10 — Clock Tree: understand how the APB prescalers feed timer clocks — this directly affects your PSC calculation
 - [x] R3 Ch.11.1 — Timer categories: basic, general purpose, advanced — know which TIM2 is and what it can do
 - [x] R3 Ch.11.2 — Basic timers: PSC (prescaler), ARR (auto-reload), CNT — read register descriptions, skip HAL
 - [x] R3 Ch.11.3 (intro) — General purpose timer counting modes — up-counting is your default
 - [x] Reference Manual: TIM2 section — confirm register addresses and reset values
 - [x] Exercise: configure TIM2 in up-counting mode to generate a 1 Hz tick
 - [x] Exercise: implement a millis() equivalent using CNT
 - [x] Exercise: replace for-loop delays in blink code with timer-based delays
 - [x] Commit to week09/timers_basic/
 - [x] C9 — SysTick Timer + Functional Debugging — SysTick is a Cortex-M standard peripheral, present on your STM32 too. The debugging theory section is solid.
 


Week 10 — Interrupts + NVIC

~10 hrs | R3 Ch.7 + ARM Cortex-M PM0214


 - [x] R3 Ch.7.1 — NVIC controller: vector table layout in STM32, interrupt numbers
 - [x] R3 Ch.7.2 — Enabling interrupts: EXTI lines, external GPIO interrupts, NVIC enable registers
 - [x] R3 Ch.7.3 — Interrupt lifecycle: pending, active, preemption — understand what happens on entry/exit
 - [x] R3 Ch.7.4 — Priority levels for your Cortex-M variant (M3/M4/M7 section if applicable)
 - [x] R3 Ch.7.6 — Masking interrupts: PRIMASK, BASEPRI — useful for critical sections
 - [x] Exercise: enable TIM2 update interrupt → toggle LED in ISR
 - [x] Exercise: enable EXTI interrupt on a button GPIO → debounce in ISR or main loop
 - [x] Exercise: refactor UART RX to use RXNE interrupt instead of polling
 - [x] Study ISR best practices: keep ISRs short, use volatile flags, avoid blocking calls
 - [x] Commit to week10/interrupts/


🏁 M5 checkpoint: Timer interrupt firing reliably; UART RX interrupt-driven.


Week 11 — PWM + Integration

~10 hrs | R3 Ch.11.3 (PWM sections) + original code


- [ ]  R3 Ch.11.3.5 — Input Capture mode: read for context, understand the capture/compare unit
- [ ]  R3 Ch.11.3.6 — Output Compare mode: the foundation PWM builds on
- [ ]  R3 Ch.11.3.7 — PWM generation: CCR, PWM1 vs PWM2 mode, duty cycle formula
- [ ]  Reference Manual: TIMx_CCMRx and TIMx_CCRx registers for your timer
- [ ]  Exercise: configure a timer in PWM mode — set duty cycle via CCR
- [ ]  Exercise: drive an LED with variable brightness (fade in/out)
- [ ]  Exercise: integrate UART command parser — accept pwm <0-100> to set duty cycle live
- [ ]  Exercise: wire together blink, UART, timer, interrupt code into a single project
- [ ]  Refactor into clean modules: gpio.c, uart.c, timer.c, main.c
- [ ] C10 — Finite State Machines — FSMs are a fundamental embedded design pattern you'll use in your UART command parser. The concepts translate directly even though the chip differs.
- [ ]  Commit to week11/pwm_integration/

Week 12 — Deliverable + Documentation

~10 hrs | Polish, README, reflection


 - [ ] R3 Ch.5 — Debugging: use the SFRs View to verify your register values, set watchpoints on key variables
 - [ ] Final integration test: all features working together on hardware
 - [ ] Write README.md covering:

 - [ ] Project overview and feature list
 - [ ] Hardware setup (board, wiring diagram or description)
 - [ ] Build instructions (make, flash with OpenOCD/STM32CubeProgrammer)
 - [ ] Design decisions: polling vs interrupts, buffer strategy, register choices
 - [ ] Lessons learned / what you'd do differently


 - [ ] Add schematic or ASCII wiring diagram
 - [ ] Tag the repo: git tag v1.0-phase1
 - [ ] Clean up dead code, add comments where missing
 - [ ] Write a short Phase 1 retrospective (NOTES.md)


🏁 M6 checkpoint: GitHub repo published. Phase 1 complete. ✅

---

## 📊 Progress Summary

| Week | Topic | Status |
|------|-------|--------|
| 1 | Environment Setup + C Basics | ✅ Done |
| 2 | Functions + Arrays | ✅ Done |
| 3 | Pointers + Memory Layout | ✅ Done |
| 4 | Structs + Bit Manipulation | ✅ Done |
| 5 | C Consolidation + Toolchain | ✅ Done |
| 6 | STM32 Architecture + GPIO | ✅ Done |
| 7 | UART TX | ✅ Done |
| 8 | UART RX + Command Parser | ✅ Done |
| 9 | Timers | ✅ Done |
| 10 | Interrupts + NVIC | ⬜ Not started |
| 11 | PWM + Integration | ⬜ Not started |
| 12 | Deliverable + Docs | ⬜ Not started |

> Update statuses to: `🟡 In progress` → `✅ Done`


---

## 📦 Resources

| # | Resource | Format | Link |
|---|----------|--------|------|
| R1 | A Book on C: Programming in C | Book | Al Kelley & Ira Pohl — ISBN 978-0805316247 |
| R2 | Embedded Systems — Shape The World | Course | [edX / UT Austin (free audit)](https://www.edx.org/course/embedded-systems-shape-the-world-microcontroller-i) |
| R3 | Mastering STM32 | Book/PDF | [Leanpub (free PDF)](https://leanpub.com/mastering-stm32) |
| R4 | ARM GCC Toolchain | Tool | [developer.arm.com](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads) |
| R5 | OpenOCD | Tool | [openocd.org](https://openocd.org/) |
| R6 | GDB (arm-none-eabi-gdb) | Tool | Included with R4 |
| R7 | Embedded C Coding Standard | PDF | [Barr Group (free)](https://barrgroup.com/sites/default/files/barr_c_coding_standard_2018.pdf) — covers `stdint.h`, `volatile`, embedded conventions |

---

## 🏁 Milestone Overview

| Milestone | Target Week | Description |
|-----------|-------------|-------------|
| M1 | End of Week 2 | Dev environment fully set up (GCC, OpenOCD, GDB) |
| M2 | End of Week 5 | C fundamentals complete — mini quiz / practice problems done |
| M3 | End of Week 7 | First LED blink via bare-metal GPIO register write |
| M4 | End of Week 9 | UART TX/RX working without HAL |
| M5 | End of Week 11 | Timer interrupt firing reliably |
| **M6** | **End of Week 12** | **GitHub deliverable repo published with README** |

---

