# 🛠️ Embedded Systems — Phase 3 Study Tracker
**Yocto + C++ + Tooling Maturity | Months 8–12 | ~10 hrs/week**

> **Goal:** Build a production-grade embedded Linux image, master modern C++ for embedded systems, and develop the engineering hygiene (CI/CD, testing, Git workflow) expected in professional embedded teams.
> **Strategy:** Yocto first — closes the Linux chapter naturally from Phase 2. Then C++ with Amar's book as the primary resource. Rust as a one-week exploration. Tooling last — Git, CI/CD, and testing apply to a real codebase, not exercises in a vacuum.

---

## 📅 Weekly Schedule

---

### 🟡 BLOCK 1 — Yocto Pipeline (Weeks 29–32)

> Yocto requires a Linux-capable processor with an MMU. All Yocto work targets the **Raspberry Pi 4B**. 

---

#### Week 29 — Yocto Concepts + First Build
> ~10 hrs | Mastering Embedded Linux Programming Ch. 6 (Simmonds) + Yocto Project docs

**Study tasks**
- [x] Understand the Yocto Project vs OpenEmbedded distinction — Yocto is the umbrella, `bitbake` is the build engine, `poky` is the reference distribution
- [x] Understand the four key concepts: layers, recipes (`.bb`), machine configurations, and image targets
- [x] Read the Yocto Project Quick Build guide: [docs.yoctoproject.org/brief-yoctoprojectqs](https://docs.yoctoproject.org/brief-yoctoprojectqs/index.html)
- [x] Simmonds Ch.6 — Selecting a Build System: read the Yocto section; understand why Yocto is the industry standard despite its complexity
- [x] Understand `bitbake` workflow: `MACHINE`, `DISTRO`, `IMAGE_INSTALL`, `local.conf`, `bblayers.conf`
- [x] Set up build host: Ubuntu 22.04 VM on your Proxmox (or your existing Ubuntu 24.04 VM) with Yocto dependencies installed
- [x] Clone `poky` (Yocto reference distribution) and `meta-raspberrypi` layer
- [x] Configure `local.conf` for `MACHINE = "raspberrypi4-64"` and `DISTRO = "poky"`
- [x] Run `bitbake core-image-minimal` — expect a long first build (2–4 hrs), use the time to read

**Exercises**
- [x] Flash the resulting `.wic` image to SD card; boot Pi and confirm SSH login
- [x] Identify the four build artefacts in `tmp/deploy/images/`: kernel, DTB, rootfs, bootloader
- [x] Explore the `tmp/work/` tree — understand where recipes unpack, patch, compile, and stage
- [x] Commit notes and `local.conf` / `bblayers.conf` to `week29/yocto_first_build/`

> 🏁 **M16 checkpoint:** Yocto `core-image-minimal` boots on Pi 4B over SSH.

---

#### Week 30 — Layers, Recipes + Custom Image
> ~10 hrs | Yocto Project docs + Simmonds Ch. 6–7

- [x] Understand the layer priority system — how `BBFILE_PRIORITY` and `LAYERVERSION` interact
- [x] Study a real recipe: pick `meta/recipes-core/busybox/busybox_*.bb` and trace the `do_fetch`, `do_configure`, `do_compile`, `do_install` task chain
- [x] Understand `DEPENDS` vs `RDEPENDS` — build-time vs runtime dependencies
- [x] Understand `inherit` directives: `cmake`, `autotools`, `pkgconfig` — know when each is used
- [x] Understand `IMAGE_INSTALL`, `CORE_IMAGE_EXTRA_INSTALL`, and how to extend a base image
- [x] Create your own layer: `meta-avindeli` — correct directory structure (`conf/layer.conf`, `recipes-*/`)
- [x] Write a recipe for a simple C program (use your Phase 2 cross-compile Hello World as the source)
- [x] Add the recipe to a custom image (`core-image-avindeli.bb`) and build it
- [x] Verify your program runs on the Pi after flashing
- [x] Commit to `week30/meta-avindeli/`

---

#### Week 31 — Kernel Module Recipe + Device Tree Overlay
> ~10 hrs | Simmonds Ch. 11 (kernel modules in Yocto) + Yocto kernel docs

- [x] Understand `linux-yocto` vs `linux-raspberrypi` kernel recipe — which one `meta-raspberrypi` uses
- [x] Study `inherit kernel-module` and what `KERNEL_MODULE_AUTOLOAD` does
- [x] Understand how out-of-tree kernel module recipes work: `module_do_compile`, `module_do_install`
- [x] Write a Yocto recipe for your Week 27 kernel module (`mydev` character device)
- [x] Verify the module loads automatically at boot via `KERNEL_MODULE_AUTOLOAD`
- [x] Understand Device Tree overlays in Yocto: `.bbappend` files on the kernel recipe, `RPI_KERNEL_DEVICETREE_OVERLAYS`
- [x] Write a minimal DT overlay recipe that configures a GPIO pin — add it to your image
- [x] Write a systemd service recipe that starts your Week 28 userspace app at boot
- [x] Verify the full pipeline: boot → module auto-loads → service starts → `/dev/mydev` readable
- [x] Commit to `week31/kernel_module_recipe/`

> 🏁 **M17 checkpoint:** Custom Yocto image boots with your own kernel module and systemd service baked in.

---

#### Week 32 — Yocto Deliverable + SDK
> ~10 hrs | Yocto Project docs (SDK chapter) + polish

- [x] Understand the Yocto extensible SDK (eSDK): `bitbake -c populate_sdk <image>` — what it produces
- [x] Install the generated SDK and verify cross-compilation of a C program against the target sysroot
- [x] Understand `devtool`: `devtool add`, `devtool modify`, `devtool build` — the recommended workflow for recipe development
- [x] Use `devtool` to modify your Hello World recipe and iterate without a full rebuild
- [x] Add `openssh-sftp-server` and `strace` to your image for debugging convenience
- [x] Enable `IMAGE_FEATURES += "debug-tweaks"` for development builds; understand why this must not go to production
- [x] Write `README.md` for the Yocto block covering:
  - [x] Host setup (distro, packages, poky version)
  - [x] Layer structure and what `meta-avindeli` contains
  - [x] Build instructions (`bitbake core-image-avindeli`)
  - [x] SDK generation and usage
  - [x] Comparison: manual cross-compile (Week 25) vs Yocto SDK vs Yocto image
- [x] Tag the repo: `git tag v1.0-phase3-yocto`
- [x] Write a short Yocto retrospective in `NOTES.md`

> 🏁 **M18 checkpoint:** Custom Yocto image + SDK generated. Yocto block complete. ✅

---

### 🔵 BLOCK 2 — C++ for Embedded Systems (Weeks 33–37)

> Primary resource: *C++ in Embedded Systems* — Amar Mahmutbegović (ISBN 978-1-83588-114-9, Packt 2025).
> Target hardware: STM32F446RE Nucleo (from Phase 1/2) + Renode simulation where hardware isn't needed.
> Discipline throughout: no heap allocation, no exceptions, no RTTI — embedded-safe C++ only.

---

#### Week 33 — C++ Introduction + Dev Environment
> ~10 hrs | Book Ch. 1–4 (Part I)

**Study tasks**
- [ ] Ch.1 — Debunking myths: understand zero-cost abstractions; compare ring buffer in C vs C++ (the book's worked example); study `constexpr` basics
- [ ] Ch.1 — Understand what bloat actually comes from: RTTI, exceptions, virtual dispatch — and how to measure it with `arm-none-eabi-size`
- [ ] Ch.2 — Embedded constraints: determinism vs non-determinism, A-B timing measurements, why dynamic memory is problematic in safety-critical systems
- [ ] Ch.2 — Understand `new`/`delete` in a freestanding environment and how to disable them
- [ ] Ch.3 — Ecosystem overview: Compiler Explorer (godbolt.org), static analyzers, unit testing frameworks — bookmark these tools
- [ ] Ch.4 — Set up a CMake-based C++ project for your STM32 target (cross-compiled with `arm-none-eabi-g++`)
- [ ] Ch.4 — Understand the containerised dev environment approach in the book; decide whether to use Docker or your existing Proxmox Ubuntu VM
- [ ] Revisit the Renode dev container issue from Phase 2 — the book uses Renode; check if Ch.4's setup resolves the glibc incompatibility

**Exercises**
- [ ] Exercise: port your Phase 1/2 ring buffer from C to C++ — encapsulate in a class, make it a template, verify same `arm-none-eabi-size` output
- [ ] Exercise: build and run the book's ring buffer example in Renode (or on hardware if Renode still fails)
- [ ] Commit to `week33/cpp_intro/`

---

#### Week 34 — C++ Fundamentals: Classes + Error Handling
> ~10 hrs | Book Ch. 5–7 (Part II)

- [ ] Ch.5 — Classes: encapsulation, constructors with member initializer lists, static members, `explicit` keyword — understand why `explicit` matters for single-argument constructors on embedded targets
- [ ] Ch.5 — Inheritance and dynamic polymorphism: virtual functions, vtable layout (understand the memory cost), UML class diagrams — you covered this in Phase 2; deepen the model here
- [ ] Ch.6 — Namespaces, function overloading, C interoperability (`extern "C"`), references vs pointers (revisit with C++ eyes), rvalue references and move semantics — understand why move semantics matter even without heap usage
- [ ] Ch.6 — `std::array` vs raw arrays, `std::span` for buffer views — embedded-safe standard library
- [ ] Ch.7 — Error handling without exceptions: error codes, asserts, `std::optional`, `std::expected` — adopt a consistent strategy for your own drivers
- [ ] Study BARR-C equivalents in C++: when to use `const` vs `constexpr`, `[[nodiscard]]`, `[[maybe_unused]]`

**Exercises**
- [ ] Exercise: rewrite your Phase 2 `bmp280.c/h` driver pair as a C++ class — `Bmp280` with `init()`, `read_temperature()`, `read_pressure()`; use `std::optional` for error returns
- [ ] Exercise: write a `UartLogger` class that wraps your UART TX code — demonstrate `extern "C"` linkage for ISR handlers
- [ ] Exercise: write unit tests for `Bmp280` using fff (fake I2C transport injected via constructor) — run on host, no hardware needed
- [ ] Commit to `week34/cpp_fundamentals/`

---

#### Week 35 — Advanced C++: Templates + Compile-Time
> ~10 hrs | Book Ch. 8–11 (Part III)

- [ ] Ch.8 — Templates: function templates, class templates, template specialisation, concepts (C++20) — understand what CRTP is and why it replaces virtual dispatch in embedded contexts
- [ ] Ch.8 — Compile-time polymorphism via CRTP: no vtable, no runtime overhead — this is the embedded C++ pattern
- [ ] Ch.9 — Type safety: implicit conversions (the dangerous ones), `static_cast` vs `reinterpret_cast`, type punning the correct way, strong types — apply to your sensor reading types (`Celsius`, `Pascal`, `Gauss`)
- [ ] Ch.10 — Lambdas: capture modes, `std::function` (understand the heap allocation risk), the command pattern for GPIO interrupt managers
- [ ] Ch.11 — `constexpr` and `consteval`: lookup table generation at compile time, MAC address parser — understand how to push computation to compile time to save Flash and cycles
- [ ] Study embedded template library (ETL) — the book covers it in Ch.17; read that section now as context for what replaces `std::vector`, `std::string` in embedded C++

**Exercises**
- [ ] Exercise: implement a type-safe `SensorReading<Unit>` template — `SensorReading<Celsius>` and `SensorReading<Pascal>` are distinct types; no accidental mixing
- [ ] Exercise: generate a sine lookup table at compile time using `constexpr` — verify it lands in Flash (`.rodata`), not RAM, with `arm-none-eabi-objdump`
- [ ] Exercise: refactor your UART command dispatch (Phase 1 function pointer table) using CRTP or lambdas — compare generated code size
- [ ] Commit to `week35/cpp_advanced/`

---

#### Week 36 — Applied C++: HAL, Patterns + FSM
> ~10 hrs | Book Ch. 12–16 (Part IV)

- [ ] Ch.12 — Writing a C++ HAL: memory-mapped peripherals as C++ types, type-safe bitfield access — model your STM32 GPIO registers as C++ structs
- [ ] Ch.13 — Wrapping C libraries: using your existing C sensor drivers (`bmp280.c`, `mpu6050.c`) safely from C++ code using RAII wrappers; adapter pattern for UART interface
- [ ] Ch.14 — Sequencer pattern: super-loop enhanced with a task sequencer — understand when this is preferable to FreeRTOS for simpler systems
- [ ] Ch.15 — Observer pattern: runtime vs compile-time implementations; variadic templates for compile-time observer lists — apply to sensor data publishing
- [ ] Ch.16 — Finite State Machines in C++: the simple switch-based FSM, then the State pattern, then Boost.SML mention — implement at least the simple and State-pattern versions
- [ ] Ch.17 — Libraries: ETL fixed-size containers, `etl::delegate` as `std::function` replacement; skim Pigweed for awareness
- [ ] Ch.18 — SOLID principles applied to embedded: SRP, OCP, DIP — these are what senior engineers look for in code review

**Exercises**
- [ ] Exercise: implement a `GpioPin` C++ class that wraps your bare-metal GPIO register code — type-safe, no runtime overhead, verified with `arm-none-eabi-size`
- [ ] Exercise: implement a BLE connection FSM (from the book's example) — state diagram first, then switch-based, then State pattern; compare code readability and size
- [ ] Exercise: write an Observer-pattern sensor publisher — `Bmp280` notifies `UartLogger` and a `LedIndicator` without knowing about them
- [ ] Commit to `week36/cpp_applied/`

---

#### Week 37 — C++ Deliverable + Integration
> ~10 hrs | Polish, integration, documentation

- [ ] Integrate the C++ work into one coherent project: `CppSensorLogger` — a C++ rewrite of your Phase 2 Week 17 data logger
  - [ ] `Bmp280` and `Mpu6050` C++ driver classes
  - [ ] `UartLogger` class with `etl::delegate` callback
  - [ ] `SensorFsm` state machine controlling log start/stop
  - [ ] Observer pattern connecting sensors → logger
  - [ ] CMake build system (replace Makefile)
  - [ ] Runs on STM32F446RE — no FreeRTOS (pure super-loop + sequencer)
- [ ] Run `cppcheck` and `clang-tidy` on the codebase — fix all warnings
- [ ] Write `README.md` covering:
  - [ ] Architecture diagram (class relationships, observer connections, FSM states)
  - [ ] C vs C++ comparison table: what each feature replaced and why
  - [ ] Build instructions (`cmake`, `make`, `openocd` flash)
  - [ ] Lessons learned
- [ ] Tag the repo: `git tag v1.0-phase3-cpp`
- [ ] Write a short C++ retrospective in `NOTES.md`

> 🏁 **M19 checkpoint:** C++ sensor logger running on STM32. CMake build. Static analysis clean. ✅

---

### 🟠 BLOCK 3 — Rust Exploration (Week 38)
> One sprint. Goal: informed opinion, not mastery. Build something real, understand the tradeoffs, be able to discuss it in an interview.

---

#### Week 38 — Rust for Embedded: One Sprint
> ~10 hrs | The Embedded Rust Book (free) + Embassy framework docs

**Study tasks**
- [ ] Read *The Embedded Rust Book* (free at [docs.rust-embedded.org/book](https://docs.rust-embedded.org/book/)): Ch. 1–5 — toolchain setup, memory layout, registers as types, the `#![no_std]` environment
- [ ] Understand Rust's ownership model in one sentence that you can explain to a C developer: *the borrow checker enforces at compile time what C enforces (or fails to enforce) at runtime*
- [ ] Understand `embassy-rs`: async/await for embedded, no RTOS needed — read the Embassy overview at [embassy.dev](https://embassy.dev/)
- [ ] Understand where Rust is gaining traction in embedded: Linux kernel drivers, safety-critical automotive (ISO 26262 discussions), WASM targets
- [ ] Understand where C/C++ still dominates and why (toolchain maturity, vendor support, existing codebases, certification trails)
- [ ] Set up the Rust toolchain: `rustup target add thumbv7em-none-eabihf` for your STM32F4

**Exercises**
- [ ] Exercise: port your Phase 1 LED blink to Rust using `embassy-stm32` — GPIO toggle in an async task
- [ ] Exercise: add UART TX — send "tick" over serial every second
- [ ] Reflection: write a `RUST_NOTES.md` comparing the experience to C — what the compiler caught that C wouldn't, what was harder, what was surprising
- [ ] Commit to `week38/rust_blink/`

> 🏁 **M20 checkpoint:** Rust blink + UART running on STM32. Reflection written.

---

### 🟢 BLOCK 4 — Engineering Hygiene: Git + CI/CD + Testing (Weeks 39–44)

> This block applies to real codebases — your Phase 2 and Phase 3 repos. No toy examples.

---

#### Week 39 — Git for Teams
> ~10 hrs | Pro Git Ch. 1–3, 5 (free at git-scm.com) + practice

**Study tasks**
- [ ] Pro Git Ch.1–2 — Refresher: object model (blob, tree, commit, tag), the index, `reflog` — understand what actually happens under the hood during a commit
- [ ] Pro Git Ch.3 — Branching: `git branch`, `git merge`, `git rebase` — understand when rebase is appropriate and when it's dangerous (never rebase shared branches)
- [ ] Pro Git Ch.5 — Distributed workflows: forking model, PR-based workflow, code review etiquette
- [ ] Study two branching strategies and know the tradeoffs:
  - [ ] **Gitflow**: `main`, `develop`, `feature/*`, `release/*`, `hotfix/*` — good for versioned releases
  - [ ] **Trunk-based development**: short-lived feature branches, merge to main frequently — preferred for CI/CD
- [ ] Study commit message conventions: Conventional Commits (`feat:`, `fix:`, `refactor:`, `docs:`) — these feed automated changelogs and CI triggers
- [ ] Understand `git bisect` — binary search through history to find a bug-introducing commit; practice on your own repo

**Exercises**
- [ ] Exercise: take your Phase 2 repo and retroactively apply Conventional Commits conventions to the last 10 commits using `git rebase -i` — practice interactive rebase
- [ ] Exercise: set up a `develop` branch; simulate a feature branch workflow — create `feature/cpp-bmp280-driver`, open a PR (even to yourself), write a proper PR description, merge with `--no-ff`
- [ ] Exercise: deliberately introduce a bug in a test branch, use `git bisect` to find it — document the workflow
- [ ] Exercise: set up `.gitignore` properly for a mixed C/C++/CMake/Yocto repo — cover `build/`, `tmp/`, `*.o`, `*.elf`, `sstate-cache/`
- [ ] Commit workflow notes to `week39/git_teamwork/`

---

#### Week 40 — CI/CD for Embedded: GitHub Actions
> ~10 hrs | GitHub Actions docs + practice

- [ ] Understand GitHub Actions concepts: workflow (`.yml`), trigger (`on:`), job, step, runner, artifact
- [ ] Understand the challenge of embedded CI: no hardware on the runner — solution is build verification + static analysis + simulation (QEMU/Renode)
- [ ] Study `ubuntu-latest` runner environment: what tools are pre-installed, how to install `arm-none-eabi-gcc` in a workflow step
- [ ] Understand GitHub Actions caching: cache the ARM toolchain and `apt` packages to speed up builds
- [ ] Understand job dependencies (`needs:`), matrix builds (build for multiple targets), and artifacts (`actions/upload-artifact`)

**Exercises**
- [ ] Exercise: write a GitHub Actions workflow for your Phase 3 `CppSensorLogger`:
  - [ ] Trigger on push to `main` and on PRs
  - [ ] Step 1: install `arm-none-eabi-gcc` and CMake
  - [ ] Step 2: `cmake` configure + `make` — build the `.elf`
  - [ ] Step 3: run `cppcheck` — fail the build on errors
  - [ ] Step 4: upload the `.elf` as a build artifact
- [ ] Exercise: add a second job that builds your Yocto SDK (or a minimal subset) — understand why this is slow and how `sstate-cache` would help in a real team setup
- [ ] Exercise: add a badge to your repo README: `![CI](https://github.com/username/repo/actions/workflows/ci.yml/badge.svg)`
- [ ] Commit workflow files to `week40/github_actions/` and verify the Actions tab shows green

> 🏁 **M21 checkpoint:** GitHub Actions CI building and linting your C++ project on every push. ✅

---

#### Week 41 — Static Analysis + Code Quality
> ~10 hrs | cppcheck docs + clang-tidy docs + MISRA-C awareness

- [ ] Understand the difference between compiler warnings, static analysis, and runtime sanitizers — three different layers of defect detection
- [ ] **cppcheck**: install and run on your C and C++ codebases; understand the checker categories (memory leaks, null pointer dereference, undefined behaviour, style)
- [ ] **clang-tidy**: understand it as a linter + refactoring tool; study the `cppcoreguidelines-*` and `modernize-*` checker families — these align with Amar's book
- [ ] **MISRA-C and MISRA-C++**: understand what they are (coding standards for safety-critical systems), which rules exist, and why companies like Melexis and Barco care about them — you don't need to implement a full MISRA check, just be able to discuss it
- [ ] Understand sanitizers: `-fsanitize=address` (ASan), `-fsanitize=undefined` (UBSan) — these run on host-compiled code, not on the target; useful for testing logic layers
- [ ] Understand `compile_commands.json` (generated by CMake with `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`) — why clang-tidy needs it

**Exercises**
- [ ] Exercise: run `cppcheck --enable=all` on your Phase 2 C sensor logger — fix the genuine warnings; document the false positives and how to suppress them cleanly
- [ ] Exercise: run `clang-tidy` with `cppcoreguidelines-*` checks on your `CppSensorLogger` — apply `modernize-use-nullptr`, `modernize-use-override` fixes
- [ ] Exercise: add clang-tidy as a step in your GitHub Actions workflow — fail on new warnings only (don't block on legacy code)
- [ ] Exercise: compile your `Bmp280` unit tests with ASan + UBSan enabled; run them; confirm no sanitizer errors
- [ ] Commit to `week41/static_analysis/`

---

#### Week 42 — Testing Foundations: fff + Unity
> ~10 hrs | fff GitHub docs + ThrowTheSwitch Unity docs

**Study tasks**
- [ ] Understand the testing pyramid for embedded: unit tests (on host, fast) → integration tests (on hardware or simulator) → system tests (full hardware) — most teams are weak on the bottom layer
- [ ] Understand the hardware abstraction layer (HAL) testing problem: your sensor drivers call `i2c_write_register()` — in a test, you don't want real hardware; you need a fake
- [ ] **fff (Fake Function Framework)**: single-header library — `FAKE_VOID_FUNC`, `FAKE_VALUE_FUNC`, `fff.h` — study the [GitHub README](https://github.com/meekrosoft/fff)
- [ ] Understand what fff gives you: call count tracking, argument history, configurable return values — everything you need to verify your driver calls the transport layer correctly
- [ ] **Unity**: lightweight C unit test framework from ThrowTheSwitch — `TEST_ASSERT_EQUAL`, `TEST_ASSERT_NULL`, `RUN_TEST` — designed for embedded, runs on host
- [ ] Understand the difference between fff (faking) and CMock (auto-generating mocks from headers) — fff is manual but transparent; CMock is more powerful but requires Ceedling tooling

**Exercises**
- [ ] Exercise: write unit tests for your `bmp280.c` driver using fff + Unity:
  - [ ] Fake `i2c_read_register()` and `i2c_write_register()` using fff
  - [ ] Test: WHO_AM_I read returns correct chip ID
  - [ ] Test: compensation formula applied correctly to known raw values
  - [ ] Test: error returned when I2C fake returns failure code
  - [ ] Run all tests on host with `gcc` — no hardware required
- [ ] Exercise: write tests for your C++ `Bmp280` class using fff injected via constructor (dependency injection pattern)
- [ ] Exercise: write a host-side `Makefile` target `make test` that compiles and runs all unit tests — separate from the firmware build
- [ ] Commit to `week42/unit_testing/`

---

#### Week 43 — Testing with Python: pytest + pyserial
> ~10 hrs | pytest docs + pyserial docs

**Study tasks**
- [ ] Understand when Python testing complements C unit tests: integration tests that talk to real hardware over serial — verify the full stack, not just logic
- [ ] **pytest**: Python's standard test framework — `assert`, fixtures (`@pytest.fixture`), parametrize (`@pytest.mark.parametrize`), test discovery
- [ ] **pyserial**: `serial.Serial()`, `write()`, `readline()`, `timeout` — communicating with your STM32 or Pi over UART from a test script
- [ ] Understand hardware-in-the-loop (HIL) testing: your PC sends commands over serial, reads responses, asserts on them — the firmware under test runs on real hardware
- [ ] Understand the difference between HIL tests (slow, require hardware, catch integration bugs) and unit tests (fast, on host, catch logic bugs) — both have a role

**Exercises**
- [ ] Exercise: write a `conftest.py` that opens a serial port to your STM32 as a pytest fixture — handles open/close, configurable port via `--port` CLI argument
- [ ] Exercise: write pytest tests for your Phase 2 UART command parser:
  - [ ] `test_led_on`: send `"led on\n"`, assert response contains `"OK"`
  - [ ] `test_led_off`: send `"led off\n"`, assert response contains `"OK"`
  - [ ] `test_unknown_command`: send `"garbage\n"`, assert response contains `"ERR"`
  - [ ] `test_status`: send `"status\n"`, assert response is parseable CSV
- [ ] Exercise: write a Python script that reads 30 seconds of CSV sensor output from your STM32, parses it with `pandas`, and asserts that no samples are missing (frame count monotonically increasing)
- [ ] Exercise: add a `pytest` step to your GitHub Actions workflow — runs against a simulated serial port using `socat` (virtual serial pair) if no hardware is available
- [ ] Commit to `week43/python_testing/`

---

#### Week 44 — Tooling Deliverable + Phase 3 Wrap-Up
> ~10 hrs | Polish, documentation, retrospective

- [ ] Write a unified `CONTRIBUTING.md` for your Phase 3 repo — the document a new team member would read:
  - [ ] Branch naming convention (`feature/`, `fix/`, `docs/`)
  - [ ] Commit message format (Conventional Commits)
  - [ ] How to run the firmware build (`cmake` + `make`)
  - [ ] How to run unit tests (`make test`)
  - [ ] How to run HIL tests (`pytest --port /dev/ttyACM0`)
  - [ ] How to run static analysis (`make lint`)
  - [ ] PR checklist: CI green, `cppcheck` clean, tests pass, reviewer assigned
- [ ] Verify your GitHub Actions workflow covers: build → static analysis → unit tests → artifact upload
- [ ] Write `README.md` for the tooling block covering:
  - [ ] Git workflow used and rationale
  - [ ] CI pipeline overview (diagram)
  - [ ] Testing strategy: what each layer tests and why
  - [ ] Static analysis tools and configuration
- [ ] Tag the repo: `git tag v1.0-phase3-tooling`
- [ ] Write a Phase 3 retrospective in `NOTES.md`:
  - [ ] Yocto: what clicked, what was frustrating, what you'd do differently
  - [ ] C++: which patterns felt natural coming from C, which felt forced
  - [ ] Rust: honest assessment — does it change how you think about C/C++?
  - [ ] Tooling: which tools will you actually use going forward

> 🏁 **M22 checkpoint:** Full tooling pipeline in place. Phase 3 complete. ✅

---

## 📊 Progress Summary

| Week | Topic | Status |
|------|-------|--------|
| 29 | Yocto Concepts + First Build | ✅ Done` |
| 30 | Layers, Recipes + Custom Image | ✅ Done`|
| 31 | Kernel Module Recipe + DT Overlay | ⬜ Not started |
| 32 | Yocto Deliverable + SDK | ⬜ Not started |
| 33 | C++ Introduction + Dev Environment | ⬜ Not started |
| 34 | C++ Fundamentals: Classes + Error Handling | ⬜ Not started |
| 35 | Advanced C++: Templates + Compile-Time | ⬜ Not started |
| 36 | Applied C++: HAL, Patterns + FSM | ⬜ Not started |
| 37 | C++ Deliverable + Integration | ⬜ Not started |
| 38 | Rust Exploration Sprint | ⬜ Not started |
| 39 | Git for Teams | ⬜ Not started |
| 40 | CI/CD: GitHub Actions | ⬜ Not started |
| 41 | Static Analysis + Code Quality | ⬜ Not started |
| 42 | Testing Foundations: fff + Unity | ⬜ Not started |
| 43 | Testing with Python: pytest + pyserial | ⬜ Not started |
| 44 | Tooling Deliverable + Phase 3 Wrap-Up | ⬜ Not started |

> Update statuses to: `🟡 In progress` → `✅ Done`

---

## 📦 Resources

| # | Resource | Format | Link |
|---|----------|--------|------|
| R1 | Mastering Embedded Linux Programming — Chris Simmonds | Book | 2nd ed. (carried over from Phase 2) |
| R2 | Yocto Project Quick Build Guide | Web | [docs.yoctoproject.org/brief-yoctoprojectqs](https://docs.yoctoproject.org/brief-yoctoprojectqs/index.html) |
| R3 | Yocto Project Reference Manual | Web | [docs.yoctoproject.org/ref-manual](https://docs.yoctoproject.org/ref-manual/index.html) |
| R4 | meta-raspberrypi layer | Git | [github.com/agherzan/meta-raspberrypi](https://github.com/agherzan/meta-raspberrypi) |
| R5 | C++ in Embedded Systems — Amar Mahmutbegović | Book | ISBN 978-1-83588-114-9, Packt 2025 |
| R6 | Compiler Explorer | Web | [godbolt.org](https://godbolt.org) — paste C++ code, see ARM assembly live |
| R7 | Embedded Template Library (ETL) | Web | [www.etlcpp.com](https://www.etlcpp.com) |
| R8 | The Embedded Rust Book | Web (free) | [docs.rust-embedded.org/book](https://docs.rust-embedded.org/book/) |
| R9 | Embassy (async embedded Rust framework) | Web | [embassy.dev](https://embassy.dev/) |
| R10 | Pro Git — Chacon & Straub | Web (free) | [git-scm.com/book](https://git-scm.com/book/en/v2) |
| R11 | GitHub Actions documentation | Web | [docs.github.com/en/actions](https://docs.github.com/en/actions) |
| R12 | fff — Fake Function Framework | GitHub | [github.com/meekrosoft/fff](https://github.com/meekrosoft/fff) |
| R13 | Unity — C unit test framework | Web | [throwtheswitch.org/unity](https://throwtheswitch.org/unity) |
| R14 | pytest documentation | Web | [docs.pytest.org](https://docs.pytest.org) |
| R15 | pyserial documentation | Web | [pyserial.readthedocs.io](https://pyserial.readthedocs.io) |
| R16 | cppcheck | Web | [cppcheck.sourceforge.io](https://cppcheck.sourceforge.io) |
| R17 | clang-tidy | Web | [clang.llvm.org/extra/clang-tidy](https://clang.llvm.org/extra/clang-tidy/) |

---

## 🏁 Milestone Overview

| Milestone | Target Week | Description |
|-----------|-------------|-------------|
| M16 | End of Week 29 | Yocto `core-image-minimal` boots on Pi 4B |
| M17 | End of Week 31 | Custom Yocto image with kernel module + systemd service baked in |
| M18 | End of Week 32 | Yocto deliverable + SDK generated. Block complete ✅ |
| M19 | End of Week 37 | C++ sensor logger on STM32, CMake build, static analysis clean ✅ |
| M20 | End of Week 38 | Rust blink + UART on STM32. Reflection written |
| M21 | End of Week 40 | GitHub Actions CI live — build + lint on every push ✅ |
| M22 | End of Week 44 | Full tooling pipeline. Phase 3 complete ✅ |

---

## 🗂️ Suggested Repo Structure

```
embedded-phase3/
├── week29/yocto_first_build/       ← local.conf, bblayers.conf, notes
├── week30/meta-avindeli/           ← custom Yocto layer
├── week31/kernel_module_recipe/    ← .bb recipe + DT overlay
├── week32/                         ← README, NOTES, tag v1.0-phase3-yocto
├── week33/cpp_intro/               ← ring buffer port, CMake setup
├── week34/cpp_fundamentals/        ← Bmp280 class, UartLogger, fff tests
├── week35/cpp_advanced/            ← templates, constexpr lookup table
├── week36/cpp_applied/             ← GpioPin class, FSM, Observer pattern
├── week37/CppSensorLogger/         ← integrated C++ deliverable
├── week38/rust_blink/              ← Embassy blink + UART, RUST_NOTES.md
├── week39/git_teamwork/            ← workflow notes, .gitignore
├── week40/github_actions/          ← .github/workflows/ci.yml
├── week41/static_analysis/         ← cppcheck config, clang-tidy config
├── week42/unit_testing/            ← fff + Unity tests, make test target
├── week43/python_testing/          ← pytest suite, conftest.py
├── week44/                         ← CONTRIBUTING.md, README, NOTES, tag v1.0-phase3-tooling
└── README.md
```

---

## 📝 Notes & Decisions Log

> Use this section to log design decisions, bugs worth remembering, and things to revisit in Phase 4.

| Date | Note |
|------|------|
| | |