#May be useful: to disassmble: objdump -d binary --architecture=i386 -D <file>
#-D forces it to do everything including data, so may be garbage at end?
CC = bcc
CFLAGS = -ansi -c

AS = as86
LD = ld86

NEWFLOPPY = dd if=/dev/zero of=floppya.img bs=512 count=2880

all: floppya.img

#makes a new floppy if one doesn't exist
floppya.img::
	[ -r floppya.img ] || $(NEWFLOPPY)

bootload: bootload.asm
	nasm bootload.asm

write_bootload: floppya.img

floppya.img:: bootload
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

kernel: kernel.c kernel_asm.o functions.o
	$(CC) $(CFLAGS) -o kernel.o kernel.c
	$(LD) -o kernel -d kernel.o kernel_asm.o functions.o

kernel_asm.o: kernel.asm
	$(AS) kernel.asm -o kernel_asm.o

write_kernel: floppya.img

floppya.img:: kernel
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

clean_floppy:
	$(NEWFLOPPY)

functions.o: functions.c functions.h
	$(CC) $(CFLAGS) functions.c

run: all
	qemu-system-i386 -fda floppya.img -boot a -no-fd-bootchk

clean:
	rm -f core kernel bootload *.o *~ floppya.img
