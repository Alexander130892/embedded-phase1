# Week 30 — Layers, Recipes + Custom Image

## Goal

Understand the Yocto layer and recipe system deeply enough to build a fully custom layer
(`meta-avindeli`) from scratch, containing a hand-written recipe for a cross-compiled C
program and a custom image that includes it — then verify the result boots and runs on
real Raspberry Pi 4B hardware.

## Scope

- `BBFILE_PRIORITY` / `LAYERVERSION` and how layer priority resolves recipe/`.bbappend` conflicts
- Full `busybox_*.bb` task chain traced on the real recipe: `do_fetch → do_configure → do_compile → do_install → do_rootfs`
- `DEPENDS` vs `RDEPENDS` — build-host-time vs target-runtime dependency resolution
- `inherit` classes (`cml1`, `cmake`, `autotools`, `pkgconfig`) as reusable cross-compilation boilerplate
- `IMAGE_INSTALL`, `require`, and extending `core-image-minimal` into a custom image
- `bitbake-layers create-layer` / `add-layer` / `remove-layer` / `show-layers` workflow
- Writing a recipe for locally-sourced code (`file://` `SRC_URI`, `S` override, `FILESPATH`/`files/` convention)
- `EXTRA_OEMAKE` for injecting Yocto's cross-toolchain variables into a hand-written Makefile
- Diagnosing and fixing a `do_package_qa` GNU_HASH/LDFLAGS failure
- Raspberry Pi UART serial console debugging (`enable_uart=1` vs `dtparam=uart0=on`)

## Architecture

```
meta-avindeli/                          (sibling to poky/, priority 6)
├── conf/layer.conf                     BBFILES, BBFILE_COLLECTIONS, LAYERDEPENDS=core
├── recipes-hello/
│   └── hello/
│       ├── hello_1.0.bb                custom recipe, LICENSE="CLOSED"
│       └── files/
│           ├── hello.c                 Phase 2 cross-compile source, reused as-is
│           └── Makefile                unmodified — CC/CFLAGS/LDFLAGS injected via EXTRA_OEMAKE
└── recipes-images/
    └── images/
        └── core-image-avindeli.bb      require core-image-minimal.bb
                                         IMAGE_INSTALL:append = " hello"

Task flow (per recipe):
  do_fetch → do_unpack → do_configure → do_compile → do_install (→ ${D}) → do_package → do_package_qa

Image flow:
  do_rootfs (assembles IMAGE_INSTALL package set) → do_image (produces .wic/.ext3/.tar.bz2)
```

Recipe internals for `hello_1.0.bb`:

```
SRC_URI = "file://hello.c file://Makefile"   # found automatically via files/ (FILESPATH)
S       = "${WORKDIR}"                        # no tarball extraction, files land directly in WORKDIR
EXTRA_OEMAKE = "CC='${CC}' CFLAGS='${CFLAGS}' LDFLAGS='${LDFLAGS}'"
do_install() { install -d ${D}${bindir}; install -m 0755 ${S}/hello ${D}${bindir}; }
```

## Key design decisions

**`.bb` vs `.bbappend`.** Derived the rule directly: write a `.bb` when no recipe exists
anywhere in any pulled-in layer (own code); write a `.bbappend` when modifying an
existing recipe (patches, config fragments, version bumps). Never edit upstream layers
(`poky/meta`, `meta-openembedded`) directly — a `git pull` would silently discard local
changes, and the customization wouldn't travel with a shared layer.

**`LICENSE = "CLOSED"` for `hello`.** No formal license applies to personal course code,
so `CLOSED` was used instead of `MIT`/`GPL-2.0-only`. This also means `LIC_FILES_CHKSUM`
is correctly omitted — that variable only exists to checksum-verify a real license file.

**Kept the Makefile's `CC := aarch64-linux-gnu-gcc` line untouched.** Initially assumed
this needed changing to `?=`, but command-line-passed `make` variables (`make CC=...`)
always override in-Makefile assignment regardless of `:=`/`=`/`?=`. The fix belongs
entirely in the recipe (`EXTRA_OEMAKE`), not the Makefile — confirmed via `bitbake -e hello`
showing the real resolved `${CC}`, which carries `--sysroot=`, hardening flags
(`-fstack-protector-strong`, `-D_FORTIFY_SOURCE=2`), and `-mcpu=cortex-a72+crc` — none of
which the hand-typed `aarch64-linux-gnu-gcc` would ever have included.

**`core-image-avindeli.bb` built via `require`, not duplication.** `require
recipes-core/images/core-image-minimal.bb` (resolved relative to `poky/meta`, a registered
`BBFILES` root — a bare filename with no path does *not* resolve) plus
`IMAGE_INSTALL:append = " hello"`, rather than copying core-image-minimal's full package
list. Kept as a real, version-controlled, shareable image recipe rather than a `local.conf`
`IMAGE_INSTALL:append` hack, per the checklist requirement.

**Layer directory location.** `bitbake-layers create-layer` defaulted to creating
`meta-avindeli` *inside* `poky/`. Moved it to be a sibling of `poky/` instead (matching
`meta-raspberrypi`), since `LAYERDIR` is resolved automatically at parse time — no
`layer.conf` edits were needed after the move, just `remove-layer` → `mv` → `add-layer`.

## Lessons Learned

- **Version vs priority conflict resolution operate at different levels.** Different
  versions of the same recipe across layers → highest version wins by default (unless
  `PREFERRED_VERSION` is set). Only when versions are *identical* across layers does
  `BBFILE_PRIORITY` get consulted. For competing `.bbappend` files, priority never excludes
  either one — both always apply; priority only controls *application order*.
- **`DEPENDS` uses `+=`, never `=`.** A plain `busybox.inc`-style `DEPENDS = "..."` would
  silently wipe out the baseline cross-toolchain dependency injected globally via
  `base.bbclass`/`bitbake.conf`, breaking the build.
- **`kern-tools-native` exists because `merge_config.sh` is a Linux kernel script**
  (`scripts/kconfig/merge_config.sh`), repackaged standalone since busybox reuses the
  kernel's entire Kconfig mechanism, not just its style.
- **`DEPENDS` never touches the target device; `RDEPENDS` is checked at `do_rootfs`.**
  `DEPENDS` items (e.g. `kern-tools-native`) exist only on the build host and could be
  deleted post-build with zero effect on the Pi. `RDEPENDS` failures surface when
  `do_rootfs` assembles the final package set, not at compile time.
- **`inherit` classes exist to solve one recurring cross-compilation problem each.**
  `cml1` → Kconfig `.config` handling; `autotools`/`cmake` → auto-inject `--host=`/
  cross-toolchain-file flags; `pkgconfig` → redirects `PKG_CONFIG_SYSROOT_DIR` so
  `pkg-config` doesn't return the build host's native (wrong-architecture) library paths,
  which a cross-linker would then hard-reject at link time (not a silent runtime bug —
  linkers refuse ABI-mismatched objects outright).
- **`do_package_qa` GNU_HASH failure = LDFLAGS never reaching the linker.** The Makefile's
  single combined compile+link line referenced `$(CFLAGS)` but not `$(LDFLAGS)` — Yocto's
  injected `EXTRA_OEMAKE` values are useless if the consuming Makefile rule never reads
  them. Fixed by appending `$(LDFLAGS)` to the link line and adding `LDFLAGS='${LDFLAGS}'`
  to `EXTRA_OEMAKE`. Verified the fix at the binary level with
  `aarch64-linux-gnu-readelf -d ... | grep -i hash`, not just by trusting the QA pass.
- **BitBake almost never fetches or auto-corrects anything on your own behalf.**
  `LAYERDEPENDS` missing → hard parse-time error, not an auto-clone. `LAYERSERIES_COMPAT`
  mismatch → a declared warning/error, not an automatic version downgrade. Good general
  heuristic for reasoning about any unfamiliar `LAYER*`/`BB*` variable.
- **Disk exhaustion mid-build is infrastructure, not a recipe bug.** Found and removed a
  48GB orphaned duplicate build directory (`~/yocto/build`, distinct from the active
  `~/yocto/poky/build`) — confirmed stale via `find -newer` before deleting. `cleanall`
  wasn't actually required to pick up the Makefile/recipe fix; BitBake's task hashing
  normally detects local `file://` and recipe changes automatically — `cleanall` was used
  as a brute-force safety net while debugging, not a routine step.
- **Pi 4B UART requires `enable_uart=1`, not just `dtparam=uart0=on`.** On Bluetooth-equipped
  Pi models, the primary PL011 UART defaults to the Bluetooth module; the GPIO header only
  gets the secondary mini-UART (clock tied to core frequency, prone to drift/silence).
  `enable_uart=1` is the documented setting that forces PL011 onto the GPIO pins. Diagnosed
  by first confirming the serial link itself was good via an RX/TX loopback echo test on
  the adapter alone, isolating the fault to Pi-side UART routing rather than wiring/cable/
  `screen` config.

## Open Items

- **UART fix is currently manual, not reproducible.** `enable_uart=1` was added by hand-
  editing `config.txt` directly on the flashed SD card. Should become a proper `.bbappend`
  to the RPi boot config recipe (or a `meta-avindeli`-owned config fragment) so future
  image builds don't require manually patching the SD card post-flash.
- **`meta-avindeli`'s `BBFILE_PRIORITY = "6"` is the tool's default, unexamined.** Not yet
  a problem since nothing collides with it, but worth revisiting once any `.bbappend`
  ordering relative to `meta-raspberrypi` (priority 9) actually matters.
- **`recipes-hello/hello/files/Makefile` is functionally correct but untouched from Phase
  2 aside from the `$(LDFLAGS)` fix** — fine for a Hello World, but the next real program
  will need to actually exercise `DEPENDS`/`pkgconfig` against a real library to validate
  that part of the theory in practice.