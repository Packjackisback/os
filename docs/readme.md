# packOS

A simple 32-bit x86 os written from scratch in C and asm (for now).

## Structure

```
.
├── boot
│   └── boot.asm # bootloader
├── build        # used for building, will be cleared with make clean
├── docs         # place of documentation
├── kernel       # where the code for the kernel goes, obv
│   ├── arch     # architecture specific stuff, usually stubs and linker stuff
│   ├── core     # holds the kernel and init files themselves, loads the rest
│   ├── cpu      # holds stuff for the cpu like handlers
│   ├── drivers  # holds drivers like vga and keyboard
│   ├── include  # holds header files
│   ├── lib      # holds libraries, like string
│   └── mm       # memory management
└── Makefile
```

## Prereqs

- `i386-elf-gcc` cross compiler
- `nasm`
- `qemu-system-i386` for running, i suppose you could run bare metal if you want
- make

## Build instructions

Build with `make`, clean artifacts with `make clean`, and run with `make run`.

## Memory partitions

0x00000000 - 0x000003FF : Real mode IVT
0x00000400 - 0x000004FF : BIOS
0x00000500 - 0x00007BFF : Free (used by bootloader)
0x00007C00 - 0x00007DFF : bootloader (512 bytes)
0x00007E00 - 0x0009FFFF : Free
0x000A0000 - 0x000BFFFF : VGA memory
0x000C0000 - 0x000FFFFF : BIOS
0x00100000 - 0x003FFFFF : Kernel
0xC0000000 - 0xCFFFFFFF : Kernel heap

## Contributing

Contributions are welcome, but as this is a personal project might not get merged if seen as differing from my vision and practices. Read the coding style documentation for information.
