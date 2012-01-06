org 0x7c00

cli
xor ax, ax
mov ds, ax
mov es, ax
sti

mov ax, 0x0201
;xor dx, dx
mov cx, 0x0002
mov bx, 0x7e00
int 0x13

lgdt [structs]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 8:_1

_1:
	bits 32

	mov ax, 16
	mov ds, ax

	mov ss, ax
	mov esp, 0xffff
	
	call ccode

	int 5

stop:
	cli
	hlt
	jmp stop



structs: dw 32
dd gdt0

gdt0: dq 0
gdt1: dq 0x004F9A000000FFF0
gdt2: dq 0x004F92000000FFF0
gdt3: dq 0x004F8E000000FFF0

times (510+$$-$) db 0
db 0x55, 0xaa

ccode:
