# Phase 2 Retrospective

## What clicked

Direct register writes were the most valuable part of Phase 2. Working at that level forces a real understanding of the underlying protocols — you can't fake it. If something doesn't work, the answer is in the reference manual or datasheet, and that's a clear, single source of truth. There's a satisfaction to it too: when a BMP280 WHO_AM_I comes back `0x58` over SPI you built from scratch, you know exactly why it worked.

The performance clarity was also real — no HAL overhead, no abstraction tax. What you write is what runs.

## What was harder than expected

Hardware debugging at the register level is brutal in a way that's hard to anticipate. A single bit not set in a control register can silently break everything, and there's no stack trace. The debugging loop is: read the manual again, re-read the manual again, find the bit you missed.

The embedded Linux block was harder in a different way. The difficulty isn't low-level complexity — it's the opposite. Linux is high-level, but you need to already know that high-level functions exist before you can use them. There's no single reference manual. Instead there's a sprawling ecosystem: kernel docs, man pages, Stack Overflow, source code, all at different levels of reliability. The level boundaries (kernel space vs userspace vs shell) compound this — the same task can be approached from three different layers and it's not always obvious which one is correct.

The embedded Linux section sometimes felt like *trust me, run this command* rather than *here's why this works*. That's a gap worth closing in Phase 3.

## How FreeRTOS changed how I think about sequencing work

Before FreeRTOS, the mental model was a superloop — do this, then that, check this flag, repeat. It works, but everything is entangled.

The intermezzo cooperative scheduler on Arduino made the first crack in that model: even without preemption, splitting work into discrete units with defined timing responsibilities clarified the code immediately. FreeRTOS extended that further — tasks, queues, and timers force a clear split of responsibility that naturally maps to file structure. Each task owns its domain, communicates through defined channels, and doesn't reach into another task's state.

That discipline carries beyond embedded. It's closer to how good software is structured generally: bounded responsibilities, explicit interfaces between components, timing made visible rather than implicit.

## First impressions: kernel space vs userspace vs bare-metal

| | Bare-metal STM32 | Linux userspace | Linux kernel space |
|---|---|---|---|
| **Feel** | Full control, full responsibility | Familiar C, OS handles the hard parts | Bare-metal rules, but with kernel APIs |
| **Debugging** | Logic analyser | `printf`, `gdb` | `printk` |
| **Reference** | One datasheet, one ref manual | Man pages + scattered docs | Kernel docs + source code |
| **Risk** | Brick the chip | Crash the process | Panic the kernel |

Bare-metal is hard in a *knowable* way — the reference manual has the answer. Linux kernel space is hard in a *discoverable* way — you need to know what exists before you can look it up. Linux userspace is the most productive but also the most opaque: a lot of low-level detail is taken away, which is powerful until something breaks and you don't know which layer to look at.

## Going into Phase 3

The gap to close: understand Linux deeply enough that it stops feeling like magic. Yocto, the build system, and the full boot chain should help with that — building the OS from scratch is the equivalent of writing the register driver yourself. The "trust me" feeling goes away when you've assembled the pieces yourself.