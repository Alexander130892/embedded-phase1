# Week 23 — FreeRTOS Integration: Multi-sensor Logger

## Goal

Port the Week 17 bare-metal multi-sensor logger into a full FreeRTOS task architecture. Four concurrent tasks handle sensor reading, data logging, command parsing, and watchdog supervision. All inter-task data passes through queues; shared hardware is protected by a mutex; timing is driven by a mix of hardware and software timers.

**M12 checkpoint:** Full multi-sensor FreeRTOS logger running in a stable manner — verified with a 120-minute continuous run (774,104 lines, zero errors, zero UART corruption).

## Scope

- FreeRTOS task architecture (four tasks, all at equal priority)
- Queue-based inter-task data passing
- Mutex protection of shared UART peripheral
- Task notifications (ISR→task and timer→task)
- Hardware timer (TIM2) vs software timer trade-offs
- Software timer callbacks and the timer daemon task
- Watchdog pattern using per-sensor boolean flags
- UART RX interrupt feeding a command queue
- `strtok`-based command parser with argument handling
- Stack and heap profiling with `uxTaskGetSystemState` and `xPortGetFreeHeapSize`
- 30-minute stability testing with automated serial logging

## Architecture

```
                    TIM2 IRQ (100 Hz)
                         │
                         │ xTaskNotifyGiveFromISR
                         ▼
                   ┌─────────────┐
                   │  vI2CTask   │  reads MPU-6050 via I2C
                   │  (100 Hz)   │  sets b_timeout_mpu6050 = false on good read
                   └──────┬──────┘
                          │ xQueueSend → xQueue_mpu6050 (depth 5)
                          │
    SW Timer (1 Hz)       │
    xTaskNotifyGive       │
         │                │
         ▼                │
   ┌─────────────┐        │
   │  vSPITask   │  reads BMP280 via SPI
   │   (1 Hz)    │  sets b_timeout_bmp280 = false on good read
   └──────┬──────┘
          │ xQueueSend → xQueue_bmp280 (depth 5)
          │                │
          └────────────────┼──────────────────┐
                           ▼                  │
                    ┌─────────────┐           │
                    │  vLogTask   │           │
                    │             │◄──────────┘
                    │  - dequeues both sensors
                    │  - formats CSV over UART
                    │  - checks watchdog flags
                    └──────┬──────┘
                           │ xSemaphoreTake/Give → xMutexUart
                           ▼
                        UART TX

    SW Timer (5 Hz)
    vHeartBeatTimer
         │
         ├─ sets b_timeout_mpu6050 = true (if not already true)
         ├─ sets b_timeout_bmp280  = true (if not already true)
         └─ xTaskNotifyGive → xLogTaskHandle (if flag still true = sensor dead)

    USART2 IRQ
         │ xQueueSendFromISR → xQueueUARTRx (depth 20)
         ▼
   ┌─────────────┐
   │  vCmdTask   │  parses commands: log on/off, hwm, mem, status
   └─────────────┘
```

## Key design decisions

**Hardware timer for MPU-6050, software timer for BMP280.** The MPU-6050 runs at 100 Hz — jitter-free timing matters for IMU data integrity. TIM2 is already configured at exactly 100 Hz (PSC=1599, ARR=99, 16 MHz clock) and fires a hardware ISR that calls `xTaskNotifyGiveFromISR`. The BMP280 runs at 1 Hz — a few milliseconds of jitter from the FreeRTOS software timer is irrelevant for a pressure/temperature sensor. Using a software timer for BMP280 avoids consuming a second hardware timer.

**Software timer callbacks must not block.** Timer callbacks execute in the timer daemon task context. Blocking inside a callback (e.g. waiting for DMA completion or taking a mutex) would starve all other software timers. All timer callbacks are therefore minimal: either `xTaskNotifyGive` or flag manipulation only.

**Task notification over binary semaphore for wake-up signals.** `xTaskNotifyGiveFromISR` / `ulTaskNotifyTake` is lighter than a binary semaphore — no separate kernel object to create, and the notification is sticky (won't be lost if the task is briefly busy elsewhere). Used for both the TIM2→vI2CTask and BMP280 timer→vSPITask paths.

**Two-flag watchdog pattern.** Each sensor task sets its own `volatile bool` flag (`b_timeout_mpu6050`, `b_timeout_bmp280`) to `false` after a confirmed successful read. The heartbeat timer sets each flag to `true` on every callback; if it finds a flag already `true` on the next callback, the sensor has missed a cycle and `vLogTask` is notified. `vLogTask` checks which flag is still `true` to identify the failing sensor. `volatile` is sufficient here — ARM Cortex-M guarantees atomic single-byte writes, and the flag is only written (never read-modify-written) by the sensor tasks.

**Status check over zero-value check for I2C liveness.** The original implementation used `accel_x == 0 && accel_y == 0 && accel_z == 0` to detect bad reads. This is unreliable — stale queue data passes the check even after the sensor is unplugged. Replaced with checking the return value of `mpu6050_read_data()` — `STATUS_OK` means a fresh read succeeded.

**SPI liveness limitation acknowledged.** SPI is a push-pull bus — the master always drives the clock and a transfer "succeeds" even with no slave present, returning floating MISO data. A sanity range check (`0 < temp < 85°C`) is used as a pragmatic interim solution. The proper approach is periodic `WHO_AM_I` register readback (expected `0x58`), deferred to a future week.

**Heap and stack sizing.** Initial heap was set to 75 KB (overly conservative). After profiling with `hwm` and `mem` at the end of the stability run, stack sizes were reduced based on HWM readings plus 25% headroom. Heap reduced to 25 KB — actual usage ~18 KB with margin for fragmentation.

## Lessons Learned

- **SW timer callbacks run in the timer daemon task** — blocking inside one (e.g. waiting for a semaphore) starves all other SW timers in the system.
- **`configTOTAL_HEAP_SIZE` too small causes `xTaskCreate` to return `-1` (`errCOULD_NOT_ALLOCATE_REQUIRED_MEMORY`)** — the task simply never runs with no other indication.
- **`portMAX_DELAY` in a tight loop without a blocking call starves other tasks** — `ulTaskNotifyTake(pdTRUE, portMAX_DELAY)` is correct; returning immediately on no notification and spinning is not.
- **`volatile` is sufficient for single-byte atomic flags on Cortex-M** — ARM guarantees atomic aligned word/byte writes. A mutex is not needed for a flag that is only ever written, never read-modify-written.
- **STM32 SR register clearing: read the datasheet** — `rc_w0` registers (write 0 to clear) use direct write of complement; `rc_w1` registers use direct write of the bit; `rw` registers use read-modify-write. A fixed pattern applied blindly is wrong.
- **`strtok` modifies the source string in place** — capture the first token into a separate `char* cmd` pointer before the `if/else` chain; using `command` directly after `strtok` compares against `"log"` not the full command.
- **HWM values should be read at the *end* of a stability test**, not the beginning — they only move in one direction and capture worst-case stack depth over the full run.
- **Bare-metal I2C spin-loop timeouts block the FreeRTOS scheduler** — lowering `MAX_TIMEOUT` to ~10000 iterations (~0.6 ms at 16 MHz) keeps the driver portable (no FreeRTOS dependency) while bounding the stall duration. `vTaskDelay` lives in the task wrapper, not the driver.
- **TIM2 must be enabled inside `vI2CTask`**, not in `main()` — the ISR can fire before the task exists and notify a null handle, causing a fault.
- **`xTaskCreate` parameter copy-paste errors are silent** — passing `mpu_addr` to `vSPITask` instead of `NULL` caused no compile error; always verify task parameters match the function signature.

## Open Items

- **BMP280 `WHO_AM_I` liveness check** — replace the temperature sanity range check with periodic readback of the chip ID register (`0x58`). Use `xTaskNotify` with a value to distinguish "read temp" vs "verify" notifications, received via `xTaskNotifyWait` in `vSPITask`.
- **Run-time stats** — `configGENERATE_RUN_TIME_STATS` requires a dedicated high-resolution free-running timer (not TIM2, which is stopped on `log off`). Worth adding a TIM3 or TIM4 at ~10× tick rate as a stats timer.

- **I2C driver FreeRTOS awareness** — long-term, replacing spin-loop waits with event-driven I2C (interrupt + semaphore) would eliminate the blocking window entirely without compromising driver portability.