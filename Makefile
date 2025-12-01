CC = i386-elf-gcc
LD = i386-elf-ld
ASM = nasm

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -nostdlib -fno-pie -Ikernel/include -Ikernel
LDFLAGS = -m elf_i386 -T kernel/linker.ld

KERNEL_C_SOURCES = $(shell find kernel -name '*.c')
KERNEL_C_OBJECTS = $(patsubst kernel/%.c, build/%.o, $(KERNEL_C_SOURCES))

KERNEL_ASM_SOURCES = $(shell find kernel -name '*.asm' ! -name 'kernel_entry.asm')
KERNEL_ASM_OBJECTS = $(patsubst kernel/%.asm, build/%.o, $(KERNEL_ASM_SOURCES))

all: build/os.img

build/os.img: build/boot.bin build/kernel.bin
	cat build/boot.bin build/kernel.bin > build/os.img
	@truncate -s 1440K build/os.img

build/boot.bin: boot/boot.asm
	@mkdir -p build
	$(ASM) -f bin boot/boot.asm -o build/boot.bin

build/kernel_entry.o: kernel/kernel_entry.asm
	@mkdir -p build
	$(ASM) -f elf32 kernel/kernel_entry.asm -o build/kernel_entry.o

build/%.o: kernel/%.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

build/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/kernel.bin: build/kernel_entry.o $(KERNEL_C_OBJECTS) $(KERNEL_ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o build/kernel.elf build/kernel_entry.o $(KERNEL_C_OBJECTS) $(KERNEL_ASM_OBJECTS)
	objcopy -O binary build/kernel.elf build/kernel.bin

run: build/os.img
	qemu-system-i386 -fda build/os.img -vga std

debug: build/os.img
	qemu-system-i386 -fda build/os.img -vga std -s -S

clean:
	rm -rf build/*

.PHONY: all run debug clean
