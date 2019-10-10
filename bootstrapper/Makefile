ASM=nasm
FORMAT=bin
BIN_DIR=binaries

all: 
	$(ASM) -f $(FORMAT) boot32.asm -o ../$(BIN_DIR)/boot32.bin
	$(ASM) -f $(FORMAT) mbr.asm -o ../$(BIN_DIR)/mbr.bin