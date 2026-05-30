# UART Communication — Arduino Uno ↔ STM32F446RE

## Setup

```
Arduino Uno TX (PD1)  ──→  STM32 UART4 RX (PA1)
Arduino Uno RX (PD0)  ←──  STM32 UART4 TX (PA0)
STM32 USART2 TX (PA2) ──→  ST-Link ──→ PC Terminal
GND ──────────────────────────────────────── GND
USB ──→ STM32 ──→ 5V ──→ Arduino 5V pin
```

## Configuration

| Parameter | Value |
|---|---|
| Baud rate | 9600 |
| Frame format | 8N1 |
| Arduino clock | 16 MHz |
| STM32 clock | 16 MHz (HSI default) |
| BRR (both) | 0x683 (mantissa 104, fraction 3) |

## Arduino (AVR bare-metal)

- **TX:** Interrupt-driven via `USART_UDRE_vect` — sends `0x01`, `0x02`, `0x03`
- **RX:** Interrupt-driven via `USART_RX_vect` — receives `0x00`/`0x01`, toggles built-in LED (PB5)
- No Arduino runtime — direct register access (`UCSR0B`, `UDR0`, `UBRR0`)

## STM32 (bare-metal register access)

- **UART4 RX (PA1, AF8):** Receives bytes from Arduino via RXNEIE interrupt, stores in circular buffer
- **UART4 TX (PA0, AF8):** Sends `0x00`/`0x01` to Arduino every 500ms via TIM2 interrupt
- **USART2 TX (PA2, AF7):** Forwards received bytes to PC terminal for monitoring
- **TIM2:** PSC=15999, ARR=499 → 2 Hz interrupt → 500ms toggle interval

## Key learnings

**BRR fractional divider** — STM32 BRR splits into mantissa [15:4] and fraction [3:0]:
```
USARTDIV = fCLK / (16 × BAUD)
mantissa = integer part
fraction = fractional part × 16, rounded
```
Omitting the fractional part causes subtle baud rate errors and corrupted characters.

**Two UARTs, clean separation:**
- UART4 = device communication channel (Arduino)
- USART2 = debug/monitor channel (terminal)

**Interrupt-driven on both sides** — no polling, no blocking. TIM2 ISR controls TX interval, RXNEIE ISR handles incoming data, circular buffer decouples ISR from main loop.

**Logic analyzer** — essential for isolating which side of a UART link is at fault. Confirmed Arduino TX was correct before investigating STM32 RX.