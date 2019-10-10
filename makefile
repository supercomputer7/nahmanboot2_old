ASM=nasm
FORMAT=bin

ETCHER=dd

BOOTLOADER=bootloader
BOOTSTRAP=bootstrapper
BIN_DIR=binaries
CMAKE=cmake

.PHONY: bootloader bootstrapper etch

bootloader:
	@$(MAKE) -C $(BOOTLOADER) all
	@mv $(BOOTLOADER)/core.bin $(BIN_DIR)/

bootstrapper:
	@$(MAKE) -C $(BOOTSTRAP)
etch:
	# Etch all things together
	@$(ETCHER) status=noxfer conv=notrunc if=$(BIN_DIR)/mbr.bin of=vm.img bs=1 seek=0
	@$(ETCHER) status=noxfer conv=notrunc if=$(BIN_DIR)/boot32.bin of=vm.img bs=1 seek=1048576
	@$(ETCHER) status=noxfer conv=notrunc if=$(BIN_DIR)/core.bin of=vm.img bs=1 seek=1056768

all: bootloader bootstrapper etch
	@$(MAKE) -C $(BOOTLOADER) clean
	@echo "Compile finished!"