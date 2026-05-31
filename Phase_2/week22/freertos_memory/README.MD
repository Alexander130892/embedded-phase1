# Week 22 — FreeRTOS Memory Management & Stack Auditing

## Goal

Deepen understanding of FreeRTOS heap allocation schemes, stack overflow detection, and runtime memory auditing. Built on top of the Week 18 FreeRTOS port of the MPU6050 sensor logger, adding production-grade memory monitoring via interactive UART commands.

**Reference:** R2-RTOS Ch. 2 (heap) + FreeRTOS source `heap_4.c`

---

## Scope

- FreeRTOS heap allocation schemes: `heap_1` through `heap_5` — trade-offs and selection rationale
- `configTOTAL_HEAP_SIZE` sizing methodology
- Stack overflow detection: Method 1 vs Method 2 (`configCHECK_FOR_STACK_OVERFLOW`)
- `vApplicationStackOverflowHook()` implementation
- Runtime stack watermark auditing via `uxTaskGetSystemState()`
- Heap monitoring via `xPortGetFreeHeapSize()` + `xPortGetMinimumEverFreeHeapSize()`
- UART command interface: `hwm` and `mem` commands
- Intentional stack overflow test on isolated Git branch

---

## Architecture

```
MPU6050
  │
  ▼ (I²C)
vSensorReadTask ("Acquire")  ──► xQueue1 (mpu6050_data_t)
                                      │
                                      ▼
                               vLogTask ("Process") ──► xQueue2 (char[64])
                                                              │
                                                              ▼
                                                    vUARTTask ("Transmit") ──► USART2
                                                              ▲
vUART_Rx_Task ("Rx Terminal") ◄──────────────── xQueueUARTRx (char)
  │
  ├── "log <ms>"  → xTimerChangePeriod(xTimerSensor)
  ├── "hwm"       → uxTaskGetSystemState() → UART report
  └── "mem"       → xPortGetFreeHeapSize()
                    xPortGetMinimumEverFreeHeapSize() → UART report

Timers:
  xTimer        (Heartbeat, 5000ms) → watchdog() → notify vUARTTask on timeout
  xTimerSensor  (Sensor,  1000ms)  → notify vSensorReadTask

UART Mutex (xMutexUart) protects all UART output across tasks.
```

---

## Key Design Decisions

### Heap scheme: `heap_4`

`heap_4` was selected over alternatives because it supports both allocation and deallocation with block coalescence — adjacent free blocks are merged on `free()`, preventing fragmentation buildup. `heap_1` (alloc-only) would suffice for a fully static system but breaks down the moment any kernel object needs to be freed. `heap_2` adds free without coalescence, making it strictly inferior to `heap_4` except in pathological same-size allocation patterns. `heap_3` (thread-safe `malloc` wrapper) sacrifices determinism. `heap_5` was noted but not needed since only one contiguous SRAM bank is in use.

### Stack overflow detection: Method 2

`configCHECK_FOR_STACK_OVERFLOW` was set to 2 rather than 1. Method 1 only checks the stack pointer at the context switch boundary — it misses transient overflows where the stack spikes past the boundary and unwinds before the switch fires. Method 2 paints a known pattern at the stack base at task creation and verifies its integrity at every context switch, catching transient overflows via residual corruption.

### `vApplicationStackOverflowHook()` — no local variables, no mutex

The hook is called from within the FreeRTOS context switch (PendSV context) with the task's stack already corrupted. Two constraints follow: (1) any local variable allocation pushes onto the corrupted stack, risking further corruption, so `pcTaskName` (passed as a parameter) is used directly rather than formatting into a local buffer; (2) `xMutexUart` must not be taken — `xSemaphoreTake()` is a blocking FreeRTOS call requiring the scheduler to be in a normal running state, which it is not. `uart_send_string()` is safe to call because it is purely polling-based (direct register writes to USART2, no interrupt dependency). `taskDISABLE_INTERRUPTS()` is called before the spin loop to prevent further ISR activity on a corrupted system.

### `hwm` command: `uxTaskGetSystemState()` over per-task manual polling

Per-task watermark polling (commented out in earlier code) was replaced with a single `uxTaskGetSystemState()` call that snapshots all tasks atomically. This scales automatically as tasks are added and produces a consolidated report under a single `xMutexUart` hold, preventing interleaving with sensor output. `configUSE_TRACE_FACILITY 1` is required.

### Heap sizing workflow

1. Start with a generous `configTOTAL_HEAP_SIZE` estimate
2. Run the system under realistic load
3. Query `xPortGetMinimumEverFreeHeapSize()` for worst-case headroom
4. Rightsize task stacks using `hwm` watermark data
5. Re-measure, then tighten `configTOTAL_HEAP_SIZE` to actual consumption + ~25-30% margin

This workflow was applied in-session, reducing `configTOTAL_HEAP_SIZE` from 75KB to 11KB.

---

## Lessons Learned

- **The FreeRTOS heap is a static byte array** (`ucHeap[configTOTAL_HEAP_SIZE]` in `.bss`), not a dynamic carving of SRAM. Fragmentation risk is low not because of the array itself, but because RTOS objects are typically created at startup and never freed — the allocation pattern is front-loaded and static.

- **`uxTaskGetSystemState()` returns 0 when the array is undersized.** The function requires the array to accommodate *all* tasks, including system tasks (idle + timer daemon). Passing `N_TASKS = 4` for a system with 6 actual tasks silently returns 0 with no error. Use `uxTaskGetNumberOfTasks()` dynamically and size the global array conservatively.

- **`strcmp` returns 0 on match.** `if(strcmp(cmd, "hwm"))` fires when strings are *not* equal. Always use `== 0`.

- **Transient stack overflows are real.** Method 1 would have missed any overflow that unwinds before the context switch. Method 2 is mandatory in a debug build.

- **Oversizing the overflow test crashes everything.** `uint64_t overflow[10000]` (80KB) corrupted all of SRAM and triggered a HardFault before FreeRTOS could run a context switch — the hook never fired. The overflow array must be sized just past the task's remaining watermark (271 words → `uint32_t overflow[300]` was sufficient).

- **Both free heap values matter together.** `xPortGetFreeHeapSize()` is a current snapshot; `xPortGetMinimumEverFreeHeapSize()` is the historical minimum. For sizing decisions, only the minimum ever is meaningful — the current value tells you nothing about peak demand.

- **CCMRAM (0x10000000) is CPU-accessible only.** The STM32F446RE has 64KB CCMRAM tightly coupled to the core via D-bus. DMA cannot access it. `heap_5` could pool CCMRAM + main SRAM for stack allocations, reserving main SRAM for DMA buffers.

---

## Measured Results (after rightsizing)

| Task | Stack (words) | HWM Free (words) | Headroom |
|---|---|---|---|
| UART_Rx_T | 256 | 114 | 44% |
| IDLE | 130 | 106 | 81% |
| SensorRea | — | 222 | — |
| Tmr Svc | — | 214 | — |
| UARTTask | — | 198 | — |
| LogTask | — | 271 | — |

| Metric | Value |
|---|---|
| `configTOTAL_HEAP_SIZE` | 11 264 bytes (11 KB) |
| Current free heap | 2 776 bytes |
| Minimum ever free heap | 2 776 bytes |
| Headroom | ~24% |

---

