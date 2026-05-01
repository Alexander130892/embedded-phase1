# Traffic Light FSM

Bare-metal STM32F446RE implementation of a two-direction traffic light controller using a Moore FSM. Direct register access, no HAL.

## Hardware

### Wiring

```
NUCLEO-F446RE
                                    
  PC5  ──[ R ]──┬── North Green     
  PC6  ──[ R ]──┼── North Yellow    
  PC8  ──[ R ]──┴── North Red       
                                    
  PC3  ──[ R ]──┬── East Green      
  PC2  ──[ R ]──┼── East Yellow     
  PC13 ──[ R ]──┴── East Red        

  PB10 ──[ 10kΩ ]── GND   North sensor (push button)
  PB9  ──[ 10kΩ ]── GND   East sensor  (push button)

  [R] = current limiting resistor (220Ω)
```

### Peripherals
- **GPIOB** — push button inputs (PB9, PB10), internal pull-down
- **GPIOC** — LED outputs (PC2, PC3, PC5, PC6, PC8, PC13)
- **TIM2** — 1 ms tick via update interrupt, overflow counter drives FSM timing

---

## FSM

### Inputs
| Bit 1 (PB10) | Bit 0 (PB9) | Meaning |
|:---:|:---:|---|
| 0 | 0 | No traffic |
| 0 | 1 | East traffic only |
| 1 | 0 | North traffic only |
| 1 | 1 | Traffic both directions |

### States
| State | North | East | Duration | Next state (00/01/10/11) |
|---|---|---|---|---|
| goN   | Green  | Red    | 5000 ms | goN / waitN / goN / waitN |
| waitN | Yellow | Red    | 1000 ms | goE / goE / goE / goE     |
| goE   | Red    | Green  | 5000 ms | goE / goE / waitE / waitE |
| waitE | Red    | Yellow | 1000 ms | goN / goN / goN / goN     |

### State Diagram
```
        no N traffic          N traffic detected
   ┌──────────────────┐    ┌─────────────────────┐
   ▼                  │    │                     ▼
 [goN] ───────────────┴────┴───────► [waitN] ──► [goE] ─► [waitE] ──► [goN]
   ▲                                                          │
   └──────────────────────────────────────────────────────────┘
```

---

## Output Byte Mapping
```c
const uint8_t pin_map[6] = {
    13,  // bit 0 = East Red
     2,  // bit 1 = East Yellow
     3,  // bit 2 = East Green
     8,  // bit 3 = North Red
     6,  // bit 4 = North Yellow
     5   // bit 5 = North Green
};
```

| State | Output byte | Binary   |
|-------|-------------|----------|
| goN   | 0x21        | 00100001 |
| waitN | 0x11        | 00010001 |
| goE   | 0x0C        | 00001100 |
| waitE | 0x0A        | 00001010 |

## File Structure
```
traffic_light/
├── Src/
│   ├── main.c      — FSM logic, state table, init
│   ├── led.c       — GPIO init and output functions
│   └── timer.c     — TIM2 init, ISR, overflow counter
├── Inc/
│   ├── stm32f446re.h
│   ├── led.h
│   └── timer.h
```