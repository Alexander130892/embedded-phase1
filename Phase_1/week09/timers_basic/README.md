# Week 09 — Timers (Basic)

Bare-metal TIM2 exercises on the NUCLEO-F446RE using direct register access.

## Exercises

### 1. 1 Hz Tick
Configure TIM2 in up-counting mode to generate a 1 Hz update event.
- PSC = 15999, ARR = 999 → 16 MHz / 16000 / 1000 = 1 Hz
- Poll UIF flag (TIMx_SR bit 0) to toggle LED on PA10

### 2. millis()
Implement a `millis()` function returning milliseconds since startup.
- `overflow_count` incremented on each UIF event
- `millis() = overflow_count * 1000 + CNT`

### 3. Non-blocking Delays
Replace busy-wait for-loop delays with timer-based delays using `millis()`.
- Uses absolute deadlines (`start += interval`) to prevent drift

## Key Registers
| Register | Offset | Purpose |
|----------|--------|---------|
| TIMx_CR1 | 0x00 | Counter enable (CEN), direction (DIR) |
| TIMx_SR  | 0x10 | Update interrupt flag (UIF) |
| TIMx_EGR | 0x14 | Force update event (UG) |
| TIMx_CNT | 0x24 | Current counter value |
| TIMx_PSC | 0x28 | Prescaler |
| TIMx_ARR | 0x2C | Auto-reload value |