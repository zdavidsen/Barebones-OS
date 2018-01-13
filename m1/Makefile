CC = bcc
CFLAGS = -ansi -c

AS = as86
LD = ld86

all: bootloader write_kernel

bootloader: make_floppy 
	nasm bootload.asm
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

make_floppy:
	dd if=/dev/zero of=floppya.img bs=512 count=2880

kernel: bootloader
	$(CC) $(CFLAGS) -o kernel.o kernel.c
	$(AS) kernel.asm -o kernel_asm.o
	$(LD) -o kernel -d kernel.o kernel_asm.o

write_kernel: kernel
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

clean:
	rm -f kernel *.o
run: write_kernel
	qemu-system-i386 -fda floppya.img -boot a -no-fd-bootchk 	
