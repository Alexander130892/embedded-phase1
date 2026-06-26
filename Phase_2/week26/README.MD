# Week 26 — Linux Device Drivers Concepts + GPIO from Userspace

## Goal

Exercise GPIO, I2C, and SPI from Linux userspace in C on a Raspberry Pi 4B (Debian 13, aarch64), cross-compiled from an Ubuntu 24.04 VM (amd64). Build a working mental model of the userspace/kernel boundary — `ioctl` request encoding, `/dev` vs `/sys` vs `/proc`, and the userspace driver model — before applying it through three concrete exercises.

**M14 checkpoint: GPIO, I2C, and SPI all exercised from Linux userspace in C.** ✅ Complete.

## Scope

- Linux privilege rings and the syscall boundary (`ioctl` as a ring 3 → ring 0 transition)
- `/dev` (I/O channel) vs `/sys` (kernel object attributes) vs `/proc` (kernel/process runtime state)
- `ioctl` request encoding: direction, size, type, nr — `_IO`/`_IOR`/`_IOW`/`_IOWR`
- `libgpiod` v2 object model: chip → line_settings → line_config → request_config → line_request
- `I2C_RDWR` combined transactions (repeated START, single STOP) vs separate `read()`/`write()` calls
- `SPI_IOC_MESSAGE(n)` and CS-held multi-phase transfers vs `read()`/`write()` (CS deasserts between calls)
- Cross-compiling against a target-specific library version via Debian/Ubuntu multiarch, and as a fallback, building from upstream source with `--host=` autotools cross-compilation
- GPG key verification for a `deb-src` repository, fingerprint-checked against Debian's published keys
- SPI signal integrity: unset `speed_hz` defaults are not necessarily conservative

## Architecture

```
Ubuntu 24.04 VM (amd64)                    Raspberry Pi 4B (aarch64, Debian 13)
─────────────────────────                  ──────────────────────────────────
aarch64-linux-gnu-gcc                      /dev/gpiochip0   (pinctrl-bcm2711)
  -static -lgpiod  ───────┐                /dev/i2c-1       (i2c_bcm2835 + i2c-dev)
                          │   scp           /dev/spidev0.0   (spi0, CS0)
  blink_led  ─────────────┼──────────────►  ./blink_led   → GPIO18, LED, 220Ω
  i2c_test   ─────────────┼──────────────►  ./i2c_test    → MPU-6050 @ 0x68, WHO_AM_I
  spi_test   ─────────────┴──────────────►  ./spi_test    → BMP280 @ CS0, chip_id reg 0xD0

libgpiod v2.2.4 built from source, statically linked
(Debian trixie ships 2.2.1; Ubuntu noble's arm64 package is v1.6.3 — version
mismatch resolved by building upstream from source rather than via apt)
```

Each exercise follows the same userspace pattern: `open()` the device node → configure via `ioctl` (or libgpiod's settings objects) → perform the I/O → `close()`. No kernel module written this week — that's Week 27.

## Key design decisions

**Static linking over dynamic, for the cross-compiled libgpiod.** The Pi already has its own system `libgpiod.so` (2.2.1) at the standard path. Dynamically linking against the locally-built 2.2.4 would mean the binary actually resolves against whichever `.so` the dynamic linker finds at runtime on the Pi — not necessarily the one built and tested against. `-static -lgpiod` produces a fully self-contained binary (verified via `readelf -d | grep NEEDED` showing zero shared dependencies), eliminating that ambiguity entirely.

**Building libgpiod from upstream source (tag v2.2.4) rather than via Debian source packages.** Ubuntu noble's `libgpiod-dev` is v1.6.3 (old API); Debian trixie's is v2.2.1 (the API actually on the Pi). Investigated `apt source libgpiod` against a `deb-src` Debian trixie entry (with GPG key fingerprint-verified against `ftp-master.debian.org`) as a way to get an exact version match, but chose to build from the upstream git tag instead — faster, and within the same `2.2.x` minor series semver guarantees no API differences from `2.2.1`, while a Debian source-package build doesn't transfer usefully to Week 27's kernel-module cross-compiling (which needs matching kernel headers, a different sysroot concern entirely). The multiarch/deb-src infrastructure remains in place and reusable for future arm64 package needs.

**`I2C_RDWR` / `SPI_IOC_MESSAGE` over separate `read()`/`write()` calls.** Both APIs exist specifically to hold the bus in one continuous transaction — repeated START for I2C, CS held low for SPI — matching what the BMP280/MPU-6050 actually require at the protocol level. Two separate syscalls would each be a complete, independent transaction (STOP/CS-deassert in between), breaking devices that expect register-address-then-data as one atomic exchange.

**Explicit `SPI_IOC_WR_MAX_SPEED_HZ` rather than relying on the zero-initialized default.** See Lessons Learned — this was the actual bug, not a design choice made up front, but it's now a deliberate part of the SPI setup pattern going forward.

## Lessons Learned

- **`ioctl()`'s third argument needs the request macro it was built with, not a re-derivation.** `_IOWR(type, nr, size)` is how the kernel header *author* constructed `I2C_RDWR`/`SPI_IOC_MESSAGE` as numeric constants — you just use the resulting named constant (`I2C_RDWR`), you don't call `_IOWR` yourself at the call site.
- **`gpiod_chip_request_lines(chip, req_cfg, line_cfg)` is the real v2 function name** — not `gpiod_line_request_new`, which I'd assumed by analogy to the other `_new()` constructors. Verified against the actual header rather than trusted from memory; this is now a standing habit (grep the header, don't guess the API).
- **Only the final handle needs to outlive its construction.** `gpiod_line_request *` must stay alive for the blink loop; `chip`, `line_settings`, `line_config`, `request_config` are all consumed during construction and can be freed immediately after — confirmed by checking `gpiod_line_request_set_value()`'s actual signature, which takes none of the scaffolding objects.
- **Process death releases a libgpiod line's *ownership claim*, not its electrical state.** A crashed process no longer holds the line, so another process can request it — but the GPIO pin itself stays at whatever level it was last driven to, until something else claims and changes it.
- **`/dev` vs `/sys` vs `/proc` is a real architectural distinction, not three paths to the same thing.** `/dev` is an I/O channel, `/sys` is kernel object attributes/configuration, `/proc` is kernel/process runtime state. Using `/sys/class/gpio/value` in a toggle loop (the deprecated approach) was architecturally wrong for the same reason using a config file as a data channel would be.
- **A kernel module being loaded (`i2c_bcm2835`) does not mean the corresponding `/dev` node exists.** The bus *controller* driver and the `i2c-dev` module (which actually creates `/dev/i2c-N` for userspace access) are separate, independently-loaded pieces. `dmesg` showed the controller registering; `lsmod` never showed `i2c_dev` — `modprobe i2c-dev` was the actual fix, persisted via `/etc/modules`.
- **`SPI_IOC_MESSAGE`'s `speed_hz = 0` (from zero-initializing the transfer struct) means "use the driver default," and that default is not necessarily conservative.** Got a stable, *wrong*, repeatable chip_id read (`0x0B`, then `0x18` across sessions) at the unset default — not random noise, which pointed away from a pure wiring fault and toward timing/signal integrity over the breadboard wiring. Explicitly setting `speed_hz = 500000` via `SPI_IOC_WR_MAX_SPEED_HZ` fixed it immediately and reproducibly (5/5 correct reads at `0x58`).
- **Cross-compilation solves CPU-architecture mismatch only — not library version mismatch.** `aarch64-linux-gnu-gcc` producing ARM64 code says nothing about whether the headers/libraries it links against match what's actually installed on the target. Ubuntu and Debian both package `libgpiod-dev` under the same name at independently-frozen versions; assuming parity because the package name matches is a real and easy mistake.
- **GPG fingerprint verification is cheap and caught a real (if non-malicious) error mid-session** — an initially-cited Debian key fingerprint turned out to be the *release* key, not the *archive automatic signing* key actually in `archive-key-13.asc`; both are legitimate Debian keys, but verifying against the actual published listing (rather than trusting a single cited value) resolved the discrepancy correctly instead of either blindly trusting or blindly rejecting the download.
- **Relay module debugging (hardware, adjacent to this week's main exercises):** a 5V-rated relay coil driven from 3.3V can exhibit pull-in/hold-in asymmetry — enough voltage to occasionally latch on, not enough margin to reliably release, producing a "switches once, then stays stuck on" symptom that looks like a software/logic fault but is purely a coil power-budget issue. Fix: VCC from the Pi's 5V rail, common GND, IN (logic-level only) from 3.3V GPIO — standard wiring for this module class against a 3.3V-logic host.
- **Build artifacts don't belong in git.** `libgpiod-cross/` (cloned upstream source + compiled install tree) and the compiled binaries (`blink_led`, `i2c_test`, `spi_test`) are excluded via a scoped `.gitignore` in `week26/linux_gpio_i2c_spi/`; only `.c` sources and documentation are tracked. Deployment to the Pi is via `scp`, entirely separate from version control.
- **Bare-metal vs. userspace driver — the reflection:** on STM32, owning every register meant register-misconfiguration was the dominant bug category. This week's bugs were structurally different — API misuse (wrong types, wrong function names, unchecked return values) and missing infrastructure (`i2c-dev` not loaded), rather than hardware-configuration mistakes. Per-transaction configurability still exists in userspace (`SPI_IOC_WR_MAX_SPEED_HZ` did fix the actual bug) but is narrower in scope than STM32's direct register ownership: a userspace override only applies to your own file descriptor for your process's lifetime, mediated by a kernel that has to stay consistent for any other potential user of the same shared bus.

## Open Items

- BMP280/MPU-6050 exercises only read identification registers (`chip_id`, `WHO_AM_I`) — no actual sensor data (pressure/temperature, accel/gyro) parsed yet. Natural follow-up if revisited, though not required by this week's checklist.
- `week26/linux_gpio_i2c_spi/` setup documented in `CROSS_COMPILE_LIBGPIOD.md`; the `apt source`/`deb-src`/Debian-keyring path explored but not used for the final solution remains available on the VM for future arm64 package needs.
