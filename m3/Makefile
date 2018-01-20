#May be useful: to disassmble: objdump -d binary --architecture=i386 -D <file>
#-D forces it to do everything including data, so may be garbage at end?
CC = bcc
CFLAGS = -ansi -c

AS = as86
LD = ld86

all: floppya.img

#makes a new floppy if one doesn't exist
floppya.img:: loadFile
	if ! [ -e floppya.img ]; then \
		dd if=/dev/zero of=floppya.img bs=512 count=2880; \
		dd if=map.img of=floppya.img bs=512 seek=1 conv=notrunc; \
		dd if=dir.img of=floppya.img bs=512 seek=2 conv=notrunc; \
		./loadFile message.txt; \
		./loadFile tstprg; \
	fi

loadFile: loadFile.c
	gcc loadFile.c -o loadFile

bootload: bootload.asm
	nasm bootload.asm

floppya.img:: bootload
	dd if=bootload of=floppya.img bs=512 count=1 conv=notrunc

kernel: kernel.c kernel_asm.o
	$(CC) $(CFLAGS) -o kernel.o kernel.c
	$(LD) -o kernel -d kernel.o kernel_asm.o

kernel_asm.o: kernel.asm
	$(AS) kernel.asm -o kernel_asm.o

floppya.img:: kernel
	dd if=kernel of=floppya.img bs=512 conv=notrunc seek=3

run: all
	qemu-system-i386 -fda floppya.img -boot a -no-fd-bootchk

clean:
	rm -f core kernel bootload *.o *~ floppya.img loadFile
