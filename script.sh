export PATH="../cross/bin:$PATH"

nasm -felf64 -o multiboot.o multiboot.asm
nasm -felf64 -o protectedStart.o protectedStart.asm
nasm -felf64 -o paging.o paging.asm
nasm -felf64 -o checks.o checks.asm
nasm -felf64 -o gdt.o gdt.asm
nasm -felf64 -o longStart.o longStart.asm
nasm -felf64 -o idt.o idt.asm

x86_64-elf-gcc -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -c kernel.c -o kernel.o
x86_64-elf-gcc -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -c idtSetup.c -o idtSetup.o
x86_64-elf-gcc -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -c keyboard.c -o keyboard.o
x86_64-elf-gcc -ffreestanding -mcmodel=large -mno-red-zone -mno-mmx -c timer.c -o timer.o
x86_64-elf-gcc -ffreestanding -O2 -nostdlib -lgcc -n -T linker.ld -o isofiles/boot/kernel.bin multiboot.o checks.o protectedStart.o paging.o gdt.o longStart.o kernel.o idt.o idtSetup.o timer.o keyboard.o
grub-mkrescue -o myos.iso isofiles/

qemu-system-x86_64 -cdrom myos.iso
#qemu-system-x86_64 -monitor stdio myos.iso

echo "done"
