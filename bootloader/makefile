BOOTLOADER_OBJS = \
       init.o \
       load.o \
	   Memory/Allocator.o \
	   Memory/malloc.o \
	   Display/Print.o \
	   Display/StandardVGA.o \
	   Display/TerminalDriver.o \
	   ACPI/RSDP.o \
	   ACPI/XSDT.o \
	   ACPI/RSDT.o \
	   Disk/Disk.o \
	   Disk/StorageDevice.o \
	   Disk/IDEController.o \
	   Disk/AHCIController.o \
	   Filesystem/Partition.o \
	   Filesystem/Ext2Filesystem.o \
	   Kernels/MultibootKernel.o \
	   Parsers/ELF32Parser.o \
	   PCI/Access.o \
	   PCI/MemoryAccess.o \
	   PCI/IOAccess.o \
	   PCI/Device.o \
	   LibC/stdstring.o

OUTPUT = core.bin
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
CXXFLAGS += -I./
LDFLAGS = -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib -lgcc
ASMFLAGS = -f elf32
CXX = i686-elf-g++
LD = i686-elf-g++
ASM = /usr/bin/nasm
LINK = i686-elf-g++

.PHONY: all

all: $(OUTPUT)

%.o: %.asm
	@echo "ASM $<"; $(ASM) $(ASMFLAGS) -o $@ $<

.cpp.o: 
	@echo "CXX $<"; $(CXX) $(CXXFLAGS) -o $@ -c $<

.c.o: 
	@echo "CXX $<"; $(CXX) $(CXXFLAGS) -o $@ -c $<

$(OUTPUT): $(BOOTLOADER_OBJS)
	@echo "LD $@"; $(LD) $(LDFLAGS) -o $@ $(BOOTLOADER_OBJS)

#-include $(CXX_OBJS:%.o=%.d)

clean:
	@echo "CLEAN"; rm -f $(BOOTLOADER_OBJS)
