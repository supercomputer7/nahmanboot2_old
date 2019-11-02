ORG 0x800
BITS 16
start:

cli

;; check memory map

memory_detection:

call detect_memory_map
jmp check_a20line_main

detect_memory_map:
pusha

push eax
push ecx
push ebx
xor eax,eax
mov ax,0x70
mov es,ax
xor ebx,ebx

	mov ecx, 70
	.loop_clean:

		mov byte [es:bx], 0
		inc bx
	loop .loop_clean

pop ebx
pop ecx
pop eax


xor eax,eax
mov ax,0x70
mov es,ax

	xor esi,esi ; ESI is a counter

	xor di,di
	xor ebx,ebx
	mov edx,0x534D4150
	mov eax,0xe820
	mov ecx,24

	int 0x15
	;jc continue_next_method ; carry flag = failure of method
	
	;cmp eax,0x534D4150
	;jne continue_next_method ; EAX != magic number => failure of method
.loop_detect_e820:
	
	mov edx,0x534D4150
	add di,24 ; next 24 bytes, so we use 24 bytes quantities
	mov eax,0xe820
	mov ecx,24
	
	cmp ebx,0
	je .end_detect_mem

	int 0x15
	inc esi
	jc .end_detect_mem

	jmp .loop_detect_e820

.end_detect_mem:

	xor eax,eax
	mov ax,0x2000
	mov es,ax
	xor bx,bx
	mov dword [es:bx],0x700
	add bx,4
	mov dword [es:bx],esi

popa
ret

error_a20line_msg db "Can't enable A20 line",0

;;; check A20 line is enabled

; Function: check_a20
; Purpose: to check the status of the a20 line in a completely self-contained state-preserving way.
;          The function can be modified as necessary by removing push's at the beginning and their
;          respective pop's at the end if complete self-containment is not required.
; Returns: 0 in ax if the a20 line is disabled (memory wraps around)
;          1 in ax if the a20 line is enabled (memory does not wrap around)
check_a20:
	pushf
	cli ; disabling interrupts
	push ds
	push es
	push di
	push si

	xor ax, ax
	mov es, ax
 
	not ax ; ax = 0xFFFF
	mov ds, ax
 
	mov di, 0x0500
	mov si, 0x0510
 
	mov al, byte [es:di]
	push ax
 
	mov al, byte [ds:si]
	push ax
 
	mov byte [es:di], 0x00
	mov byte [ds:si], 0xFF
 
	cmp byte [es:di], 0xFF
 
	pop ax

	mov byte [ds:si], al
 
	pop ax
	mov byte [es:di], al
 
	mov ax, 0
	je check_a20_exit
 
	mov ax, 1
 
check_a20_exit:
	pop si
	pop di
	pop es
	pop ds
	popf
	ret


; Function: enable_a20line
; Purpose: try to enable A20 Line first with BIOS interrupt and if it's not working - with the Keyboard controller. If failed - stop execution.
; Returns: nothing

enable_a20line:


	;; Try to enable A20 line with bios
	mov     ax,2403h                ;--- A20-Gate Support ---
	int     15h
	jb      a20_failed                  ;INT 15h is not supported
	cmp     ah,0
	jnz     a20_failed                  ;INT 15h is not supported
	 
	mov     ax,2402h                ;--- A20-Gate Status ---
	int     15h
	jb      a20_failed              ;couldn't get status
	cmp     ah,0
	jnz     a20_failed              ;couldn't get status
	 
	cmp     al,1
	jz      a20_activated           ;A20 is already activated
	 
	mov     ax,2401h                ;--- A20-Gate Activate ---
	int     15h
	jb      a20_failed             ;couldn't activate the gate
	cmp     ah,0
	jnz     a20_failed              ;couldn't activate the gate

	a20_failed:
	call enable_a20line_keyboard ; try to enable A20 line with keyboard controller
	call check_a20
	cmp ax,1 ; A20 line is activated
	jmp a20_activated
	
	a20_fault:
		;call clear_screen
		;lea bx,[error_a20line_msg]
		;call print_string
		jmp $ ; stop execution because of failure

	a20_activated:
	ret

; Function: enable_a20line
; Purpose: try to enable A20 Line with Keyboard controller.
; Returns: nothing

enable_a20line_keyboard:
 
        call    a20wait
        mov     al,0xAD
        out     0x64,al
 
        call    a20wait
        mov     al,0xD0
        out     0x64,al
 
        call    a20wait2
        in      al,0x60
        push    eax
 
        call    a20wait
        mov     al,0xD1
        out     0x64,al
 
        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al
 
        call    a20wait
        mov     al,0xAE
        out     0x64,al
 
        call    a20wait
        sti
        ret
 
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret


check_a20line_main:

	
	
	call check_a20
	
	cmp ax,1 ; 0 = a20 line is disabled, 1 = enabled
	je stage4_framebuffer ; proceed to next stage
	call enable_a20line ; otherwise enable a20line

;;; setup framebuffer
stage4_framebuffer:

    mov esp,0x4000 ; important! set a sane value here

	mov ax,0x3
	int 0x10

	mov ah,0x1
	mov ch,0x3f
	int 0x10

	;; get frame buffer address

	;mov ax, 0x4F02	; set VBE mode
	;mov bx, 0x4118	; VBE mode number; notice that bits 0-13 contain the mode number and bit 14 (LFB) is set and bit 15 (DM) is clear.
	;mov bx,0x4138
	;int 0x10			; call VBE BIOS
	;cmp ax, 0x004F	; test for error

	;error:
	;	jne error

cli


jmp load_kernel

%include "load_core.asm"

BITS 16

protected_mode:
;;; setup protected mode

; clear Segments before we move to protected mode
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


jmp 0x8:boot2 ; long jump to the code segment

gdt_start:
    dq 0x0
gdt_code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
gdt_data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:

gdt_pointer:
    dw gdt_end - gdt_start
    dd gdt_start
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

;;; print menu based MBR table

BITS 32

boot2:

    cli

    mov esp,0x4000 ; important! set a sane value here

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax
	

	mov cx,0

cli

lidt [IDT]

push 0x100000
ret ; jump!

hlt ; hopefully we never reach this opcode

IDT:
dw 256*8-1
dd 0x7000;

times (1024 - ($-$$)) db 0

%include "RealModeSwitcher.asm"

times (4096 - ($-$$)) db 0

