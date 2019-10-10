BOOTLOADER_OBJS = \
       init.o \
       load.o \
	   Memory/Allocator.o \
	   Display/StandardVGA.o \
	   Display/TerminalDriver.o \
	   LibC/stdstring.o

OUTPUT = core.bin
CXXFLAGS = -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
LDFLAGS = -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib -lgcc
ASMFLAGS = -f elf32
CXX = /home/liav/git-projects/nahmanboot2/toolchain/bin/i686-elf-g++
LD = /home/liav/git-projects/nahmanboot2/toolchain/bin/i686-elf-g++
ASM = /usr/bin/nasm
LINK = /home/liav/git-projects/nahmanboot2/toolchain/bin/i686-elf-g++

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
