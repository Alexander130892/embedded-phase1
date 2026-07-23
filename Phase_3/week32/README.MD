# Week 32 — Yocto Deliverable + SDK

> Phase 3, Weeks 29–32 (Yocto block). Milestone **M18**: custom Yocto image + SDK generated.

## Goal

Close out the Yocto block by producing the two artifacts that make a custom distribution
usable by someone other than its author: a **cross-development SDK** built from the image
itself, and a **reproducible build** whose provenance can be traced from a git tag.

The week's real subject is not "how do I run `populate_sdk`" — it is *where does a
cross-toolchain get its knowledge of the target from, and what does it cost to iterate
once it has it.* Weeks 25–28 answered that question the hard way. This week replaced the
hard way with two supported ones and measured the difference.

---

## Scope

- Yocto plain SDK (`populate_sdk`) — structure, installation, verification
- Yocto extensible SDK (`populate_sdk_ext`) — what it adds and why
- `devtool add` / `modify` / `build` / `deploy-target` / `reset` workflow
- `externalsrc` as the mechanism behind `devtool modify`
- `IMAGE_INSTALL` vs `IMAGE_FEATURES` — packages vs features
- `debug-tweaks` and the development/production split
- Build provenance: layer revisions, tracked configuration, reproducibility
- sstate-cache behaviour under partial rebuilds and workdir removal

---

## Host setup

| Item | Value |
|---|---|
| Build host | Ubuntu 24.04.4 LTS VM on Proxmox (`192.168.0.111`) |
| Root filesystem | 106 GB LVM (`ubuntu--vg-ubuntu--lv`) — **see Open Items** |
| Poky / Yocto | Scarthgap **5.0.18**, BitBake 2.8.1 |
| `MACHINE` | `raspberrypi4-64-avindeli` |
| `DISTRO` | `poky` |
| Tune | `aarch64 crc cortexa72` |
| Target | Raspberry Pi 4B (64-bit), `192.168.0.248` |
| Flashing host | MacBook Air (`dd` to `/dev/rdiskN`) |

Host packages are the standard Yocto build-essentials set for Ubuntu (`gawk wget git
diffstat unzip texinfo gcc build-essential chrpath socat cpio python3 python3-pip
python3-pexpect xz-utils debianutils iputils-ping python3-git python3-jinja2 zstd
liblz4-tool file locales libacl1`) — verify against the Yocto Project Quick Build guide
for the release in use, as the list changes between releases.

### Layer stack

```
meta                   scarthgap:3482e7f3   (poky core)
meta-poky              scarthgap:3482e7f3
meta-yocto-bsp         scarthgap:3482e7f3
meta-raspberrypi       scarthgap:167703d5   (BSP)
meta-avindeli          main:8c19927f        (this repo)
```

All five layers report a branch and commit. That was not true at the start of the week —
see *Build provenance* below.

---

## Layer structure — `meta-avindeli`

```
meta-avindeli/
├── COPYING.MIT
├── conf/
│   ├── layer.conf
│   └── machine/
│       └── raspberrypi4-64-avindeli.conf     MACHINE definition; ENABLE_UART = "1"
├── recipes-example/
│   └── example/example_0.1.bb                 boilerplate from create-layer (unused)
├── recipes-hello/
│   └── hello/
│       ├── hello_1.0.bb                       trivial C app, in-tree Makefile build
│       └── files/hello.c
├── recipes-images/
│   └── images/core-image-avindeli.bb          the image recipe
├── recipes-kernel/
│   ├── chardev/
│   │   ├── chardev_1.0.bb                     out-of-tree module via module.bbclass
│   │   └── files/chardev.c
│   └── linux/
│       ├── linux-raspberrypi_%.bbappend       overlay wiring
│       └── files/mydev-overlay.dts            Device Tree overlay
└── recipes-mydev/
    └── mydev-app/
        ├── mydev-app_1.0.bb
        └── files/
            ├── user_program.c                 userspace reader for /dev/mydev
            └── mydev-app.init                 sysvinit service
```

The machine config carries `ENABLE_UART = "1"`, which is the correct home for it — UART is
a property of the machine, not of an image or a BSP recipe. Anyone selecting this machine
gets serial console without a post-flash edit to `config.txt`.

---

## Build instructions

```bash
# 1. Fetch poky and the BSP layer at the matching release
git clone -b scarthgap git://git.yoctoproject.org/poky
git clone -b scarthgap git://git.yoctoproject.org/meta-raspberrypi

# 2. Clone this repo; the layer lives at Phase_3/week32/meta-avindeli

# 3. Initialise the build environment
cd poky
source oe-init-build-env

# 4. Install the sample configuration and edit absolute paths
cp <repo>/Phase_3/week32/conf-samples/local.conf.sample    conf/local.conf
cp <repo>/Phase_3/week32/conf-samples/bblayers.conf.sample conf/bblayers.conf
#    bblayers.conf contains absolute paths — these MUST be edited.
#    local.conf sets BB_NUMBER_THREADS/PARALLEL_MAKE to 4; tune to your core count.

# 5. Build
bitbake core-image-avindeli
```

Output lands in `tmp/deploy/images/raspberrypi4-64-avindeli/`. Image artifacts are written
with a timestamp and a stable symlink alongside:

```
core-image-avindeli-raspberrypi4-64-avindeli.rootfs-20260722185800.wic.bz2
core-image-avindeli-raspberrypi4-64-avindeli.rootfs.wic.bz2 -> (the above)
```

Always reference the symlink in scripts; always check the *date* before flashing manually.

Verify image contents against the manifest rather than trusting a successful build:

```bash
grep -E 'openssh-sftp-server|strace' tmp/deploy/images/*/[*].rootfs.manifest
```

### Configuration split — development vs production

`debug-tweaks` (empty root password, relaxed auth) is supplied by
`EXTRA_IMAGE_FEATURES ?= "debug-tweaks"` in **`local.conf`**, *not* by the image recipe.
This is deliberate:

- `local.conf` is per-developer and untracked → the flag is a local choice
- the image recipe is tracked and shipped with the tag → it must not hardcode a
  passwordless root

`local.conf.sample` in this repo **does** enable `debug-tweaks`. Following the build
instructions above verbatim therefore produces a *development* image with passwordless
root over SSH. That is correct for this repo's purpose. It is not a production
configuration, and the flag is one line to remove.

---

## SDK generation and usage

### Plain SDK

```bash
bitbake -c populate_sdk core-image-avindeli
./tmp/deploy/sdk/poky-glibc-x86_64-core-image-avindeli-cortexa72-\
raspberrypi4-64-avindeli-toolchain-5.0.18.sh        # 176 MB → /opt/poky/5.0.18
```

Usage, in a **clean shell**:

```bash
. /opt/poky/5.0.18/environment-setup-cortexa72-poky-linux
$CC hello.c -o hello
```

The installer name is self-documenting: `x86_64` is the host it runs on,
`core-image-avindeli` is the image it is scoped to, `cortexa72` /
`raspberrypi4-64-avindeli` is the target, `5.0.18` is the Poky release whose glibc is in
the sysroot.

Alongside it, `populate_sdk` emits `.host.manifest` and `.target.manifest` (exact package
lists per sysroot), `.testdata.json`, and SPDX SBOM tarballs.

### What the environment script actually sets

```
$CC = aarch64-poky-linux-gcc
      -mcpu=cortex-a72+crc
      -mbranch-protection=standard
      -fstack-protector-strong -O2 -D_FORTIFY_SOURCE=2
      -Wformat -Wformat-security -Werror=format-security
      --sysroot=/opt/poky/5.0.18/sysroots/cortexa72-poky-linux
```

`$CC` is not a compiler. It is a compiler **plus the distribution's policy for this
machine** — the exact tuning, the hardening set, and the sysroot. Everything that had to
be remembered and passed by hand in Week 25 is here, unforgettable by construction.

Verification:

```bash
$CC /tmp/t.c -o /tmp/t
file /tmp/t                      # ELF 64-bit LSB pie executable, ARM aarch64
                                 # interpreter /lib/ld-linux-aarch64.so.1
$CC -print-sysroot               # /opt/poky/5.0.18/sysroots/cortexa72-poky-linux
aarch64-poky-linux-readelf -d /tmp/t | grep NEEDED    # libc.so.6
```

### Extensible SDK

```bash
bitbake -c populate_sdk_ext core-image-avindeli
./tmp/deploy/sdk/...-toolchain-ext-5.0.18.sh         # 1.9 GB → ~/poky_sdk
. ~/poky_sdk/environment-setup-cortexa72-poky-linux
```

The 176 MB → 1.9 GB jump *is* the difference: the eSDK ships BitBake, poky, the layer
stack, a real `conf/`, and a seeded sstate-cache. `devtool build` is a front-end onto a
genuine build system running inside the SDK, not a wrapper around `$CC`.

### devtool workflow

```bash
devtool modify hello                              # extract source + generate bbappend
$EDITOR ~/poky_sdk/workspace/sources/hello/hello.c
devtool build hello                               # 765 tasks, 756 cached, 9 executed
devtool deploy-target hello root@192.168.0.248    # scp onto the running target
devtool undeploy-target hello root@192.168.0.248  # restore the image's version
devtool reset hello                               # discard workspace, recipe untouched
```

The generated `workspace/appends/hello_1.0.bbappend`:

```
inherit externalsrc
EXTERNALSRC:pn-hello       = ".../workspace/sources/hello"
EXTERNALSRC_BUILD:pn-hello = ".../workspace/sources/hello"
# initial_rev: d431621f...
```

`deploy-target` records what it wrote in `/.devtool/` on the target
(`hello.list`, `hello.preserve`) so the deployment is a **tracked, reversible overlay** on
the flashed image — not an untraceable `scp` dump.

---

## Comparison: three ways to build for the target

| | **Week 25 — manual cross-compile** | **Yocto plain SDK** | **Yocto eSDK + devtool** | **Full image build** |
|---|---|---|---|---|
| Toolchain | Ubuntu `aarch64-linux-gnu-gcc` | `aarch64-poky-linux-gcc` | same, plus BitBake | same |
| Target headers | Debian's `libc6-dev-arm64-cross` | sysroot built **from the image** | regenerable sysroot | n/a — built in place |
| Target libraries | whatever `-L` pointed at | same sysroot | same | n/a |
| Version match | **not guaranteed** | guaranteed by construction | guaranteed | guaranteed |
| ABI / hardening flags | must be known and passed by hand | baked into `$CC` | baked into `$CC` | applied by the recipe |
| Adding a new library | build it from source yourself | must be in the image, or extend `TOOLCHAIN_TARGET_TASK` | `devtool` can build it | add to `IMAGE_INSTALL` |
| Edit → running on target | compile, `rsync`, run | compile, copy, run | `devtool build` + `deploy-target` | rebuild + **reflash SD card** |
| Cost of one iteration | seconds, but correctness unverified | seconds | ~9 tasks, seconds | ~5800 tasks + physical access |
| Size on disk | ~100 MB (distro packages) | 176 MB | 1.9 GB installer | — |
| Known failure modes | version skew, wrong hash style, silent ABI mismatch | sysroot scoped to image | large; hashserve/mirror friction | slow; requires the card |


**Scoping rule worth remembering:** the SDK's target sysroot is a function of the *image*
(`TOOLCHAIN_TARGET_TASK` derives `-dev` packages from the image's runtime dependencies).
An SDK built from `core-image-minimal` would compile `hello` fine but would lack, say,
`libgpiod` headers. The fix is to add the library to the image, or override explicitly:

```
TOOLCHAIN_TARGET_TASK:append = " libgpiod-dev"
```

---

## Build provenance

At the start of the week every build printed:

```
meta-avindeli = "<unknown>:<unknown>"
```

The layer had been living at `~/yocto/meta-avindeli` and being hand-copied to and from the
repo, on the theory that keeping it out of git kept build artifacts out of git. That theory
was wrong: **a Yocto layer contains no build artifacts.** Everything generated lives in
`build/tmp/` and `build/sstate-cache/`, outside the layer entirely.

The copy workflow had already failed silently. A `diff -rq` between the live layer and the
Week 30 snapshot showed `recipes-kernel/`, `recipes-mydev/`, and `conf/machine/` existing
only in the live tree — two weeks of work that had never been copied back. There is no
error message for this; it is discovered later, or not at all.

Fix: the layer now lives at `Phase_3/week32/meta-avindeli` and `bblayers.conf` points at
it there. BitBake reports `main:8c19927f` and the tag corresponds to what actually built.
`local.conf` and `bblayers.conf` are captured as `.sample` files, since a build is not
reproducible from recipes alone.

---

## Key design decisions

**Layer inside the repo, single canonical copy.** The alternative — the copy-back
workflow — has no failure signal and had already lost two weeks of recipes. One location,
referenced by absolute path from `bblayers.conf`, removes the class of error entirely.

**`debug-tweaks` in `local.conf`, not in the image recipe.** Both were set; removing the
recipe's `IMAGE_FEATURES:append` produced a build where **3840 of 3840 tasks were cached**
— proof the append was redundant, since the final `IMAGE_FEATURES` value and therefore
every task hash was unchanged. Keeping the flag only in untracked local config expresses
the dev/production split structurally rather than as a warning comment.

**`ENABLE_UART` in the machine config.** Cleaner than an `rpi-config` `.bbappend`: serial
console is a machine property, so it belongs to the machine definition and applies to any
image built for it.

**Plain SDK before eSDK; both after the manual weeks.** Encountering `devtool` in Week 30
would have made it magic. Having hand-written a `.bbappend` in Week 31, the generated
`externalsrc` append in Week 32 was recognisable rather than mysterious.

---

## Lessons Learned

**Toolchains**

- A cross-compiler needs three things, not one: the compiler binary, a **sysroot**
  (the target's headers *and* libraries arranged as `/`), and an environment script
  encoding *how to invoke the compiler for this machine*. Week 25 had the first, an
  approximation of the second, and none of the third.
- A sysroot is not a rootfs. The rootfs boots; the sysroot has headers and unstripped
  libraries the running image does not need. `populate_sdk` is a separate task from the
  image build because the content differs.
- The SDK ships **two** sysroots — `cortexa72-poky-linux` for the target and
  `x86_64-pokysdk-linux` for the host, because Poky does not trust the host distro's
  `gcc`, `cmake`, or `pkg-config` either.

**BitBake mechanics**

- `IMAGE_INSTALL` takes *packages*; `IMAGE_FEATURES` takes *features*. Putting
  `debug-tweaks` in the former yields `Nothing RPROVIDES 'debug-tweaks'` — BitBake is
  hunting for a package by that name.
- `:append` concatenates literally. The leading space inside the quotes is not optional.
- `tainted from a forced run` means a task was previously run with `-f`, so its output is
  not derived purely from its inputs. Before tagging, `bitbake -c clean <recipe>` and
  rebuild — if it still builds, the tag is reproducible; if it needs the force, that is a
  bug worth finding before the tag, not after.
- Cleaning the kernel does not clean its dependents' workdirs. A stale `chardev` workdir
  plus a pseudo database pointing at inodes that no longer existed produced
  `do_package failed with exit code '134'` (SIGABRT) and a wall of `inode mismatch` lines.
  `bitbake -c clean chardev` resets the workdir and the pseudo db; `cleansstate` if that
  is not enough.
- `build/tmp/` is disposable. With `sstate-cache/` and `downloads/` intact, deleting it
  reclaimed 72 GB and the next build refilled it from cache. Never delete the other two.
- BitBake's disk monitor (`STOPTASKS`) refuses to start new tasks near zero free space and
  lets running ones finish. It parked the build gracefully instead of corrupting it.

**Shell hygiene**

- Sourcing an SDK environment script and then `oe-init-build-env` in the same shell earns
  *"The OE SDK/ADT was detected as already being present in this shell environment"* —
  and BitBake proceeds anyway with a polluted `CC`/`CFLAGS`/`PATH`. Use separate shells.
- `devtool` run from the wrong shell parses the *main* build (`Loaded 0 entries from
  dependency cache`, 966 recipes parsed) instead of the eSDK's, and reports
  `No files to deploy` because the workspace it needs does not exist there. The
  cache-entry count in the output identifies which BitBake is running.
- Long builds belong in `tmux`. One dropped SSH session cost a `populate_sdk` run before
  this was adopted.

**Deployment**

- `scp` defaults to the SFTP protocol, which requires a server-side binary. A minimal
  Yocto image has none — hence `sh: /usr/libexec/sftp-server: not found`. The week's
  `openssh-sftp-server` task exists precisely because of this.
- Reflashing regenerates the target's SSH host keys. A stale `known_hosts` entry then
  breaks `devtool deploy-target` with a bare `scp: Connection closed` and **no host-key
  warning**, because the non-interactive scp has no way to prompt. `ssh-keygen -R <ip>`,
  then connect once interactively to accept the new key.
- Diagnose outside the tool before changing the target: plain `ssh`, plain `scp`, and
  `scp -O` (legacy protocol) isolate host-key vs sftp-subsystem vs network in three
  commands. This session's `dropbear`-vs-OpenSSH hypothesis was wrong, and testing
  established that before an unnecessary image rebuild.
- A passing test that proves nothing is worse than a failing one: `hello` printing the
  original string after a reflash showed only that the SD card image worked. Verifying
  `deploy-target` required a string that exists nowhere in the flashed image.
- Keep one image file per build in the flashing directory. Four `.wic` files across three
  dates nearly resulted in flashing a week-old image and debugging a "missing" package
  that was present in the build all along.

---

## Open Items

- **Tag `v1.0-phase3-yocto`** and write the Yocto retrospective in `NOTES.md`.
- **Resize the VM disk.** 106 GB filled to 100% during `populate_sdk_ext`. Deleting
  `build/tmp/` bought headroom but the constraint is structural: Proxmox disk resize →
  `growpart` → `lvextend` → `resize2fs`.
- **`devtool undeploy-target hello`** — the Pi still carries a devtool-deployed binary
  that no longer corresponds to any recipe.
- **Remove `recipes-example/example_0.1.bb`** — `create-layer` boilerplate, never used.
- **`BB_HASHSERVE` vs `SSTATE_MIRRORS` warning** in the eSDK: the shipped configuration
  enables hash equivalence *and* an sstate mirror, which can prevent mirror artifacts from
  matching. Harmless for `hello`; worth understanding if `devtool` starts rebuilding more
  than it should.
- **The eSDK at `~/poky_sdk` references the old layer path** (`~/yocto/meta-avindeli`,
  now deleted). It will need regenerating before further `devtool` work.
- **`chardev.c`** still needs `class_create()` / `device_create()` for proper devtmpfs
  node creation — carried over from Week 31.
- **Physical LED wiring** via `libgpiod` to verify BCM21 overlay pin muxing — carried over
  from Week 31.