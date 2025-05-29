# nullos

_minimal 64bit hobby OS_

nullos is an OS project built for learning and experimentation.  
It boots via limine, and runs a small kernel written in C.

The Project is still under heavy devlopment

## Repository layout

| Path        | Contents                                              |
| ----------- | ----------------------------------------------------- |
| `kernel/`   | Kernel sources & per-arch sub-dirs (`arch/x86_64/`)   |
| `userland/` | User-space programs (future work)                     |
| `libs/`     | Reusable libraries shared between kernel and userland |
| `build/`    | Top-level build artifacts (ISO images, logs, etc.)    |
| `Makefile`  | Convenience wrapper driving the full build            |
| `LICENSE`   | MIT license text (see below)                          |

## Requirements

| Tool                   | Tested version |
| ---------------------- | -------------- |
| `gcc` cross x86_64-elf | 15.1.0         |
| `binutils` cross       | 2.44           |
| `make`                 | 4.4.1          |
| `qemu-system-x86_64`   | 10.0.0         |

## Quick start

```bash
# build the kernel and bootable ISO
$ make all

# run it in QEMU with sane defaults
$ make run
```

Targets:

| Command      | Result                                   |
| ------------ | ---------------------------------------- |
| `make all`   | Builds kernel ELF and `build/nullos.iso` |
| `make clean` | Removes all generated files              |
| `make run`   | Launches QEMU with the freshly‑built ISO |

## Debugging

| Technique           | How to start                                                                            |
| ------------------- | --------------------------------------------------------------------------------------- |
| **QEMU + GDB stub** | `make debug` – launches QEMU with `-s -S`; then `make gdb ` - launches gdb -x .gdbinit` |

## Roadmap / TODO

- Memory management (paging, simple allocator)
- Interrupt & exception handling
- Basic drivers (timer, keyboard, serial)
- Syscall layer and minimal userland runtime

## License

nullos is released under the **MIT License** – see [`LICENSE`](LICENSE) for full text.
