# $@ = target file
# $< = first dependency
# $^ = all dependencies

# First rule is the one executed when no parameters are fed to the Makefile
all: os-image.bin

kernel.bin: kernel-entry.o kernel.o screen.o lowlevel.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary -nostdlib
	
kernel-entry.o: kernel-entry.asm
	nasm $< -f elf -o $@

kernel.o: kernel.c lowlevel.c
	gcc -m32 -ffreestanding -fno-pie -c -o $@ $<

screen.o: screen.c
	gcc -m32 -ffreestanding -fno-pie -c -o $@ $<

lowlevel.o: lowlevel.c
	gcc -m32 -ffreestanding -fno-pie -c -o $@ $<


mbr.bin: mbr.asm
	nasm $< -f bin -o $@

os-image.bin: mbr.bin kernel.bin
	cat $^ > $@

qemu: os-image.bin
	qemu-system-i386 -fda $<

clean:
	$(RM) *.bin *.o *.dis