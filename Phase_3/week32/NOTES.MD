# NOTES.md — Yocto block retrospective (Weeks 29–32)

## What the block was

Four weeks, Poky Scarthgap 5.0.18, Raspberry Pi 4B. From a first stock build (W29) to a
custom layer, image, kernel module recipes and Device Tree overlay (W30–31), to an SDK and
a tagged, reproducible repo (W32). Milestones M16, M17, M18.

## What actually changed in how I think

**Yocto is a build system, not a distribution.** I came in expecting to learn a Linux
distro and instead learned a dependency graph with a content-addressed cache bolted to it.
Nearly every behaviour that surprised me — why a rebuild was instant, why deleting `tmp/`
was safe, why removing a redundant `IMAGE_FEATURES` line produced 3840 of 3840 cached
tasks — follows from sstate hashing. Once I started predicting behaviour from "what does
this change do to the task hashes", Yocto stopped being arbitrary.

**The manual weeks were the prerequisite, not the warm-up.** `devtool modify` generates a
`.bbappend` using `externalsrc`. I could read that file and recognise it only because I
hand-wrote a `.bbappend` in Week 31. Encountering `devtool` in Week 29 would have been
magic — a command that does something opaque and works. Encountering it in Week 32 it was
just a script doing something I already knew how to do slowly.

The same is true of the SDK. `$CC` carrying `--sysroot` and the hardening flags only means
something because Week 25's toolchain *didn't*, and I lost real time to libgpiod version
skew and a GNU_HASH QA rejection as a direct result. The SDK isn't impressive in the
abstract; it's impressive as the fix to problems I'd already had.

**Cross-compilation is a knowledge problem, not a compiler problem.** The compiler was
never the hard part. What the compiler *knows about the target* is the hard part, and a
sysroot is that knowledge in file form.

## Where the time actually went

Not where I expected. The conceptual material — layers, recipes, tasks, classes, overrides
— was maybe a third of it. The rest was environment:

- disk exhaustion, twice (106 GB is not enough for this workflow)
- an SSH disconnect killing a long build, because I wasn't using `tmux`
- a pseudo abort from a stale workdir after a kernel rebuild
- shell contamination from sourcing an SDK env and `oe-init-build-env` together
- a stale `known_hosts` entry after reflashing, which broke `devtool deploy-target` with a
  bare `Connection closed` and no host-key warning

None of that is Yocto knowledge. All of it is the actual experience of using Yocto, and I
suspect that ratio doesn't improve much with expertise — it just gets diagnosed faster.
The lesson I want to keep is the diagnostic habit rather than the individual fixes: test
outside the tool first. Plain `ssh`, plain `scp`, `scp -O` settled in three commands what
I'd been about to fix by rebuilding an image on a wrong hypothesis.

## The mistake worth remembering

I kept `meta-avindeli` outside the git repo and copied it back and forth, to keep build
artifacts out of GitHub. A Yocto layer contains no build artifacts — everything generated
lives in `build/tmp` and `build/sstate-cache`, outside the layer. So the reasoning was
wrong from the start, and the workflow had already silently lost two weeks of recipes:
`diff -rq` against the Week 30 snapshot showed `recipes-kernel/`, `recipes-mydev/` and the
machine config existing only in the working copy.

What makes this worth writing down isn't the specific error. It's that a workflow can be
lossy with no error message at all. I only found it because tagging forced me to ask what
`meta-avindeli = "<unknown>:<unknown>"` meant. If I hadn't cared about provenance, I'd
have shipped a tag that didn't build.

## Honest self-assessment

I can write recipes, `.bbappend`s, a machine config, an image recipe, and a kernel module
recipe via `module.bbclass`. I can read a task graph and a QA failure. I can generate and
use both SDKs.

I do **not** yet understand: `bbclass` internals beyond using them, variable flags and the
override syntax in any depth, `SRC_URI` fetchers beyond `file://` and git, multiconfig, or
what I'd do if the BSP layer didn't already work. Every kernel change I made went through
`meta-raspberrypi` doing the hard part.

## Open threads

- `chardev.c` still lacks `class_create()` / `device_create()` — the device node is not
  created properly by devtmpfs.
- LED wiring via `libgpiod` never physically verified; the BCM21 overlay pin muxing is
  untested against hardware.
- VM disk needs resizing rather than periodic `rm -rf tmp`.
- The eSDK at `~/poky_sdk` references the old layer path and needs regenerating.
- `BB_HASHSERVE` / `SSTATE_MIRRORS` interaction in the eSDK — understood as a warning,
  not as a mechanism.

