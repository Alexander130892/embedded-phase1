# Week 29 — Yocto Concepts + First Build

## Goal

Build a foundational understanding of the Yocto Project build system and produce a bootable
`core-image-minimal` image for the Raspberry Pi 4B from scratch. The image must boot to a login
prompt and accept SSH connections over ethernet.

**M16 checkpoint:** Yocto `core-image-minimal` boots on Pi 4B over SSH. ✅

## Scope

- Yocto Project conceptual model: layers, recipes, machine config, image targets
- `bitbake` workflow: dependency graph resolution, task execution, sstate-cache
- Build host setup: Ubuntu 24.04, LVM disk resize, Yocto dependencies
- Cloning `poky` (Scarthgap) and `meta-raspberrypi`
- Configuring `local.conf` and `bblayers.conf`
- Cold build of `core-image-minimal` (~5 hrs)
- Adding `dropbear` via `IMAGE_INSTALL:append` and incremental rebuild (~10 min)
- First-boot bring-up: serial console debugging, DHCP, SSH verification
- Exploring `tmp/deploy/images/` and `tmp/work/` build artefacts

## Yocto Conceptual Model

```
┌─────────────────────────────────────────────────────┐
│                   Yocto Project                     │
│         (umbrella: standards, releases, docs)       │
└────────────────────┬────────────────────────────────┘
                     │
              ┌──────▼──────┐
              │    poky      │  ← reference distro (like Ubuntu)
              │  (Scarthgap) │
              └──────┬───────┘
                     │
              ┌──────▼───────┐
              │   bitbake    │  ← build engine (like apt/make)
              └──────┬───────┘
                     │
       ┌─────────────┼──────────────┐
       ▼             ▼              ▼
┌────────────┐ ┌──────────────┐ ┌───────────────────┐
│    meta    │ │meta-raspberrypi│ │  meta-mycompany  │
│ (poky core)│ │  (BSP layer) │ │  (custom layer)  │
└────────────┘ └──────────────┘ └───────────────────┘
```

### The four core concepts

| Concept | Purpose | Owned by |
|---|---|---|
| **Layer** | Separates concerns by ownership boundary — enables reuse and independent upgrades | Yocto Project / BSP vendor / you |
| **Recipe (`.bb`)** | Declarative description of one component: fetch → patch → compile → install | Layer maintainer |
| **Machine (`MACHINE`)** | Hardware target — CPU arch, BSP, device tree | BSP layer |
| **Image** | Rootfs contents — which packages end up on the target | You / distro policy |

Layers are not a build dependency graph — they are a **priority-ordered list** of recipe search
paths. Higher priority wins when two layers provide the same recipe. `meta-raspberrypi` at
priority 9 overrides poky layers at priority 5.

### bitbake workflow

```
bblayers.conf  →  bitbake knows where to look (which layers)
local.conf     →  bitbake knows what to build (MACHINE, DISTRO, IMAGE)
recipes (.bb)  →  bitbake knows how to build each component
dependency graph → bitbake resolves task execution order + parallelism
sstate-cache   →  bitbake skips tasks already completed (task-level cache)
```

The sstate-cache operates at **task** granularity, not recipe granularity. Changing one recipe
only invalidates downstream tasks — everything else is pulled from cache. This is why the second
build (after adding dropbear) took 10 minutes vs 5+ hours for the cold build.

## Build host configuration

**VM specs:** Ubuntu 24.04, 4 vCPUs, 8 GB RAM (bumped from 4 GB), 110 GB disk (bumped from 80 GB)

LVM resize procedure (no reinstall required):
```bash
sudo growpart /dev/vda 3
sudo pvresize /dev/vda3
sudo lvextend -l +100%FREE /dev/ubuntu-vg/ubuntu-lv
sudo resize2fs /dev/ubuntu-vg/ubuntu-lv
```

AppArmor fix required for Ubuntu 24.04 (unprivileged user namespace restriction):
```bash
sudo sysctl -w kernel.apparmor_restrict_unprivileged_userns=0
echo 'kernel.apparmor_restrict_unprivileged_userns=0' | sudo tee /etc/sysctl.d/99-userns.conf
```

## Configuration files

**`conf/bblayers.conf`** — layers included in the build:
```
/home/avindeli/yocto/poky/meta           (priority 5)
/home/avindeli/yocto/poky/meta-poky      (priority 5)
/home/avindeli/yocto/poky/meta-yocto-bsp (priority 5)
/home/avindeli/yocto/meta-raspberrypi    (priority 9)
```

**`conf/local.conf`** — key settings:
```bitbake
MACHINE = "raspberrypi4-64"
DISTRO ?= "poky"
BB_NUMBER_THREADS = "4"
PARALLEL_MAKE = "-j4"
IMAGE_INSTALL:append = " dropbear"
```

`BB_NUMBER_THREADS` controls how many bitbake tasks run in parallel (Yocto level).
`PARALLEL_MAKE` controls how many make jobs run within a single recipe (`-j4` passed to `make`).

## Build artefacts

Located in `tmp/deploy/images/raspberrypi4-64/`:

| Artefact | File | Notes |
|---|---|---|
| Kernel | `Image` | Raw ARM64 binary, no ELF headers |
| DTB | `bcm2711-rpi-4-b.dtb` | BCM2711 = Pi 4B SoC |
| Full disk image | `core-image-minimal-raspberrypi4-64.rootfs.wic.bz2` | Partition table + boot + rootfs |
| Block map | `core-image-minimal-raspberrypi4-64.rootfs.wic.bmap` | Speeds up `bmaptool` flash |
| Bootloader | `bootfiles/bootcode.bin` + `start4.elf` | Proprietary Broadcom GPU blobs |
| Rootfs tarball | `core-image-minimal-raspberrypi4-64.rootfs.tar.bz2` | Inspection only, not for flashing |

The Pi's boot chain starts with proprietary GPU firmware blobs — there is no u-boot in the
default `meta-raspberrypi` configuration.

## tmp/work/ structure

```
tmp/work/
├── x86_64-linux/              # Native tools (run on build host)
├── cortexa72-poky-linux/      # Target packages (generic ARMv8 Cortex-A72)
├── raspberrypi4_64-poky-linux/ # Machine-specific BSP packages
└── all-poky-linux/            # Architecture-independent packages
```

Each recipe work directory (e.g. `cortexa72-poky-linux/dropbear/2022.83/`) contains:
- `dropbear-2022.83/` — unpacked upstream source
- `*.patch` files — CVE fixes and feature patches applied by `do_patch`
- `build/` — out-of-tree compiled artefacts
- `recipe-sysroot/` — isolated dependency sysroot for this recipe
- `image/` — staged install tree (what goes into rootfs)
- `packages-split/` — split into sub-packages (dropbear, dropbear-dbg, dropbear-dev)
- `temp/` — per-task logs (`log.do_compile`, `log.do_fetch`, etc.) — first stop for debugging

The cross-compiler invoked is `aarch64-poky-linux-gcc` — the same toolchain used manually
in Phase 2, now orchestrated automatically by bitbake across all 961 recipes.

## First-boot bring-up issues

| Issue | Symptom | Fix |
|---|---|---|
| AppArmor blocking user namespaces | `ERROR: User namespaces are not usable by BitBake` | `sysctl kernel.apparmor_restrict_unprivileged_userns=0` |
| No SSH server in `core-image-minimal` | SSH hangs, nothing in manifest | `IMAGE_INSTALL:append = " dropbear"` in `local.conf` |
| No UART console output | Serial adapter connected but silent | Add `console=serial0,115200` to `cmdline.txt`, `enable_uart=1` to `config.txt` |
| No IP address after late cable plug-in | `udhcpc` gave up before cable connected | Run `udhcpc -i eth0` manually to renew lease |

## Lessons Learned

- **Yocto ≠ poky ≠ bitbake**: Yocto is the umbrella project, poky is the reference distro,
  bitbake is the build engine. Analogous to: Linux project / Ubuntu / apt.
- **Layers are ownership boundaries, not a dependency graph**: they define where bitbake looks
  for recipes and which version wins on conflict, not build order.
- **Recipe tasks are discrete and cacheable**: `do_fetch`, `do_patch`, `do_configure`,
  `do_compile`, `do_install`, `do_package` — each independently cached in sstate.
- **`IMAGE_INSTALL:append` requires a leading space**: `:append` is literal string
  concatenation, so `" dropbear"` not `"dropbear"`.
- **`source oe-init-build-env` must be re-run every session**: it sets `PATH` and environment
  variables in the current shell only — not persistent across SSH sessions.
- **`core-image-minimal` is genuinely minimal**: no SSH, no WiFi, no password — deliberate.
  Add everything explicitly.
- **`tmp/work/` is your debugging surface**: recipe logs, sysroots, staged artefacts, and
  applied patches are all inspectable on disk.
- **80 GB disk is too tight for Yocto**: cold build consumed ~40 GB in `tmp/` alone. 110 GB
  recommended minimum.

## Open Items

- WiFi support not included — `core-image-minimal` lacks `wpa_supplicant` and WiFi firmware;
  would require `IMAGE_INSTALL:append` additions and a rebuild for wireless connectivity.
- Root password not set — production images should configure this via `extrausers` or a
  custom `passwd` file in a `meta-mycompany` layer.
- Weeks 30–32: custom layers, custom recipes, `devtool`, kernel configuration with `menuconfig`