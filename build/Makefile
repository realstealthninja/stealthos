OBJECTS = loader.o kmain.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -c
LDFLAGS = -T ../bootloader/link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS)  -o iso/boot/kernel.elf

stealthos.iso: kernel.elf 
		genisoimage -R                              \
					-b boot/grub/stage2_eltorito    \
					-no-emul-boot                   \
					-boot-load-size 4               \
					-A stealthos                    \
					-input-charset utf8             \
					-quiet                          \
					-boot-info-table                \
					-o stealthos.iso                \
					iso

run: stealthos.iso
	bochs -f bochsrc.txt -q


%.o: ../bootloader/%.s
	$(AS) $(ASFLAGS) $< -o $@
%.o: ../src/%.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o stealthos.iso