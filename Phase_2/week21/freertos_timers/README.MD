# Week 21 — Software Timers + Task Notifications

## Goal

Implement software timers and task notifications on STM32 (Nucleo, Cortex-M4) with FreeRTOS, culminating in a runtime-configurable sensor polling rate controlled via UART commands.

**M11 checkpoint:** Dynamic logging rate configurable over UART at runtime.

## Scope

- FreeRTOS software timers (auto-reload, `xTimerCreate`, `xTimerChangePeriod`)
- Task notifications as a lightweight alternative to semaphores (`xTaskNotifyGive` / `ulTaskNotifyTake`)
- Interrupt-driven UART receive with command parsing
- Watchdog-style heartbeat monitoring of the MPU6050 sensor task

## Architecture

```
┌──────────────┐
│  MPU6050      │
│  Sensor       │
└──────┬───────┘
       │ I2C
┌──────▼───────┐  notification   ┌────────────────┐
│ vSensorRead  │◄────────────────│ xTimerSensor   │  (FreeRTOS software timer)
│ Task         │                 │ callback:       │
│              │                 │ xTaskNotifyGive │
└──────┬───────┘                 └────────────────┘
       │ xQueue1 (mpu6050_data_t)        ▲
       │                                  │ xTimerChangePeriod
┌──────▼───────┐                 ┌────────┴───────┐
│ vLogTask     │                 │ vUART_Rx_Task  │◄── xQueueUARTRx (char)
│ (Process)    │                 │ (Parse "log N")│         ▲
└──────┬───────┘                 └────────────────┘         │
       │ xQueue2 (char[])                          ┌───────┴────────┐
       │                                            │ USART2_IRQHandler│
┌──────▼───────┐  notification   ┌────────────────┐ └────────────────┘
│ vUARTTask    │◄────────────────│ Watchdog Timer │
│ (Transmit)   │  (error alert)  │ callback:      │
│              │                 │ checks b_timeout│
└──────────────┘                 └────────────────┘
       │
       ▼ UART TX (xMutexUart protected)
```

### Key design decisions

**Software timer replaced hardware timer (TIM2) for sensor polling.**
TIM2 previously triggered an ISR that notified the sensor task via semaphore. Refactored to a FreeRTOS software timer so `xTimerChangePeriod` could be used for runtime rate changes. This also moved the trigger from ISR context to daemon task context, simplifying the notify call (`xTaskNotifyGive` instead of `vTaskNotifyGiveFromISR`).

**Task notifications replaced binary semaphore on the sensor path.**
The ISR-to-sensor-task signal is a textbook notification use case: one sender, one known receiver. The notification makes intent explicit in the code — `xTaskNotifyGive(xSensorTaskHandle)` names the target directly. Semaphores remain in use for the UART mutex where multiple tasks contend for the same resource.

**Watchdog error reporting via notification to UART task.**
The watchdog timer callback cannot safely call `uart_send_string` (would block the daemon task, starving all other timers). Instead it sends a notification to the UART task, which prints the error on its own schedule. This required a short timeout (100 ticks) on `xQueueReceive` in the UART task instead of `portMAX_DELAY`, so the notification check runs periodically.

**Sensor data validation gates the heartbeat.**
The heartbeat flag (`b_timeout`) is only cleared when the MPU6050 returns non-zero accelerometer data. With the sensor unplugged, I2C returns all zeros — the watchdog correctly detects this as a failure since at least one accelerometer axis should always read gravity.

**UART RX is interrupt-driven with a dedicated char queue.**
Polling for UART input is inappropriate in a multithreaded environment. Characters arrive via `USART2_IRQHandler` into `xQueueUARTRx` (item size: 1 byte). A single task accumulates characters until `\r`, null-terminates, and parses with `strtok`.

## Lessons Learned

- **Timer daemon context ≠ ISR context.** Blocking in a timer callback is legal but destructive — it stalls the shared daemon task and starves every other timer. The constraint is architectural (shared resource), not hardware (no task context).
- **Task notification state lives in the TCB.** This is why only one task can wait on a given notification — there's no external object for multiple tasks to pend on. The one-waiting-task "rule of thumb" falls directly out of the implementation.
- **`TaskHandle_t` is already a pointer.** Declaring `TaskHandle_t*` creates a double pointer. `xTaskNotifyGive` takes the handle directly, not `&handle`.
- **`strtok` second call takes `NULL`.** Passing the original string again restarts tokenization.
- **`atoi` returns 0 on invalid input.** A lower bound (100 ms) protects against nonsense or malformed commands being passed to `xTimerChangePeriod`.
- **Queue item size must match what's being sent.** Can't reuse a `mpu6050_data_t` queue for single `char` items.

## Open Items

- **Sensor recovery:** Currently the watchdog detects failure and reports it. A real system would attempt re-initialization of the MPU6050 after a timeout.
- **Upper bound on period:** Only a lower bound (100 ms) is enforced. An upper bound could prevent accidentally setting a multi-minute polling interval.
- **Command expansion:** The parser only handles `log <period>`. Could add `log stop` / `log start`, sensor status queries, or watchdog configuration.
- **UART TX from watchdog:** Currently notifications are one-shot. If the sensor stays dead, only one error prints. Could add repeated alerts or a distinct error state.