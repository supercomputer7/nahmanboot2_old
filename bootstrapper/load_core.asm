BITS 16
load_kernel:

xor ecx,ecx
mov cx, 100
.loop_transfer:
		jmp move_data
		.continue:
	loop .loop_transfer

jmp protected_mode


;; Defining Disk Address Packet Structure
daps db 10h
db 0 ; always 0
daps_sectors dw 1 ; number of sectors to load - 4
daps_offset dw 0x0 ; offset to transfer
daps_segment dw 0x100 ; segment to transfer
daps_lba dq 2064 ; lower 32 bits of 48Bit LBA

ReadSectors:
	push esi
    	lea si, [daps]
    	call .load_data_from_disk
	pop esi
	add dword [daps_lba],1
    ret

; Function: load_data_from_disk
; Parameters: no parameters
; Purpose: Load Data from Disk
; Returns: nothing
.load_data_from_disk:
    push ax
    push dx
    mov ah,0x42
    mov dl,0x80
    int 13h
    pop dx
    pop ax
ret

move_data:
	call ReadSectors
    add word [daps_offset],0x200
    jmp load_kernel.continue