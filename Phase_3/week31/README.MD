# Week 31 — Kernel Module Recipe + Device Tree Overlay

## Goal

Formalize Phase 2's hand-built `chardev` character device driver and userspace app into
proper Yocto recipes on a Raspberry Pi 4B (Scarthgap / Yocto 5.0.18, `meta-avindeli` custom
layer), covering kernel module recipes, Device Tree overlays, and boot-time service
integration.

**M17 checkpoint — achieved:** custom Yocto image boots with a self-authored kernel module,
a compiled and merged Device Tree overlay, and an init-managed userspace service, with zero
manual post-boot steps.

## Scope

- `linux-yocto` vs `linux-raspberrypi` kernel recipe selection (`PREFERRED_PROVIDER_virtual/kernel`)
- `inherit module` (formerly assumed to be `kernel-module.bbclass` — corrected mid-session) and `KERNEL_MODULE_AUTOLOAD`
- Out-of-tree kernel module build internals: `module_do_compile`, `module_do_install`, `STAGING_KERNEL_DIR`
- `LIC_FILES_CHKSUM` / SPDX licensing workflow for custom recipes
- Device Tree overlay authoring (`/dts-v1/`, `/plugin/`, `fragment@0`, `brcm,pins`/`brcm,function`/`brcm,pull`)
- `RPI_KERNEL_DEVICETREE_OVERLAYS`, `KERNEL_DEVICETREE`, and `IMAGE_BOOT_FILES` — and where each is actually scoped
- Machine-conf customization via `require` + derived `MACHINE` (`raspberrypi4-64-avindeli.conf`)
- `rpi-config`'s parameterized `config.txt` generation (`ENABLE_UART`, `RPI_EXTRA_CONFIG`)
- systemd vs. sysvinit as Yocto init managers, and `update-rc.d.bbclass` as the sysvinit equivalent of `systemd.bbclass`
- sstate-cache behavior under forced task reruns, `cleansstate`/`cleanall`, and pseudo database desync recovery

## Architecture

```
Boot
 │
 ├─ Bootloader reads config.txt (enable_uart=1, dtoverlay=mydev)
 │   └─ merges mydev.dtbo into base DTB (BCM21 → GPIO fn, no pull)
 │
 ├─ Kernel boots, mounts rootfs
 │
 ├─ systemd-modules-load.service (from KERNEL_MODULE_AUTOLOAD)
 │   └─ modprobe chardev
 │       └─ chardev_init() → register_chrdev(0, "mydev", &char_dev_ops)
 │           └─ /proc/devices: "239 mydev"
 │
 ├─ sysvinit: S99mydev-app (/etc/init.d/mydev-app start)
 │   ├─ mknod /dev/mydev c 239 0   (devtmpfs doesn't auto-create —
 │   │                              chardev.c uses register_chrdev(),
 │   │                              not device_create()/class_create())
 │   ├─ chmod 666 /dev/mydev
 │   └─ /usr/bin/user_program
 │       └─ open("/dev/mydev") → read() loop → prints counter value
 │
 └─ /dev/mydev now readable by any user: cat /dev/mydev → "0"
```

Recipe layout (`meta-avindeli/`):

```
recipes-kernel/chardev/chardev_1.0.bb          — kernel module recipe (inherit module)
recipes-kernel/linux/linux-raspberrypi_%.bbappend — DT overlay compile + Makefile patch
recipes-mydev/mydev-app/mydev-app_1.0.bb        — userspace app + init.d script
conf/machine/raspberrypi4-64-avindeli.conf      — require + ENABLE_UART + RPI_EXTRA_CONFIG
                                                   + RPI_KERNEL_DEVICETREE_OVERLAYS:append
recipes-images/images/core-image-avindeli.bb    — IMAGE_INSTALL:append = hello chardev mydev-app
```

## Key design decisions

**`inherit module`, not `kernel-module.bbclass`.** Initially assumed the class name matched
the kernel-module concept literally. `find`ing the actual file on disk (rather than trusting
a plausible-sounding name) revealed the real class is `module.bbclass`. Lesson generalized:
verify class/variable names against the filesystem before writing code around them, since
naming conventions in Yocto don't always match intuition — and outdated references (including
me, mid-session) get this wrong.

**Makefile must forward `KERNEL_SRC`/`O`, not hardcode `uname -r`.** The Week 27 Makefile's
`all:` target ran `make -C /lib/modules/$(shell uname -r)/build`, which is correct for
standalone dev-machine builds but silently wrong inside Yocto — `oe_runmake` injects
`KERNEL_SRC=...` and `O=...` on the command line pointing at the Yocto-built target kernel,
and a Makefile that ignores them ends up cross-compiling against the *host's* x86_64 kernel
headers instead. Fixed by making `all:`/`modules_install:`/`clean:` all reference `$(KERNEL_SRC)`
explicitly, while keeping a `KDIR ?=` fallback for standalone use.

**DT overlay source files must end in `-overlay.dts`, not just `.dts`.** kbuild's own
`scripts/Makefile.lib` pattern rule is `$(obj)/%.dtbo: $(src)/%-overlay.dts` — a plain
`mydev_overlay.dts` compiling to a target named `mydev_overlay.dtbo` doesn't match that rule
at all, regardless of whether the file is present and regardless of whether it's correctly
listed in the overlays `Makefile`'s `dtbo-y`. Renamed to `mydev-overlay.dts` → `mydev.dtbo`.

**`RPI_KERNEL_DEVICETREE_OVERLAYS` needs to be set in the machine conf, not only the kernel
`.bbappend`.** The kernel recipe's own `.bbappend` `:append` is scoped to that recipe's
variable namespace. But the actual boot-partition file list (`IMAGE_BOOT_FILES`) is computed
by `make_dtb_boot_files(d)` inside `rpi-base.inc`, evaluated in the **image recipe's**
variable space — which never sees a `.bbappend`-scoped append on a different recipe. The
`.dtbo` compiled correctly either way (kernel recipe's own scope was sufficient for
`do_compile`), but it never reached `/boot/overlays/` on the SD card until the same
`RPI_KERNEL_DEVICETREE_OVERLAYS:append` line was duplicated into the machine conf, which is
globally visible.

**sysvinit init script instead of systemd unit files.** Originally wrote `mydev-mknod.service`
+ `mydev-app.service` with `inherit systemd`. Build succeeded, but the `.service` files were
silently stripped from the package by `rm_systemd_unitdir` — `poky`'s default `DISTRO_FEATURES`
includes `sysvinit`, not `systemd`, so systemd unit files are dead weight and get removed as a
matter of packaging policy. Rather than switch the whole distro's init manager (a much bigger
architectural change), redesigned as a single combined `/etc/init.d/mydev-app` script using
`inherit update-rc.d` with `INITSCRIPT_NAME`/`INITSCRIPT_PARAMS = "defaults 99"` — priority 99
guarantees it runs after the (much earlier-numbered) module-autoload init script, since
sysvinit ordering is purely priority-number-based rather than dependency-graph-based like
systemd's `After=`/`Requires=`.

**`mknod` kept in the init script rather than fixing the driver.** `chardev.c` uses the older
`register_chrdev()` API, which doesn't create a `struct device` that `devtmpfs` watches —
so `/dev/mydev` is never auto-created, confirmed directly (`ls /dev/mydev` → No such file).
The "correct" long-term fix is adding `class_create()`/`device_create()` to the driver; kept
the `mknod`-in-init-script workaround for this week's scope and flagged the driver-level fix
as a follow-up.

## Lessons Learned

- `.bbappend` files must mirror the target recipe's path *inside its own layer*
  (`recipes-kernel/linux/`), not an arbitrary location — BitBake finds them via `BBPATH`
  search across all registered layers, but the convention keeps things discoverable.
- `FILESEXTRAPATHS:prepend := "${THISDIR}/files:"` needs `:=` (immediate expansion) — `${THISDIR}`
  must resolve at parse time to the `.bbappend`'s own directory, not lazily later.
- `LIC_FILES_CHKSUM` workflow: let `do_populate_lic`'s QA error report the real md5 rather than
  guessing — but always double-check the checksum is being computed against the *staged* copy
  of the source file (`files/`), not a stale duplicate sitting one directory level up.
- `do_configure:prepend()` / `do_install:prepend()` (not a bare override) is the safe way to
  inject setup steps without silently replacing a class's substantial existing task body.
- `KERNEL_MODULE_AUTOLOAD` writes `/etc/modules-load.d/<name>.conf`, read by
  `systemd-modules-load.service` → `modprobe`, **only on systemd images**. On sysvinit images
  the equivalent path is `/etc/modules`, read by an early-priority init script — the underlying
  Yocto variable is the same, but the delivery mechanism is init-system-dependent.
  `KERNEL_MODULE_AUTOLOAD` writes the correct file for whichever init manager is active without needing
  recipe-level changes.
- Compressed `.ko.xz` modules load transparently via `modprobe`/`kmod` — compression is
  invisible to `KERNEL_MODULE_AUTOLOAD`, no special handling needed.
- `bitbake -e <recipe> | grep ^VAR=` is the reliable way to see a variable's *final*, fully
  resolved value (post all `:append`/`:prepend`/`??=` layering) — much faster than guessing
  from source alone, and it's what caught the `RPI_KERNEL_DEVICETREE_OVERLAYS` scoping bug.
- Forced task reruns (`-c <task> -f`) and `cleansstate`/`cleanall` can leave the **pseudo**
  fakeroot database out of sync with real inode numbers on disk after enough manual
  force-reruns during debugging — surfaces as `do_package` failing with `path mismatch`/`inode
  mismatch` errors. `cleansstate` + `cleanall` on the affected recipe resolves it; no manual
  pseudo-db surgery needed.
- `bbwarn` inside a task is a fast, unambiguous way to confirm whether a `:prepend`/`:append`
  function body is actually executing, vs. staring at file content and assuming — caught a
  case where a `sed` line had gone missing from a `.bbappend` that *looked* fine on a
  cursory `cat`.
- BusyBox `od` only supports its legacy single-letter flags (`-D`, `-X`) — no GNU long-form
  `-A`/`-t`. Device tree property values are big-endian; reading them with a little-endian-native
  `od` requires manually reversing the byte order by eye.
- Pin muxing (Device Tree, `brcm,function`) and GPIO line direction/drive state are two
  separate layers — an overlay correctly configuring a pin as "GPIO function" does not by
  itself put the line into an actively-driven output state; that still requires a runtime
  consumer (a driver's `gpio-hog`, or userspace via `libgpiod`).

## Open Items

- `chardev.c`: add `class_create()` + `device_create()` so `/dev/mydev` is created by
  `devtmpfs` automatically, removing the need for the `mknod`-in-init-script workaround.
- Wire up an actual `libgpiod` call (reusing Week 26 skills) from `user_program` or a small
  companion tool to explicitly drive BCM21 high/low and verify the wired LED — the DT overlay
  only established boot-time pin muxing, not runtime drive state.
- `local.conf` additions (`MACHINE = "raspberrypi4-64-avindeli"`) are not layer-tracked by
  design (build-directory-local) — the staging repo keeps a copy for reference, but this
  remains a manual step on any fresh `poky/build/` checkout.
- Consider whether `poky`'s sysvinit default should eventually be revisited in favor of
  systemd for a real product image, given systemd's richer dependency ordering — deferred
  as a deliberate architectural decision, not a bug.