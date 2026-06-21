# Cross-Compiling Against libgpiod v2 — Setup Notes

**Context:** Week 26, GPIO blink exercise (`gpiod_blink/blink_led.c`). Target: Raspberry Pi 4B (Debian 13 "trixie", `aarch64`). Build host: Ubuntu 24.04 VM (`amd64`) with `aarch64-linux-gnu-gcc` cross-compiler.

This document exists because getting from "write blink_led.c" to "binary running on the Pi" took a real detour through multiarch, GPG key verification, and building libgpiod from source. Worth recording *why*, not just *what*, so this doesn't need to be re-derived next time a library version mismatch shows up.

---

## The problem

The Pi runs **Debian 13 (trixie)** with `libgpiod-dev 2.2.1-2+deb13u1` (the v2 API). The VM runs **Ubuntu 24.04 (noble)**. To cross-compile against libgpiod, the VM's cross-compiler needs ARM64 headers and libraries — but:

1. Ubuntu's package mirrors (`archive.ubuntu.com`) don't carry `arm64` binaries at all — ARM architectures live on a separate mirror, `ports.ubuntu.com`.
2. Even after fixing that, **Ubuntu noble's `libgpiod-dev` package is version 1.6.3** — the old v1 API. Debian trixie's is 2.2.1 — the v2 API actually on the Pi. Same package name, different upstream version, because Ubuntu and Debian freeze package versions independently. Cross-compilation solves the *CPU architecture* mismatch; it does **not** solve a *library version* mismatch — that's a separate, "sysroot" problem.

## Resolution path taken

### 1. Enable arm64 as a foreign architecture on the VM

```bash
sudo dpkg --add-architecture arm64
```

### 2. Point arm64 package fetches at the correct mirror

Ubuntu's deb822-format sources (`/etc/apt/sources.list.d/ubuntu.sources`) had no `Architectures:` field, so `apt` was trying (and 404ing) to fetch `arm64` indexes from the `amd64`-only main archive. Fixed by:
- Restricting the existing stanzas to `Architectures: amd64`
- Adding a new stanza for `arm64`, pointed at `http://ports.ubuntu.com/ubuntu-ports/` (note: `ubuntu-ports`, not `ubuntu`)

This got `apt install libgpiod-dev:arm64` working — but only gave us Ubuntu's **v1.6.3**, not the v2.2.1 we needed. Dead end for the actual goal, but the multiarch setup itself is correct and reusable for any future arm64 library.

### 3. Considered: pulling the exact Debian source package instead

Investigated `apt source libgpiod` as a way to get the *exact* `2.2.1-2+deb13u1` source Debian used. This required:
- Adding a `deb-src` entry for Debian trixie (`/etc/apt/sources.list.d/debian-trixie-src.sources`)
- Installing `debian-archive-keyring` to verify signatures — but the Ubuntu-packaged version of this keyring only went up to bookworm (12); no trixie key included.
- Fetching the trixie archive signing key directly from `https://ftp-master.debian.org/keys/archive-key-13.asc` and verifying its fingerprint (`04B5 4C3C DCA7 9751 B16B C6B5 2256 29DF 75B1 88BD`) against the one published on `ftp-master.debian.org/keys.html` before trusting it.

**Decision: didn't pursue this further.** `apt source` + Debian's `debhelper`-based build is real, reusable infrastructure (and now sits ready for future use), but it's a heavier path than necessary just to get one library, and it doesn't transfer to Week 27/28's kernel-module cross-compiling (which needs matching **kernel headers**, not Debian source packages, as the relevant sysroot concern).

### 4. Actual fix: build libgpiod from upstream source, pinned to a close version tag

```bash
git clone https://github.com/brgl/libgpiod.git
cd libgpiod
git checkout v2.2.4   # v2.2.1 isn't tagged upstream; 2.2.4 is the same minor
                       # series — semver guarantees no API changes, only
                       # patch-level fixes, so this is safe for our purposes.

sudo apt install autoconf automake libtool pkg-config build-essential autoconf-archive
./autogen.sh   # generates ./configure from configure.ac (needs autoconf-archive
               # for the AX_* macros used in configure.ac)

./configure --host=aarch64-linux-gnu --prefix=$HOME/embedded/week26/libgpiod-cross/install
# --host tells autotools to cross-compile FOR aarch64 (the Pi), while building
# ON amd64 (the VM). Autotools auto-discovers aarch64-linux-gnu-gcc from the
# --host triplet — no need to set CC= explicitly.
# --prefix isolates the output in a scratch dir, well away from the VM's own
# native /usr/local, so amd64 and arm64 builds never get mixed on disk.

make -j$(nproc)
make install
```

Output landed in `~/embedded/week26/libgpiod-cross/install/{include,lib}/`.

### 5. Static linking, deliberately

The Pi already has its own `libgpiod.so` (2.2.1) installed system-wide. Dynamically linking the blink binary would mean it resolves against *whatever* `libgpiod.so` the Pi finds at runtime — not necessarily the one it was built/tested against. Statically linking (`-static -lgpiod`, using `libgpiod.a` from the install tree) avoids that ambiguity entirely: the binary is fully self-contained, verified with:

```bash
file blink_led                              # confirms ARM aarch64 ELF
aarch64-linux-gnu-readelf -d blink_led | grep NEEDED   # confirms zero shared deps
```

### Final compile command

```bash
aarch64-linux-gnu-gcc -Wall -o blink_led blink_led.c \
  -I$HOME/GitHub/Phase_2/week26/libgpiod-cross/install/include \
  -L$HOME/GitHub/Phase_2/week26/libgpiod-cross/install/lib \
  -static -lgpiod
```

---

## Key lessons

- **Cross-compiling ≠ matching library versions.** A cross-compiler solves the CPU instruction-set problem only. The *sysroot* (headers/libs for the target) is a separate concern you have to manage yourself — apt, multiarch, or building from source are all valid approaches depending on the situation.
- **Same package name, different distro, different version.** Ubuntu and Debian both ship `libgpiod-dev` but freeze it at whatever was current for their own release schedule. Never assume parity just because the package name matches.
- **`apt source` only downloads/unpacks into the CWD — it never touches `dpkg`'s installed-package state.** Safe to experiment with, unlike installing foreign binary packages, which *can* create dependency conflicts.
- **GPG fingerprint verification matters and is cheap.** Caught a citation error during this session (comparing against the wrong Debian key — release key vs. archive signing key) precisely because the verification step was actually performed rather than skipped.
- **Build artifacts (cloned source trees, compiled `.a`/`.so`, binaries) don't belong in git.** `libgpiod-cross/`, `*.o`, and the `blink_led` binary itself are excluded via `.gitignore` in this directory — only `blink_led.c` and this README are tracked.

## Reproducing this setup elsewhere

If `~/embedded/week26/libgpiod-cross` is ever deleted, steps 4–5 above are sufficient to reproduce the exact toolchain state — steps 1–3 (multiarch, deb-src, keyring) are **not required** for this specific outcome; they're documented above because they were explored, but the working solution doesn't depend on them. They remain useful general-purpose VM setup for any future arm64 package needs.