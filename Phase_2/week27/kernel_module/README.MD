# Week 27 — Device Tree Basics + Simple Kernel Module

## Goal

Understand the Linux Device Tree overlay system on the Raspberry Pi 4B, then write and load three kernel modules from scratch: a minimal "Hello, kernel" module, a `module_param()` exercise, and a character device driver exposing a counter via `/dev/mydev`. All modules compiled natively on the Pi and tested live with `insmod`/`rmmod`/`dmesg`.

## Scope

- Device Tree overlay mechanics: `__symbols__`, `__fixups__`, bootloader merge flow
- Kernel config files: `/boot/config.txt` vs `config-<version>` distinction
- Kernel header versioning and why cross-compilation doesn't apply to modules
- `module_init()` / `module_exit()` lifecycle and `__init` / `__exit` section placement
- `printk()` severity levels and the kernel ring buffer
- `module_param()` parameter passing at `insmod` time
- Character device registration: `register_chrdev()`, `file_operations`, `copy_to_user()`
- Major/minor number allocation and manual device node creation with `mknod`

## Architecture

### Device Tree Overlay Flow

```
bcm2711-rpi-4-b.dtb          (base tree — compiled with -@, contains __symbols__)
+ i2c.dtbo                   (overlay — contains __fixups__ referencing &i2c1)
+ mpu6050.dtbo               (further overlay)
─────────────────────────────
→ single merged DTB in RAM   (what the kernel parses — bootloader did the merge)
```

### Character Device Flow

```
userspace: cat /dev/mydev
     │
     ▼
  VFS layer
     │  looks up major number (236) → finds "mydev" driver
     ▼
char_dev_read()
     │  snprintf(buffer, "%d", counter)
     │  copy_to_user(user_buf, buffer, len)
     ▼
userspace buffer receives counter value as ASCII string

userspace: echo "x" > /dev/mydev
     │
     ▼
char_dev_write()
     │  counter++
     │  return size
     ▼
counter incremented, caller sees full write success
```

### Module File Layout

```
week27/kernel_module/
├── hello/
│   ├── hello.c          ← module_init/exit + printk
│   └── Makefile
└── chardev/
    ├── chardev.c        ← fops, register_chrdev, copy_to_user
    └── Makefile
```

## Key Design Decisions

**Native compilation on the Pi instead of cross-compilation.** For userspace binaries, cross-compiling on the Ubuntu VM makes sense — the Pi is the deployment target. For kernel modules, the build must use the exact headers, `Module.symvers`, and Kbuild infrastructure matching the running kernel (`6.18.34+rpt-rpi-v8`). The Pi already has all of this installed under `/lib/modules/$(uname -r)/build`. Cross-compiling would require copying the full Kbuild tree from the Pi to the VM and keeping them in sync — not worth it for modules this size.

**Dynamic major number allocation.** Passing `0` as the major argument to `register_chrdev()` asks the kernel to assign a free major number dynamically, rather than hardcoding one. The assigned number (236 in this session) is captured from the return value, stored in a file-scope `static int major_nr`, printed via `printk(KERN_INFO ...)` at init, and passed to `unregister_chrdev()` at exit. Hardcoding a major number risks collisions with other drivers.

**EOF signalling in `char_dev_read()`** — `cat` calls `read()` in a loop until it receives `0` (EOF). Without checking `*loff`, the driver would return the counter value on every call forever. The fix is to check `if (*loff > 0) return 0` at the top of the function. After a successful read, `*loff` is advanced by `len` bytes, so the second call returns EOF immediately and `cat` terminates.

**Returning `size` from `char_dev_write()`** — the write handler ignores the actual bytes sent by userspace (any write just increments the counter) but must still return `size` to signal that all bytes were consumed. Returning anything less would cause the caller to retry the write, interpreting it as a short write.

## Lessons Learned

- **`__init` frees code after use, `__exit` can be omitted entirely for built-ins.** `__init` places the function in `.init.text` — the kernel frees this section after all init functions complete. `__exit` on a `=y` built-in is stripped at link time by the linker because `rmmod` is impossible for a built-in, so the code is dead before the image even boots.

- **Missing or buggy `module_exit()` sets a landmine.** A registered interrupt handler points to code inside the module. If the module unloads without unregistering the handler, the next interrupt fires to an address containing whatever memory the kernel has since reused. Immediate kernel panic, no recovery until reboot.

- **`module_init(fn)` and `module_exit(fn)` register pointers, they don't call the functions.** Writing `my_init();` at file scope is a bare function call — illegal in C at file scope and wrong semantically even if it weren't. The macros tell the kernel which function to invoke at `insmod`/`rmmod` time.

- **`MODULE_LICENSE`, `MODULE_AUTHOR`, `MODULE_DESCRIPTION` are macros, not `#define`s.** They emit ELF section metadata the kernel reads at load time. `#define MODULE_LICENSE` just creates an empty preprocessor symbol — the module loads but `loading out-of-tree module taints kernel` taint flags are worse without the GPL declaration.

- **Variable shadowing in `chardev_init()`.** Writing `int major_nr = register_chrdev(...)` inside the function declares a new local variable that shadows the file-scope `static int major_nr`. The file-scope variable stays `0` forever, so `unregister_chrdev()` always unregisters the wrong major number. Remove the `int` to assign to the file-scope variable.

- **`copy_to_user()` returns bytes *not* copied, not bytes copied.** A non-zero return is failure. The error return to userspace must be `-EFAULT`, not the raw positive return value — the kernel convention is negative error codes, and returning a positive value signals a successful partial read to the caller.

- **`/boot/config.txt` and `/boot/config-<version>` are completely different files.** `config.txt` is the GPU firmware config (dtoverlay, arm_64bit, etc.). `config-6.18.34+rpt-rpi-v8` is the saved `.config` from the kernel build — thousands of `CONFIG_*` flags. Two `rpi-v8` vs `rpi-2712` variants exist because Pi 4 (BCM2711) and Pi 5 (BCM2712) have separate kernel builds.

- **Kernel header path for the Pi is `linux-headers-<ver>+rpt-common-rpi`, not `rpi-common`.** The suffix order matters for grepping headers directly. Use `ls /usr/src/` to confirm the exact directory name before constructing paths.

- **Locale warning on the Pi is harmless for compilation** but fixable via `sudo sed -i 's/# en_US.UTF-8 UTF-8/en_US.UTF-8 UTF-8/' /etc/locale.gen && sudo locale-gen`.

## Open Items

- Exercise: write a character device with `copy_from_user()` — accept a value written by userspace and store it (rather than ignoring the write payload)
- Investigate `udev` rules to auto-create `/dev/mydev` on `insmod` instead of manual `mknod`
- Explore `/sys/module/chardev/parameters/` visibility for `module_param()` entries
- Week 28: device driver with actual hardware — wire `chardev` to a GPIO or I2C peripheral