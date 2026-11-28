#!/bin/bash
i386-elf-gcc -ffreestanding -m32 -g -I/usr/include -c "kernel.c" -o "kernel.o"
i386-elf-ld -o "kernel.bin" -Ttext 0x1000 "kernel_entry.o" "kernel.o" --oformat binary
cat ../bootloader/"boot.bin" "kernel.bin" > "os.bin"
cat "os.bin" ../"failsafe.bin" > "final.bin"
qemu-system-x86_64 -drive file=final.bin,format=raw
