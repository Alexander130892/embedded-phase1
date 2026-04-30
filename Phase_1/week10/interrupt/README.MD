# Week 10 — Interrupts & NVIC

**Platform:** STM32 NUCLEO-F446RE (Cortex-M4)  
**Toolchain:** arm-none-eabi-gcc, OpenOCD, STM32CubeIDE  
**Constraint:** No HAL, no CubeMX — bare-metal register writes only

---

## Exercises

### 1. TIM2 Update Interrupt — LED Toggle
- TIM2 configured at 16MHz HSI, PSC=15999, ARR=499 → 2Hz update event
- UIE enabled in DIER, NVIC wired via ISER0 (IRQ position 28)
- ISR toggles PA10 (D2) and clears UIF flag
- `overflow_count` incremented in ISR to support `millis()` timekeeping

### 2. EXTI Button Interrupt — Breadboard Button on PB10 (D6)
- SYSCFG_EXTICR3 routes PB10 to EXTI10
- Rising edge trigger (RTSR), interrupt unmasked in IMR
- NVIC wired via ISER1 (IRQ position 40, bit 8)
- Software debounce using `millis()` — 500ms lockout window
- Button press clears LED and toggles TIM2 interrupt via DIER XOR

### 3. USART2 RX Interrupt — Interrupt-driven Echo with Circular Buffer
- PA2 (TX) / PA3 (RX) at 115200 8N1, AF7
- RXNEIE enabled in CR1 — ISR stores byte into circular buffer, advances head
- Main loop consumes from tail, assembles into local string buffer
- Handles `\r\n` terminators and Windows double-terminator edge case
- Overflow protection: bytes silently dropped when buffer approaches capacity

---

## Key Concepts Practiced
- Two-gate interrupt enable: peripheral (DIER/CR1/IMR) + NVIC (ISER)
- ISR flag clearing — write-1-to-clear (EXTI PR) vs write-0-to-clear (TIM SR)
- `volatile` for ISR-shared variables
- Circular buffer head/tail decoupling producer (ISR) from consumer (main)
- NVIC ISER register banking: ISER0 (IRQ 0–31), ISER1 (IRQ 32–63)

---

## Hardware
| Signal | Pin | Connector |
|--------|-----|-----------|
| LED | PA10 | D2 |
| Button | PB10 | D6 (breadboard, 10kΩ pull-down) |
| USART2 TX | PA2 | D1 (routed to ST-LINK VCP) |
| USART2 RX | PA3 | D0 (routed to ST-LINK VCP) |