# Exercise 10 — Washing Machine Implementation Plan

## Hardware Mapping (final)

| Function          | Arduino | AVR  | Driver/Timer        |
|-------------------|---------|------|---------------------|
| Start button      | D2      | PD2  | INPUT_PULLUP        |
| Cancel button     | D3      | PD3  | INPUT_PULLUP        |
| Door contact      | D4      | PD4  | INPUT_PULLUP        |
| Water level       | A0      | PC0  | ADC channel 0       |
| Pump (relay)      | D8      | PB0  | active LOW          |
| Water valve servo | D9      | PB1  | Timer1 OC1A (50Hz)  |
| Motor (L298N ENA) | D5      | PD5  | digital on/off      |
| Buzzer            | D11     | PB3  | Timer2 OC2A (500Hz) |
| Status LED 1      | D12     | PB4  | digital output      |
| Status LED 2 / HB | D13     | PB5  | digital output      |

## Phase 1 — Pin remapping & drivers
- [x] Update `gpio.h` — remap all pins to final spec
- [x] Wiring on physical breadboard
- [x] Update `gpio.c` — `gpio_init` for new layout, `readDoor()`, `readStartButton()`, `readCancelButton()`
- [x] New `adc.c/.h` — `adc_init()`, `adc_read(channel)`
- [x] New `actuators.c/.h` — `valveOpen/Close()`, `pumpOn/Off()`, `motorStart/Stop()`
- [x] New `servo.c/.h` — `servo_init()`, `servo_setAngle(angle)` via Timer1 OC1A
- [x] Update `buzzer.c/.h` — remap to Timer2 OC2A (D11/PB3)

## Phase 2 — New tasks
- [x] Add `taskDoor` — edge detect on PD4, post `EV_DOOR_UNLOCKED`/`EV_DOOR_CLOSED`
- [x] Add `taskWater` — ADC read A0, post `EV_WATER_LEVEL_OK` when threshold reached
- [x] Add `taskStatusLEDs` — update status LEDs based on `wm_fsm_get_state()`
- [x] Rename/adjust existing tasks to match spec periods
- [x] Extend `taskSerialInput` to full command shell (`start`, `cancel`, `status`, `fault`)

## Phase 3 — New FSM
- [ ] New `motor_fsm.c/.h` — table-driven, states: STOPPED/STARTING/SLOW/FAST/ERROR
- [ ] Wire `motor_fsm` into `event_handler.c`

## Phase 4 — Update existing FSMs
- [x] Update `wm_fsm.c` — add actuator calls in entry/exit actions
- [x] Update `door_fsm.c` — adapt for physical door sensor
- [x] Review all `EV_DOOR_UNLOCKED` vs `EV_DOOR_OPEN` handling
- [x] Add `NDEBUG` guards to all debug prints

## Phase 5 — Quality
- [x] Apply BARR-C conventions file by file
- [x] Add CPU idle counter
- [x] Functional test — full wash cycle simulation
- [x] Review checklist audit