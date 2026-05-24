# Week 19 — FreeRTOS Queues & Inter-task Communication

## Pipeline Architecture
vSensorReadTask → [xQueue1] → vLogTask → [xQueue2] → vUARTTask → UART TX
(Acquire)                     (Process)               (Transmit)

## Architectural Decisions

### Double queue pattern
A three-stage pipeline was chosen even though a single queue would suffice for this 
exercise. The pattern decouples acquire, process, and transmit stages — if processing 
becomes expensive (e.g. filtering, unit conversion, JSON formatting), vLogTask can 
fall behind without blocking the sensor read. Each stage runs at its own pace.

### Passing device address via pvParameters
The MPU6050 I2C address is passed to vSensorReadTask via pvParameters cast to void*. 
This allows spawning multiple instances of the same task for different sensor addresses
without hardcoding via macro.
Cast pattern: (void*)(uintptr_t)addr on create, (uint8_t)(uintptr_t)pvParameters on receive.

### Queue item types
- xQueue1: sizeof(mpu6050_data_t) — struct copied by value into queue
- xQueue2: sizeof(char) * STR_LEN — full string buffer copied by value

FreeRTOS queues copy by value, so stack-allocated items are safe to reuse after xQueueSend.

### Stack sizing
All tasks set to 512 words after measuring with uxTaskGetStackHighWaterMark():
- SensorReadTask HWM: 253 — I2C burst read + float math
- LogTask HWM: 392 — snprintf with float formatting is stack-heavy
- UARTTask HWM: 380 — polling transmit loop

configMINIMAL_STACK_SIZE (128 words) is insufficient for tasks using snprintf or I2C.

### NVIC interrupt priority
Any ISR calling a FreeRTOS FromISR function must have a priority numerically >=
configMAX_SYSCALL_INTERRUPT_PRIORITY. Default priority 0 causes a hard fault inside
xQueueSendFromISR. USART2 priority set to 6 (>5) via NVIC_IPR9.

### RXNEIE enable timing
RXNEIE must not be enabled before vTaskStartScheduler() — spurious bytes during 
init trigger the ISR before the scheduler is running, causing xQueueSendFromISR 
to be called with no active tasks. Solved via vStartupTask (priority 2) that enables
RXNEIE then deletes itself.

## Lessons Learned

- Adding FreeRTOS folders to include paths (-I) is not enough — source folders must 
  be registered as source locations in CubeIDE (C/C++ General > Paths and Symbols > 
  Source Location tab) to get the .c files compiled and linked.

- xQueueCreate() cannot be called at global scope — it allocates from the FreeRTOS 
  heap which is not initialized until runtime. Declare handles globally, call 
  xQueueCreate() in main() before xTaskCreate().

- portMAX_DELAY on xQueueReceive puts the task to sleep until data arrives — zero 
  CPU usage while waiting. Timeout 0 causes a busy-wait spin loop.

- Concurrent UART access from multiple tasks produces garbled output. A queue alone 
  does not solve this — mutual exclusion via mutex is required (Week 20).

## Queue vs Mailbox vs Mutex

| Primitive | Purpose                          | Consumer behaviour     |
|-----------|----------------------------------|------------------------|
| Queue     | Transfer data between tasks      | Removes item           |
| Mailbox   | Broadcast latest value           | Peeks without removing |
| Mutex     | Protect shared hardware resource | Blocks until free      |