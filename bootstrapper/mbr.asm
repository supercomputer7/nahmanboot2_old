;Copyright 2019 Liav Albani

;Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

;The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

BITS 16
ORG 0x600

start:
  cli                         ; We do not want to be interrupted
  mov eax,0x7fffffff 
  mov ebx,cr0
  and ebx,eax
  mov cr0,ebx ; disable paging just in case someone before this code enabled it (e.g. bootloader like SYSLINUX)
  
  xor ax, ax                  ; 0 AX
  mov ds, ax                  ; Set Data Segment to 0
  mov es, ax                  ; Set Extra Segment to 0
  mov ss, ax                  ; Set Stack Segment to 0
  mov sp, ax                  ; Set Stack Pointer to 0

  ;;CopyLower
    mov cx, 0x0100            ; 256 WORDs in MBR
    mov si, 0x7C00            ; Current MBR Address
    mov di, 0x0600            ; New MBR Address
    rep movsw                 ; Copy MBR
	
  jmp 0:LowStart             ; Jump to new Address
 
LowStart:
cli                        ; Disable interrupts

xor ebx,ebx	
mov bx,word [default_bios_boot_lba]
    call ReadSectors          ; Read Sector

	jmp 0x0:0x800                ; Jump To VBR


;; Defining Disk Address Packet Structure
daps1 db 10h
db 0 ; always 0
daps1_sectors dw 8 ; number of sectors to load - 4
daps1_offset dw 0x800 ; offset to transfer
daps1_segment dw 0x0 ; segment to transfer
daps1_lba dq 1 ; lower 32 bits of 48Bit LBA
default_bios_boot_lba dw 2048
ReadSectors:
    lea si, [daps1]
    mov [daps1_lba],ebx
    call load_data_from_disk
    ret

; Function: load_data_from_disk
; Parameters: no parameters
; Purpose: Load Data from Disk
; Returns: nothing
load_data_from_disk:
    push ax
    push dx
    mov ah,0x42
    mov dl,0x80
    int 13h
    pop dx
    pop ax
ret

	ERROR:
		mov ecx,0xDEADC0DE
		jmp $


times (218 - ($-$$)) nop      ; Pad for disk time stamp
 
DiskTimeStamp times 8 db 0    ; Disk Time Stamp
 
bootDrive db 0                ; Our Drive Number Variable
PToff dw 0                    ; Our Partition Table Entry Offset

times (0x1b4 - ($-$$)) nop    ; Pad For MBR Partition Table