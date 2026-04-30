# Week 11 — PWM + LED Fade

**Board:** NUCLEO-F446RE (STM32F446RE, Cortex-M4)  
**Toolchain:** arm-none-eabi-gcc, OpenOCD, VS Code

---

## What this project does

Configures TIM2 in PWM mode 1 on PA5 (TIM2_CH1) and drives an LED with a continuous fade in/out — all bare-metal, no HAL.

---

## Key concepts covered

- Alternate function GPIO config (MODER, AFRL)
- Timer clock chain: PSC → CNT → ARR
- PWM mode 1 via TIMx_CCMR1 (OC1M bits)
- CCR preload enable (OC1PE) to prevent mid-cycle glitches
- Channel output enable via TIMx_CCER (CC1E)
- Shadow register initialisation via UG bit in TIMx_EGR
- Software millisecond timer using TIM2 overflow counter

---

## Timer configuration

| Parameter | Value | Result |
|-----------|-------|--------|
| Input clock | 16 MHz | APB1 default |
| PSC | 159 | 100,000 Hz after prescaler |
| ARR | 99 | 1,000 Hz PWM frequency |
| CCR | 0–99 | 0–99% duty cycle |

**Duty cycle formula:** `CCR = duty% / 100 * (ARR + 1)`

---

## Fade loop

CCR steps from 0 → 99 → 0 with a 10 ms delay per step, giving a ~2 second full fade cycle.

---

## Notes

- FPU not initialised — avoid explicit `(float)` casts; use `100.0` literal to promote to double instead
- `uint32_t` underflow was a real bug: `fade >= 0` wraps to 4 billion; fixed with `fade > 0` and pre-decrement `--fade`
- UART integration and module refactor (`gpio.c`, `uart.c`, `timer.c`) deferred to Week 12