# Week 07 — Bare-Metal UART TX on STM32F446RE

## Objective
Implement UART transmission from scratch on the NUCLEO-F446RE using direct register writes — no HAL, no CMSIS.

## Hardware
- STM32 NUCLEO-F446RE (Cortex-M4, 16 MHz internal clock)
- PA2 — UART TX (USART2, AF7)
- PA10 — LED output (reserved for Week 08 command parser)

## Key Concepts

**Clock configuration**
- GPIOA clock enabled via `RCC_AHB1ENR` (bit 0)
- USART2 clock enabled via `RCC_APB1ENR` (bit 17)

**GPIO configuration**
- PA2 set to Alternate Function mode (`MODER = 0b10`)
- AF7 selected in `GPIOA_AFRL` bits [11:8] → connects PA2 to USART2 TX hardware
- PA10 set to GPIO output mode (`MODER = 0b01`)

**Baud rate calculation**
- Internal clock: 16 MHz, OVER8 = 0 (16x oversampling)
- Formula: `USARTDIV = f_clk / (8 × (2 - OVER8) × BR)`
- USARTDIV = 8.6875 → Mantissa = 8 (`0x8`), Fraction = 0.6875 × 16 = 11 (`0xB`)
- `BRR = 0x8B` for 115200 baud

**USART2 init sequence**
1. Write `BRR`
2. Set `TE` (bit 3 in CR1) — enable transmitter
3. Set `UE` (bit 13 in CR1) — enable USART

## Functions

```c
void uart_send_char(uint8_t c);    // Polls TXE flag, writes to DR
void uart_send_string(char* str);  // Walks string, calls uart_send_char per byte
```

**TXE flag** (SR bit 7): goes high when the data register is empty and ready for the next byte. Always poll before writing to DR.

**Line endings**: use `\r\n` over raw UART — serial terminals do not translate `\n` to `\r\n` the way a host OS does.
