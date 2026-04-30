# Week 05 — Host-Side UART Command Parser

A host-side skeleton for the UART command parser that will run on the STM32 in Week 8.
Built in plain C, compiled and tested on macOS — no hardware required.

## Purpose

Validates the command parsing logic and function pointer dispatch table before porting to bare-metal STM32.
Input is read from `stdin` to simulate a UART receive buffer.

## Commands

| Command       | Argument   | Description                      |
|---------------|------------|----------------------------------|
| `led on`      | —          | Simulates turning LED on         |
| `led off`     | —          | Simulates turning LED off        |
| `pwm <0-100>` | integer    | Simulates setting PWM duty cycle |
| `status`      | —          | Simulates status query           |

## Build

```bash
make
```

Binary is output to `bin/UART_parse`, object files to `obj/`.

## Run

```bash
./bin/UART_parse
```

Type a command when prompted, e.g.:

```
led on
pwm 75
status
```

## Clean

```bash
make clean
```

## Design Notes

- Commands are dispatched via a **function pointer table** (`command_t` struct array) — the same pattern used in the STM32 port
- `parse_command()` accepts a raw input string and handles splitting internally, matching the UART receive buffer interface
- `strtol` used instead of `atoi` for PWM argument validation to catch non-numeric input
- `BUFFERSIZE` defined in `parser.h` — controls both the input buffer in `main.c` and the internal parse buffers in `parser.c`