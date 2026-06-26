# FreeRTOS Phase Retrospective — Weeks 18–24

## What was built

A four-task bare-metal FreeRTOS application on the STM32F446RE Nucleo running entirely
without HAL. Two sensors (MPU-6050 over I2C, BMP280 over SPI) stream data through typed
queues into a CSV logger task. A hardware timer (TIM2 @ 100 Hz) drives the IMU; a software
timer (1 Hz) drives the pressure sensor. A two-flag watchdog detects sensor failures. A
UART command parser handles `log on/off`, `hwm`, and `mem` at runtime. The full build and
flash toolchain runs independently of CubeIDE via a hand-written Makefile.

Verified: 15-minute continuous run, zero stack overflows, zero UART corruption, live serial
output from VS Code terminal.

---

## What I'd do differently

### 1. Architecture first — file structure before any code

`main.c` accumulated responsibility over Weeks 18–23: queue handles, task handles, timer
handles, watchdog flags, timer callbacks, and raw register writes all lived there at various
points. Untangling it in Week 24 took significant effort that could have been avoided.

The right approach is to sketch the module boundaries before writing the first line of code:
- What are the layers? (drivers / tasks / app / rtos)
- What state is shared, and who owns it?
- Where do callbacks live relative to the tasks they signal?

A `kernel_objects` module and a clear `Inc/` + `Src/` mirror structure should be the
starting point, not the final refactor. Architecture decisions made under time pressure at
the end of a project are always more expensive than decisions made at the beginning.

### 2. Clarity on header file responsibilities from the start

The distinction between declaration (`.h`) and definition (`.c`) was understood in principle
but not always applied consistently. Specific lessons:

- `extern` declarations belong in the header — without them, other translation units cannot
  see symbols defined in the corresponding `.c` file.
- `volatile` qualifiers must match exactly between declaration and definition — a mismatch
  produces a `conflicting type qualifiers` error that is easy to miss.
- Internal-only functions (IRQ handlers, timer callbacks, driver primitives) do not belong
  in public headers. The linker resolves IRQ handlers via the vector table; no other
  translation unit calls them by name.
- Best practice is explicit includes in every `.c` file for what it directly uses, rather
  than relying on transitive includes. Transitive reliance breaks silently when a header is
  refactored.

A brief header contract written per module at the start — "this header exposes X, hides Y,
depends on Z" — would have prevented most of the include path issues encountered.

### 3. Stack sizing discipline

Stack sizes were set by intuition early in the phase and only validated properly in Week 24
after a 15-minute stability run. Two tasks hit stack overflows that required investigation:

- `vLogTask` needed 512 words — `snprintf` formatting at `-O0` is heavier than expected,
  and large local buffers compound the problem.
- `vCmdTask` needed 256 words even after moving local string buffers to `static` storage.

The correct workflow is:
1. Assign conservative initial stack sizes (256–512 words for non-trivial tasks).
2. Run a stability test at the actual build optimization level.
3. Read HWM values at the *end* of the run (they only move in one direction).
4. Set final sizes to HWM + 25% headroom.

Stack HWM readings at startup are misleading — they reflect only the initial call depth, not
worst-case behaviour under sustained load. And `-O0` inflates stack usage significantly
compared to `-Og` or `-O1`; size stacks at the optimization level that will actually be
used.

---

## What worked well

- **Socratic design sessions** — reasoning through decisions (notification vs semaphore,
  hardware vs software timer, `static` vs `extern`) before touching code produced better
  outcomes than writing first and refactoring later. The architecture review in Week 24
  confirmed that the high-level design from Week 23 was sound; only the packaging needed work.

- **`kernel_objects` pattern** — a single module owning all RTOS handles is clean,
  portable, and immediately comprehensible. Worth establishing as a default starting point
  for any multi-task FreeRTOS project.

- **Driver/task separation** — keeping bare-metal register writes inside driver functions
  (`timer2_enable`, `timer2_disable`) and out of task files made the task code readable and
  the driver layer testable in isolation.

- **Hand-written Makefile** — understanding exactly which source files, include paths, and
  linker flags the build requires is more valuable than letting an IDE manage it. The
  Makefile is now a living document that travels with the project.

---

## Open items carried forward

- BMP280 `WHO_AM_I` liveness check (replace temperature range sanity check)
- Run-time stats via dedicated TIM3 stats counter
- Event-driven I2C driver (interrupt + semaphore, eliminate spin-loop blocking window)
- `-Wunused-parameter` cleanup with `(void)` casts