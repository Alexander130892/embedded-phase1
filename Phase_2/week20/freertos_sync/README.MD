# Week 20 — Semaphores + Mutexes

## ISR → Task: Decision Tree

| Primitive | Use when | Data handling | Overhead |
|---|---|---|---|
| **Binary semaphore** | Only the latest value matters (e.g. temperature) | Shared variable — ISR writes, task reads | Lowest |
| **Queue** | Every sample matters (UART bytes, audio, waveform) | Queue buffers values in FIFO order | Higher |
| **Counting semaphore** | Multiple instances of a resource (pool of N buffers) | Tracks available count, no data transfer | Low |
| **Mutex** | Never in ISR — no task context, no ownership | N/A | Highest (tracks owner + priority) |

### Semaphore + shared variable vs Queue

- **Semaphore**: signals "event happened." No data, no buffer. If the ISR fires 3× before the task runs, a binary semaphore only records "at least once" — a counting semaphore counts 3, but the shared variable only holds the last value. First two readings are lost.
- **Queue**: signals + transfers data atomically in FIFO order. Every value is preserved. Use when every sample matters.

### When only the latest value matters

Use a binary semaphore + shared `volatile` variable. The ISR overwrites the variable each time — stale intermediate values are irrelevant by definition (e.g. current temperature).

### Broadcasting to multiple tasks

Semaphores don't broadcast. If multiple tasks need the same event, use **Event Groups** (`xEventGroupSetBits`) — one event wakes all waiting tasks simultaneously.

---

## Binary Semaphore — ISR Pattern

```c
volatile uint16_t adc_result;
SemaphoreHandle_t xSemaphore;

void TIM2_IRQHandler(void) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (*((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) & 0x1) {
        adc_result = ADC1->DR;
        xSemaphoreGiveFromISR(xSemaphore, &xHigherPriorityTaskWoken);
        *((volatile uint32_t*)(TIM2_BASE_ADDR + TIMx_SR_OFFSET)) &= ~(0x1);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void sensor_task(void *p) {
    for (;;) {
        xSemaphoreTake(xSemaphore, portMAX_DELAY);  // blocks, yields CPU
        process(adc_result);
    }
}
```

### Hardware timer vs `vTaskDelay`

- `vTaskDelay(500)`: 500ms starts after the task finishes work → actual period = 500ms + work time.
- Hardware timer + semaphore: TIM2 fires at exactly 500ms regardless of task execution time. Better timing accuracy.

---

## Mutex — Protecting Shared Resources

```c
SemaphoreHandle_t xMutexUart;

void vUARTTask(void *pvParameters) {
    for (;;) {
        if (xQueueReceive(xQueue2, &output_str, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(xMutexUart, portMAX_DELAY);
            uart_send_string(output_str);
            uart_send_string(hwm_str);
            xSemaphoreGive(xMutexUart);
        }
    }
}
```

### Mutex vs Binary Semaphore

| | Binary Semaphore | Mutex |
|---|---|---|
| Ownership | None — any task/ISR can give | Only the owner can give |
| Priority inheritance | No | Yes |
| ISR-safe | Yes (`xSemaphoreGiveFromISR`) | No — ISRs have no task context |
| Use case | Signalling events | Protecting shared resources |

### FreeRTOS primitives are self-protecting

Queue operations (`xQueueSend`, `xQueueReceive`) internally enter critical sections — they are thread-safe by design. No external mutex needed. Only **your own code** (e.g. `uart_send_string`) needs explicit protection.

---

## Priority Inversion

Three tasks: L (low), M (medium), H (high) sharing a mutex.

**Without priority inheritance:**
1. L takes the mutex, enters critical section
2. H becomes ready, tries to take mutex → blocks (L holds it)
3. M becomes ready, preempts L (M > L)
4. L can't finish critical section → H waits behind L → M effectively runs at higher priority than H
5. This is **unbounded priority inversion** — H could be starved indefinitely

**With priority inheritance (FreeRTOS mutex):**
1. L takes the mutex
2. H blocks on the mutex → FreeRTOS boosts L to H's priority
3. M cannot preempt L (L is now at H's priority level)
4. L finishes critical section, releases mutex, drops back to original priority
5. H runs immediately

Priority inheritance raises the holder to the priority of **the highest waiter**, not the highest priority in the system.

---

## Cortex-M4 + FreeRTOS: NVIC Priority Gotcha

Any interrupt calling FreeRTOS API functions (`xSemaphoreGiveFromISR`, `xQueueSendFromISR`, etc.) must have an NVIC priority value ≥ `configMAX_SYSCALL_INTERRUPT_PRIORITY`.

Default NVIC priority is 0 (highest on ARM — lower number = higher priority), which is **above** FreeRTOS's manageable range. Always set the priority explicitly:

```c
// TIM2 = IRQ 28 → IPR register 7, byte 0, upper nibble (bits [7:4])
*((volatile uint32_t*)(NVIC_IPR_ADDR + TIM2_POS_IPR)) &= ~(0xF << ((8*0)+4));
*((volatile uint32_t*)(NVIC_IPR_ADDR + TIM2_POS_IPR)) |=  (0x6 << ((8*0)+4));
```

### Init-time safety

Use `configASSERT` to catch allocation failures at creation rather than checking every call site:

```c
xSemaphore  = xSemaphoreCreateBinary();
configASSERT(xSemaphore != NULL);

xMutexUart  = xSemaphoreCreateMutex();
configASSERT(xMutexUart != NULL);
```

Also ensure creation order: **create FreeRTOS objects before enabling interrupts that use them.**