BITS 32

switch_structure:
.sig dd 0x70BBCAFE
.returnaddr dd 0x10076d
.previousstack dd 0x0
.interrupt dw 0x0
.eax_reg dd 0x0
.ebx_reg dd 0x0
.edx_reg dd 0x0
.ecx_reg dd 0x0
.esi_reg dd 0x0
.edi_reg dd 0x0
.es_reg  dw 0x0
.gs_reg  dw 0x0
.fs_reg  dw 0x0
.jump 	 dd entry

entry:
BITS 32
nop
;jmp .protectedmode
;;; dropping to realmode

lgdt [GDT16.pointer]
jmp 0x8:.return_realmode

.protectedmode:
    mov ax, DATA32_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax

ret

BITS 16

.return_realmode:
;;; drop back to real mode (remapping the PIC, reset CPU to realmode)
nop
cli


mov     ax,0x0010
        mov     ds,ax 
        mov     es,ax 
        mov     fs,ax 
        mov     gs,ax 
        mov     ss,ax 
	
	mov     ax,0x03FF               
        mov     word [IDT16_n],ax 
        xor     ax,ax 
        mov     word [IDT16_n+2],ax 
        mov     word [IDT16_n+4],ax 
        

	mov     eax,cr0 
        and     eax,0x7FFFFFFE
        mov     cr0,eax 


	jmp 0:.complete_realmode 

.complete_realmode:
nop

	lidt    [IDT16_n]
	xor     ax,ax
        mov     ds,ax 
        mov     es,ax 
        mov     fs,ax 
        mov     gs,ax 
        mov     ss,ax

;;; return to text mode

push eax
push ebx
push ecx
push edx
push esi
push edi
push es

mov ax,word [switch_structure.es_reg]
mov es,ax

mov eax,dword [switch_structure.eax_reg]
mov ebx,dword [switch_structure.ebx_reg]
mov ecx,dword [switch_structure.ecx_reg]
mov edx,dword [switch_structure.edx_reg]
mov edi,dword [switch_structure.edi_reg]
mov esi,dword [switch_structure.esi_reg]

int 0x10

pop es
pop edi
pop esi
pop edx
pop ecx
pop ebx
pop eax
;
; clear Segments before we move to protected mode
	mov ax,0x0
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax


	cli
	
	;; Setting up GDTR for Protected mode
lgdt [GDT32.pointer] ; load the gdt table


mov eax, cr0 
or eax,0x1 ; set the protected mode bit on special CPU reg cr0
mov cr0, eax


jmp 0x8:entry.protectedmode ; long jump to the code segment

IDT16_n:
dq 0
dq 0

GDT32:
.entry:
    dq 0x0
.code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
.data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
.end:

.pointer:
    dw GDT32.end - GDT32
    dd GDT32
CODE32_SEG equ GDT32.code - GDT32.entry
DATA32_SEG equ GDT32.data - GDT32.entry


GDT16:
.entry:
    dq 0x0
.code:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10011010b
    db 00001111b
    db 0x0
.data:
    dw 0xFFFF
    dw 0x0
    db 0x0
    db 10010010b
    db 00001111b
    db 0x0
.END:

.pointer:
    dw GDT16.END - GDT16
    dd GDT16
CODE16_SEG equ GDT16.code - GDT16.entry
DATA16_SEG equ GDT16.data - GDT16.entry

BITS 32

cli
hlt

dw 0xAA55
dw 0x55AA
