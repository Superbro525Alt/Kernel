
CP := cp
RM := rm -rf
MKDIR := mkdir -pv

BIN = kernel
CFG = grub.cfg
ISO_PATH := iso
BOOT_PATH := $(ISO_PATH)/boot
GRUB_PATH := $(BOOT_PATH)/grub
KERNEL_BIN := build/kernel

.PHONY: all
all: make-folders bootloader kernel linker iso create-vm clean
	@echo Make has completed.

make-folders:
	${MKDIR} build
	${MKDIR} sim

bootloader: boot.asm
	nasm -f elf32 boot.asm -o build/boot.o

kernel: kernel.c
	gcc -m32 -c kernel.c -o build/kernel.o

linker: linker.ld ./build/boot.o ./build/kernel.o
	ld -m elf_i386 -T linker.ld -o build/kernel build/boot.o build/kernel.o

iso: kernel
	$(MKDIR) $(GRUB_PATH)
	$(MKDIR) build/completed
	$(CP) $(KERNEL_BIN) $(BOOT_PATH)
	$(CP) $(CFG) $(GRUB_PATH)
	grub-file --is-x86-multiboot $(BOOT_PATH)/$(BIN)
	grub-mkrescue -o build/completed/OS.iso $(ISO_PATH)

create-vm: build/completed/OS.iso
	$(MKDIR) sim
	qemu-img create sim/disk.img 3G
	sudo kvm -boot d -cdrom build/completed/OS.iso -m 512 -hda sim/disk.img


.PHONY: clean
clean:
	$(RM) *.o $(BIN) *iso
	$(RM) ./iso
	${RM} ./build/*.o
	${RM} build/*.o
	${RM} **/*.o
	${RM} ./sim
