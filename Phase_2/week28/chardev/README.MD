# Week 28 — Kernel Modules & Linux Integration

## Goal

Build a complete Linux kernel character device driver from scratch, expose it to userspace via `/dev/mydev`, cross-compile a userspace consumer application on the host, and wire the full pipeline into systemd so it comes up automatically at boot. Milestone M15: cross-compiled kernel module loading on Pi, Phase 2 complete.

## Scope

- Device Tree overlay mechanics: `.dts` / `.dtsi` / `.dtb` / `.dtbo`, `__symbols__` and `__fixups__` nodes, runtime merging by the bootloader
- Kernel module lifecycle: `module_init()` / `module_exit()`, `__init` / `__exit` section placement and memory reclaim semantics
- Kernel build infrastructure: `/lib/modules/$(uname -r)/build`, `vermagic` string, built-in (`=y`) vs loadable (`.ko`) modules
- `printk` severity levels, `insmod` / `rmmod` / `lsmod`
- `module_param()` — passing parameters at `insmod` time
- Character device registration: `register_chrdev()`, major/minor numbers, `/proc/devices`
- `file_operations` struct as a function pointer dispatch table
- Userspace/kernel address space boundary: `copy_to_user()` / `copy_from_user()`
- EOF signalling via `*loff_t` tracking
- Cross-compilation with `aarch64-linux-gnu-gcc` targeting Pi glibc
- systemd `.service` units: `Type=oneshot`, `ExecStart=`, `After=`, `Requires=`, `RemainAfterExit=`
- Dynamic major number discovery via `/proc/devices`

## Architecture

```
Host (Ubuntu VM)                        Pi (192.168.0.165)
─────────────────                       ──────────────────────────────────
aarch64-linux-gnu-gcc                   /lib/modules/.../chardev.ko
  user_program.c ──► user_program ──►  /home/avindeli/.../user_program

Kernel build (native on Pi):
  chardev.c + Makefile ──► chardev.ko

Boot sequence (systemd):
  chardev-load.service
    ExecStart[1]: insmod chardev.ko          ← registers major, creates /proc/devices entry
    ExecStart[2]: chardev-setup.sh           ← reads major from /proc/devices, mknod + chmod
         │
         ▼
  chardev-app.service  (After= + Requires= chardev-load)
    ExecStart: user_program                  ← open("/dev/mydev") → read() → printf → close()

Kernel space:
  /dev/mydev
    open()    → char_dev_open()             (noop)
    read()    → char_dev_read()             → snprintf(counter) → copy_to_user()
    write()   → char_dev_write()            → counter++
    release() → char_dev_release()          (noop)
```

## Key design decisions

**Native module compilation on Pi vs cross-compilation**
Kernel modules must match the running kernel's `vermagic` string exactly — kernel version, compiler version, SMP flags, and preemption model. The Pi already has the exact matching headers under `/lib/modules/$(uname -r)/build`, so modules are compiled natively on the Pi. The userspace application (`user_program.c`) has no such constraint and is cross-compiled on the Ubuntu VM with `aarch64-linux-gnu-gcc`, then deployed via rsync.

**Dynamic major number with `/proc/devices`**
`register_chrdev(0, ...)` requests a dynamically assigned major number. Hardcoding the result (e.g. 236) into `mknod` is fragile — it can change across reboots or if modules load in a different order. `chardev-setup.sh` reads the assigned major at runtime with `grep mydev /proc/devices | awk '{print $1}'`, making the device node creation robust across reboots.

**Two-service systemd split**
Module loading and device node creation are separated from the userspace application into two `.service` files. This makes the dependency explicit (`After=` / `Requires=`) and allows either component to be restarted independently. `Type=oneshot` with `RemainAfterExit=yes` is used for both — the processes exit after completing their work but systemd considers them "active", which is required for the `Requires=` dependency chain to hold.

**`*loff_t` for EOF signalling**
`char_dev_read()` uses `*loff` to track whether the counter string has already been sent in this open file description. On the first `read()`, `*loff == 0` so the data is sent and `*loff` is advanced by `len`. On subsequent `read()` calls within the same open, `*loff > 0` returns 0 (EOF). When the process closes and reopens the device, `*loff` resets, so the counter is readable again on every fresh `open()`.

## Lessons learned

- `copy_to_user()` returns the number of bytes that *could not* be copied — 0 on full success. The error check is `if (ret_val > 0)`, not `< 0`.
- Variable declarations after a statement (the `if (*loff > 0) return 0` guard) are valid in C99 but worth ordering carefully for readability.
- `printf(buffer)` is a format string vulnerability when `buffer` contains user-controlled or device data. Always use `printf("%s", buffer)`.
- `read()` does not null-terminate — `buffer[bytes_read] = '\0'` must be done explicitly before passing to `printf`. The termination belongs inside the `while` loop, not after it.
- `insmod` fails if the module is already loaded. In a systemd context this means `rmmod` must be run before restarting the service, or `modprobe` used instead (requires the module to be installed in the kernel module tree).
- `ExecStart=` in a systemd unit is not a shell — `&&` does not work. Multiple `ExecStart=` lines in a `Type=oneshot` service run sequentially.
- Shell scripts called from systemd must be marked executable (`chmod +x`) — `status=203/EXEC` is the error when they are not.
- `git push` does not push tags by default — `git push origin main --tags` is required.
- Kernel build artifacts to add to `.gitignore`: `*.ko`, `*.mod`, `*.mod.c`, `*.order`, `*.symvers`, `.*.cmd`.

## Comparison: when to use each approach

| | Bare-metal STM32 | Linux userspace | Linux kernel driver |
|---|---|---|---|
| **Abstraction** | None — direct register access | OS syscall API (`open`/`read`/`write`) | Kernel APIs (`file_operations`, `copy_to_user`) |
| **Latency** | Deterministic, sub-microsecond | Non-deterministic, OS scheduling overhead | Low, but still kernel context switch |
| **Development speed** | Slow — no stdlib, no debugging tools | Fast — full toolchain, `gdb`, `strace` | Medium — kernel headers, `dmesg` for debugging |
| **Safety** | A bug crashes the chip | A bug crashes the process, OS survives | A bug panics the kernel |
| **Use when** | Hard real-time, resource-constrained, no OS | Sensor reading, data processing, networking | Custom hardware, shared resource arbitration, interrupt handling |
| **Example** | BMP280 @ 100 Hz with SysTick ISR | Reading BMP280 via `/dev/i2c-1` with ioctl | Writing a driver for a novel peripheral |

## Open items

- `modprobe` integration: install `chardev.ko` into `/lib/modules/$(uname -r)/extra/` and run `depmod` so `modprobe` can be used in place of `insmod`, enabling graceful handling of already-loaded modules.
- `udev` rule to replace the `mknod` + `chmod` in `chardev-setup.sh` — the proper production approach is a `/etc/udev/rules.d/` rule triggered when the module registers the device.
- Persistent counter: currently resets to 0 on every boot. Could be persisted to a file and restored in `module_init()`.
- Device Tree overlay: no custom overlay was written in this block. A natural next exercise would be registering the device in the DT and having the driver bind via `of_match_table`.