BITS 16
load_kernel:

xor ecx,ecx
mov cx, 2049
.loop_transfer:
		jmp move_data
		.continue:
	loop .loop_transfer

jmp protected_mode


;; Defining Disk Address Packet Structure
daps db 10h
db 0 ; always 0
daps_sectors dw 1 ; number of sectors to load - 4
daps_offset dw 0x8000 ; offset to transfer
daps_segment dw 0x0 ; segment to transfer

daps_lba dq 2064 ; lower 32 bits of 48Bit LBA

location dd 0x100000

;; no getting any parameter but will increase EDI
ReadSectors:
	push esi
    	lea si, [daps]
    	call .load_data_from_disk
	pop esi
	;add word [daps_offset],0x200
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


;; ESI = buffer input, EDI = buffer output, ECX = how much to transfer in double words
move_data:

	call ReadSectors

	mov ax,0x0
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax


	cli
	
	
	;; Setting up GDTR for Protected mode
lgdt [gdt_pointer] ; load the gdt table


mov eax, cr0 
or eax,0x1 ; set the protected mode bit on special CPU reg cr0
mov cr0, eax


jmp 0x8:protected_mode_transfer ; long jump to the code segment
BITS 32
protected_mode_transfer:

    cli
	

    mov esp,0x4000 ; important! set a sane value here

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    push esi
    push edi
	push ecx

	xor esi,esi
	mov si,word [daps_offset]
	xor edi,edi
	mov edi,dword [location]

	xor ecx,ecx
	mov cx,0x200

    .loop_transfer:
       mov al,byte [esi]
        mov byte [edi],al
        inc edi
        inc esi
    loop .loop_transfer

	add dword [location],0x200

	pop ecx
	pop edi
    pop esi

lgdt [gdt2_pointer]
jmp 0x8:return_realmode

gdt2_start:
    dq 0x0
gdt2_code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 00001111b
    db 0x0
gdt2_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 00001111b
    db 0x0
gdt2_end:

gdt2_pointer:
    dw gdt2_end - gdt2_start
    dd gdt2_start
CODE_SEG equ gdt2_code - gdt2_start
DATA_SEG equ gdt2_data - gdt2_start

BITS 16

IDT16:
dw 0x3ff		; 256 entries, 4b each = 1K
dd 0			; Real Mode IVT @ 0x0000

return_realmode:
;;; drop back to real mode (remapping the PIC, reset CPU to realmode)
nop
cli

	mov ax,0x10               ; 8.9.2. Step 4. 
    mov ds,ax 
    mov es,ax 
    mov fs,ax 
    mov gs,ax 
    mov ss,ax 
	
	;mov ax,0x03FF               ; 8.9.2. Step 5. 
    ;mov word [IDT16],ax 
    ;xor ax,ax 
    ;mov word [IDT16+2],ax 
    ;mov word [IDT16+4],ax 
        
	mov eax,cr0                 ; 8.9.2. Step 6. 
    and eax,0x7FFFFFFE
    mov cr0,eax 

	jmp 0:complete_realmode ; 8.9.2. Step 7. 

complete_realmode:
	nop

    mov sp,0x4000

	xor     ax,ax                   ; 8.9.2. Step 8. 
    mov     ds,ax 
    mov     es,ax 
    mov     fs,ax 
    mov     gs,ax 
    mov     ss,ax

	lidt    [IDT16]

jmp load_kernel.continue