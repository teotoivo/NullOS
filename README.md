# NullOS

NullOS is a small, freestanding 64-bit x86 hobby operating system built to learn
kernel development. It currently boots through Limine, enters a higher-half C
kernel, initializes basic x86_64 descriptor and interrupt infrastructure, and
writes diagnostic text to both a graphical framebuffer and the COM1 serial port.

This is an early kernel, not a usable general-purpose operating system. There is
no memory allocator, scheduler, filesystem, syscall interface, or running
userland yet.

## Current status

The current boot path is:

1. Limine loads the higher-half ELF64 kernel and supplies a framebuffer.
2. The architecture entry point installs a five-entry GDT.
3. The legacy 8259 PIC is remapped; every IRQ except the PS/2 keyboard IRQ is
   masked.
4. A 256-entry IDT and a C-level interrupt-handler table are installed, then
   interrupts are enabled.
5. The kernel validates the Limine framebuffer and initializes COM1 serial
   output.
6. The TTY enables serial and framebuffer backends, prints test messages, and
   triggers a breakpoint interrupt as an interrupt-path test.
7. The CPU enters an idle `hlt` loop.

### Implemented

- Limine v9 boot integration and a higher-half x86_64 linker layout.
- Bootable hybrid ISO layout for legacy BIOS and UEFI systems.
- Freestanding C17 kernel built with an `x86_64-elf` cross-toolchain.
- Kernel/user code and data descriptors in a Global Descriptor Table (GDT).
- 256-entry Interrupt Descriptor Table (IDT) with assembly ISR stubs and a
  C-level handler-registration API.
- Legacy PIC remapping and initial PS/2 keyboard IRQ routing.
- Diagnostic handlers for breakpoints, page faults, unhandled exceptions, and
  keyboard scancodes.
- COM1 serial initialization and text output at 38400 baud.
- 32-bit Limine framebuffer validation, pixel drawing, and bitmap-font text
  rendering.
- A small TTY abstraction that can mirror character, decimal, and hexadecimal
  output to serial and framebuffer backends.
- Fatal kernel panic output followed by a halted CPU loop.
- QEMU run/debug targets and a GDB command file.

### Present but incomplete

- Interrupt handling is experimental. The ISR frame/error-code handling still
  needs to be validated and corrected for every x86 exception before it should
  be treated as stable.
- The keyboard path reports raw PS/2 scancodes only; it does not translate keys,
  track modifiers, or provide an input queue.
- Framebuffer text supports newlines and line wrapping, but not scrolling,
  cursor control, colors through the TTY API, or display-mode fallback.
- Serial initialization performs a loopback check, but kernel startup currently
  ignores failure and continues enabling the serial TTY backend.
- `libs/` and `userland/` are scaffolding. They are not part of the top-level
  build and contain no runtime or programs yet.

## Repository layout

| Path | Contents |
| --- | --- |
| `kernel/arch/x86_64/` | Entry code, GDT, PIC, IDT, ISR stubs, and serial I/O |
| `kernel/core/` | Limine requests, kernel initialization, TTY, and panic code |
| `kernel/drivers/` | Framebuffer console and bundled bitmap fonts |
| `kernel/include/` | Kernel, driver, architecture, and Limine headers |
| `build_scripts/` | Shared toolchain, Limine, QEMU, and GDB make rules |
| `libs/` | Future reusable system-library scaffolding |
| `userland/` | Future startup code and user-space program scaffolding |
| `build/` | Generated objects, kernel ELF, Limine checkout, and ISO |

## Requirements

The checked-in configuration expects these commands to be available:

- `x86_64-elf-gcc` and `x86_64-elf-ld` (GCC 15.1.0 and binutils 2.44 were
  previously tested)
- GNU Make (4.4.1 was previously tested)
- NASM
- Git, used to download the pinned Limine `v9.x-binary` branch
- `xorriso`, used to create the bootable ISO
- `qemu-system-x86_64` to run the OS (10.0.0 was previously tested)
- GDB with x86_64 ELF support for source-level debugging

## Build and run

The intended workflow is:

```sh
make all       # build build/kernel.elf and build/nullos.iso
make run       # build, then boot the ISO in QEMU
make debug     # boot QEMU paused with a GDB server on localhost:1234
make gdb       # connect GDB using .gdbinit (run in another terminal)
make clean     # remove generated build output
```

QEMU is configured with 512 MiB of RAM, serial output connected to the host
terminal, and automatic reboot/shutdown disabled.

## Roadmap / TODO

The next useful milestone is a stable kernel foundation followed by the first
user-mode program. A practical order is:

- [ ] Add an automated, reproducible build smoke test.
- [ ] Correct and validate ISR stack frames, CPU-supplied exception error codes,
  PIC acknowledgements, and exception diagnostics.
- [ ] Add a PIT or APIC timer and a monotonic kernel tick source.
- [ ] Read Limine's memory map and implement a physical page-frame allocator.
- [ ] Add kernel virtual-memory mapping helpers and a small heap allocator.
- [ ] Finish the console with framebuffer scrolling and buffered keyboard input.
- [ ] Introduce a task structure, kernel stacks, and a minimal round-robin
  scheduler.
- [ ] Enter x86_64 ring 3 safely with a TSS and a defined user address space.
- [ ] Implement a small syscall ABI plus a freestanding userland runtime.
- [ ] Load and run the first user program, initially from an embedded or
  Limine-provided module.
- [ ] Add automated boot tests that assert expected serial output in QEMU.

Longer-term goals include a VFS and initial RAM filesystem, storage drivers,
process isolation, and a basic interactive shell. Those should follow the first
verified user-mode program rather than being developed in parallel with an
unstable interrupt and memory foundation.

## License

NullOS is released under the [MIT License](LICENSE). The vendored Limine protocol
header carries its own BSD Zero Clause license notice.
