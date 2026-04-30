# uart_pwm_commander

**Board:** NUCLEO-F446RE (STM32F446RE, Cortex-M4)  
**Toolchain:** arm-none-eabi-gcc, OpenOCD, VS Code  
 PWM + UART Integration (Phase 1 capstone)

---

## Project overview

Full bare-metal integration project built from an empty project — no HAL, no CubeMX. Accepts UART commands over serial to control LED brightness live via PWM, while a hardware timer interrupt drives a background 1 Hz blink. Every peripheral is configured by direct register writes.

**Features:**
- Live PWM duty cycle control via UART command (`pwm <0-100>`)
- LED on/off commands (`led on`, `led off`)
- 1 Hz hardware blink toggle (`blink`) — duty cycle preserved during blink
- Interrupt-driven UART RX with circular buffer — main loop never blocks on input
- Software millisecond timer derived from TIM2 overflow

---

## Hardware setup

**Components:**
- NUCLEO-F446RE (onboard ST-Link V2, USB connection)
- External LED + 330Ω resistor on PA0

**Wiring:**

```
NUCLEO-F446RE
                                    220Ω
PA0 (TIM2_CH1, AF1) ──────────────[===]────── LED ────── GND
PA2 (USART2_TX, AF7) ─────────── USB-Serial (ST-Link onboard)
PA3 (USART2_RX, AF7) ─────────── USB-Serial (ST-Link onboard)
```

**Serial settings:** 115200 baud, 8N1 (use any terminal — PuTTY, minicom, screen)

---

## Build & flash

**Build:**
```bash
make
```

**Flash with OpenOCD:**
```bash
openocd -f interface/stlink.cfg \
        -f target/stm32f4x.cfg \
        -c "program build/uart_pwm_commander.elf verify reset exit"
```

**Or flash with STM32CubeProgrammer:**
```
File → Open: build/uart_pwm_commander.elf → Download
```

---

## Architecture

```
main.c          — init sequence, non-blocking main loop
gpio.c/.h       — GPIO clock and pin configuration
uart.c/.h       — USART2 init, interrupt-driven RX, circular buffer, polling TX
timer.c/.h      — TIM2 init, PWM config, blink ISR, millis(), CCR helpers
parser.c/.h     — command parser, PWM and LED command handlers
```

---

## Timer configuration

| Parameter | Value | Result |
|-----------|-------|--------|
| Input clock | 16 MHz | APB1 default, no PLL |
| PSC | 159 | 100,000 Hz after prescaler |
| ARR | 99 | 1,000 Hz PWM frequency, 1 ms overflow |

**Duty cycle formula:** `CCR = duty% * (ARR + 1) / 100`

---

## Design decisions

**Polling TX, interrupt RX**
TX is host-initiated — the wait for TXE is a handful of cycles and never blocks anything meaningful. RX is the opposite: you cannot predict when a character arrives, so polling RXNE would stall the main loop. RXNEIE fires on each received byte, the ISR pushes to a circular buffer, and the main loop consumes when a newline is detected.

**Circular buffer**
`head` is advanced by the ISR, `tail` is advanced by the main loop. No locking needed here — single producer, single consumer, and `head`/`tail` are `volatile`. Buffer size is 64 bytes, sufficient for the command set.

**Blink via CC1E toggle, not CCR**
`led_blink()` toggles the CC1E bit in TIMx_CCER rather than writing CCR to 0. This disconnects the timer output from the pin without touching the duty cycle register — so `pwm 75` followed by `blink` preserves 75% duty when blink re-enables the output.

**Integer arithmetic for duty cycle**
Multiply before divide (`duty * (ARR+1) / 100`) to avoid truncation to zero. Avoids floating point entirely — the FPU is not initialised on this target, and explicit `float` casts caused undefined behaviour.

**UG bit before CEN**
Writing UG=1 in TIMx_EGR forces an update event before the counter starts, loading PSC and ARR into their shadow registers. Without this, the timer could start with stale hardware register contents.

---

## Lessons learned / what I'd do differently

- **Define variables in `.c`, declare in `.h`** — putting `head` and `tail` definitions in the header gave every translation unit its own copy; linker multiple-definition error.
- **`static` local variables for persistent state** — `index` in `uart_receive_string` must survive between calls to accumulate a partial line; a local resets every call.
- **Clear interrupt flags before acting, not after** — reduces the window for a re-entrant flag miss, especially relevant if the ISR body grows.
- **Next time:** use a proper ring buffer abstraction with a `line_ready` flag rather than scanning for newlines inside the receive function; cleaner separation of concerns.

---

## Register reference (key writes)

| Purpose | Register | Bit(s) |
|--------|----------|--------|
| TIM2 clock | RCC_APB1ENR | bit 0 |
| GPIOA clock | RCC_AHB1ENR | bit 0 |
| PA0 AF mode | GPIOA_MODER | [1:0] = 10 |
| PA0 AF1 (TIM2_CH1) | GPIOA_AFRL | [3:0] = 0001 |
| PWM mode 1 | TIMx_CCMR1 | OC1M [6:4] = 110 |
| Preload enable | TIMx_CCMR1 | OC1PE bit 3 |
| Channel output enable | TIMx_CCER | CC1E bit 0 |
| Shadow register init | TIMx_EGR | UG bit 0 |
| Update interrupt enable | TIMx_DIER | UIE bit 0 |
| TIM2 NVIC enable | NVIC_ISER0 | bit 28 |
| RXNE interrupt enable | USART_CR1 | RXNEIE bit 5 |
| USART2 NVIC enable | NVIC_ISER1 | bit 6 |