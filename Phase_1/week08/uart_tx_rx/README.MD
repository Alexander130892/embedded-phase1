# Week 08 — Bare-Metal UART RX + Command Parser

## What this does

Implements a UART receive driver and interactive command parser on the STM32F446RE (NUCLEO board), building on the TX driver from Week 07. Commands are typed over a serial terminal at 115200 8N1 and dispatched to hardware handlers via a function pointer table.

## Supported commands

| Command     | Action                          |
|-------------|---------------------------------|
| `led on`    | Sets PA10 high (LED on)         |
| `led off`   | Clears PA10 low (LED off)       |
| `status`    | Reports current LED state       |
| `pwm <val>` | Validates 0–100 (stub only)     |

## File structure

```
Inc/
  stm32f446re.h   — base addresses and register offsets for the target MCU
  uart.h          — UART driver API
  parser.h        — command parser API
  led.h           — LED driver API
Src/
  main.c          — peripheral init, main receive/parse loop
  uart.c          — uart_send_char, uart_send_string, uart_receive_char, uart_receive_string
  parser.c        — command tokenisation, dispatch table, handlers
  led.c           — led_init, led_on, led_off, led_get_state
```

## Key concepts practised

- Polling RXNE (SR bit 5) for receive, mirroring TXE polling from Week 07
- Enabling RE (CR1 bit 2) and configuring PA3 as AF7 for USART2_RX
- Newline-terminated string receive with overflow protection
- Separating peripheral drivers from application logic
- Centralising MCU base addresses in a single device header

## Hardware

- Board: STM32F446RE NUCLEO
- TX: PA2 (AF7), RX: PA3 (AF7)
- LED: PA10 (GPIO output)
- Baud rate: 115200, 16 MHz clock, OVER8=0, BRR=0x8B

## Tested with

-`screen` on macOS at 115200 8N1