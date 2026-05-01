# avr-baremetal

Bare-metal ATmega328P (Arduino Uno) — no Arduino runtime, no HAL.

## Prerequisites

```bash
# Ubuntu / Debian (including WSL)
sudo apt install gcc-avr avr-libc avrdude

# macOS
brew install avr-gcc avrdude

# Windows
# Install WinAVR or use the toolchain bundled inside Arduino IDE:
# C:\Program Files\Arduino\hardware\tools\avr\bin\
```

## Project structure

```
avr-baremetal/
├── main.c          ← your application (bare register access)
├── Makefile        ← build + flash targets
└── .vscode/
    ├── c_cpp_properties.json   ← IntelliSense for avr-gcc headers
    └── tasks.json              ← Ctrl+Shift+B → Build / Flash
```

## Workflow

```bash
make build    # compile → main.elf + main.hex, print size report
make flash    # build + upload via avrdude (optiboot bootloader)
make size     # AVR memory breakdown (Flash / SRAM / EEPROM)
make disasm   # disassemble ELF — great for understanding codegen
make clean    # remove build artifacts
```

> **Port:** edit `PORT` in the Makefile to match your system  
> Linux: `/dev/ttyACM0` · macOS: `/dev/cu.usbmodem*` · Windows: `COMx`

## What the blink does (register-level)

| Step | Code | Why |
|---|---|---|
| Set PB5 as output | `DDRB \|= (1 << PB5)` | DDR: 1=output, 0=input |
| LED on | `PORTB \|= (1 << PB5)` | Drive pin high |
| LED off | `PORTB &= ~(1 << PB5)` | Drive pin low |
| Wait | `_delay_ms(500)` | Busy-loop, depends on F_CPU |

## STM32 → ATmega328P cheat sheet

| Concept | STM32 | ATmega328P |
|---|---|---|
| Compiler | `arm-none-eabi-gcc` | `avr-gcc` |
| Flasher | ST-Link / OpenOCD | `avrdude` (bootloader) |
| GPIO direction | `GPIOx->MODER` | `DDRx` |
| GPIO write | `GPIOx->ODR` | `PORTx` |
| GPIO read | `GPIOx->IDR` | `PINx` |
| Clock enable | `RCC->AHB1ENR` | *(not needed — always on)* |
| Interrupt vectors | `startup_stm32*.s` | `avr/interrupt.h` macros |
| Linker script | `.ld` file | handled by `-mmcu` flag |

## Next steps

- **Timer interrupt blink** — replace `_delay_ms` with Timer1 CTC interrupt
- **UART** — `UBRR0H/L`, `UDR0` without `Serial.print()`
- **SPI / I2C** — `SPCR`, `TWCR` raw registers
- **PWM** — `TCCR0A/B`, `OCR0A` for fast/phase-correct PWM