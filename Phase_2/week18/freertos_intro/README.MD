# Week 18 — FreeRTOS Intro: Two Concurrent Tasks on STM32F446RE

## Objective
Manually port FreeRTOS to the Nucleo-F446RE without CubeMX, and verify concurrent task execution using an LED blink task and a UART print task.

---

## Project Structure

```
week18/freertos_intro/
├── Inc/                        # Project headers (uart.h, led.h)
├── Src/                        # Application code (main.c, uart.c, led.c)
├── Startup/                    # CubeIDE-generated startup assembly
├── FreeRTOS/
│   ├── Source/                 # FreeRTOS kernel .c files
│   ├── Port/                   # ARM_CM4F port layer (port.c, portmacro.h)
│   └── Include/                # Kernel headers + FreeRTOSConfig.h
├── STM32F446RETX_FLASH.ld
└── STM32F446RETX_RAM.ld
```

---

## What the Project Does

Two FreeRTOS tasks run concurrently:

| Task | Function | Period |
|---|---|---|
| LED Blink | Toggles onboard LED | 500ms |
| UART Tick | Sends `"tick\r\n"` over UART | 5000ms |

Both tasks block on `vTaskDelay()` between executions. The idle task runs at priority 0 whenever both application tasks are blocked.

Concurrency is verified by the independent periods: UART prints twice per LED toggle, confirming the scheduler manages both tasks independently rather than sequentially.

---

## Architectural Decisions

### Manual port — no CubeMX middleware
FreeRTOS was added manually by copying kernel source files from the official FreeRTOS zip. CubeMX was not used to generate RTOS configuration. This exposes the actual structure of FreeRTOS: kernel source, port layer, and configuration are three distinct concerns.

### Port layer selection: ARM_CM4F
The STM32F446RE is a Cortex-M4F. The FreeRTOS port layer is architecture-specific, not chip-specific. Any `CORTEX_M4F` demo port files are directly reusable regardless of which STM32 F4 chip is targeted. Chip-specific demo code (board drivers, peripheral init) was ignored entirely.

### Heap implementation: heap_4
`heap_4.c` was chosen as the memory manager. It supports dynamic allocation with coalescence of free blocks, suitable for a system where tasks are created at startup and not deleted at runtime.

### FreeRTOSConfig.h based on STM32F407ZG demo
The configuration was taken from the `CORTEX_M4F_STM32F407ZG-SK` demo as the closest available reference. Changes made:
- Replaced `#ifdef __ICCARM__` (IAR) guard with `#ifdef __GNUC__` (GCC)
- `configCPU_CLOCK_HZ` set to `SystemCoreClock` for portability
- `configTICK_RATE_HZ` set to 1000 (1ms tick)
- `configMAX_PRIORITIES` kept at 5 — sufficient for week 18

### No SystemClock_Config()
The chip runs on the default internal oscillator at 16MHz. `SystemCoreClock` defaults to 16000000. FreeRTOS programs SysTick internally at `vTaskStartScheduler()` using this value. Acceptable for week 18 where timing precision is not critical.

### Task priority: both at 1
Both tasks are assigned equal priority. The scheduler round-robins between them when both are ready. The idle task at priority 0 receives CPU time whenever both tasks are blocked in `vTaskDelay()`. FreeRTOS housekeeping depends on the idle task running — keeping application tasks above priority 0 ensures this.

---

## Lessons Learned

### Empty project is too bare
CubeIDE's "STM32CubeIDE Empty Project" does not generate CMSIS device files (`system_stm32f4xx.c`, `stm32f4xx.h`, etc.). These had to be added manually one by one to resolve `SystemCoreClock`. For future projects, start from a CubeIDE project with CMSIS generated, then add FreeRTOS manually on top.

### FreeRTOS hook functions must be implemented
`configUSE_TICK_HOOK`, `configUSE_IDLE_HOOK`, `configUSE_MALLOC_FAILED_HOOK`, and `configCHECK_FOR_STACK_OVERFLOW` being set to non-zero means the application must provide the corresponding hook function bodies. The linker will fail with undefined reference errors otherwise. Minimal stubs are sufficient:

```c
void vApplicationTickHook(void){}
void vApplicationIdleHook(void){}
void vApplicationMallocFailedHook(void){}
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName){}
```

### Interrupt vector aliases are critical
The three lines at the bottom of `FreeRTOSConfig.h` map FreeRTOS port handlers to CMSIS handler names:

```c
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
```

Without these, the FreeRTOS context switch and tick handlers are never called by the hardware. The scheduler appears to start but tasks never execute.

### FreeRTOS is just C source
The kernel receives no special treatment from the toolchain. It compiles as ordinary `.c` files, and the port layer assembly compiles alongside application code. The "magic" is entirely at runtime: SysTick, PendSV, and SVC are the only hardware dependencies.

### Task functions must loop and delay
A FreeRTOS task that returns is a fatal error. Every task must contain a `for(;;)` loop. Calling `vTaskDelay()` inside the loop yields the CPU back to the scheduler, allowing other tasks and the idle task to run. A task that loops without yielding starves lower-priority tasks.

### Sequential vs concurrent verification
Two tasks with identical delays do not prove concurrency — sequential execution would look identical. Assigning different delays (e.g. 500ms and 1000ms) and observing independent periods is the correct verification method.

---

## Include Paths Added in CubeIDE

Under **MCU GCC Compiler → Include Paths**:

```
${workspace_loc:/${ProjName}/FreeRTOS/Include}
${workspace_loc:/${ProjName}/FreeRTOS/Port}
../Inc
```

---

## Key API Used

| API | Purpose |
|---|---|
| `xTaskCreate()` | Create a task and add it to the ready list |
| `vTaskDelay(pdMS_TO_TICKS(ms))` | Block task for a given number of milliseconds |
| `vTaskStartScheduler()` | Hand control to the FreeRTOS kernel |
| `pdMS_TO_TICKS()` | Convert milliseconds to ticks portably |