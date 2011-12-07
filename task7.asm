org 0x7c00

lgdt [structs]

mov eax, cr0
or eax, 1
mov cr0, eax

jmp 8:_1

_1:
	bits 32
	mov ax, 16
	mov ds, ax

	mov word [0xb8000], 0x0731

stop:
	cli
	hlt
	jmp stop

structs: dw 24
dd gdt0

gdt0: dq 0
gdt1: dq 0x00CF9A000000FFFF
gdt2: dq 0x00CF92000000FFFF

times (510+$$-$) db 0
db 0x55, 0xaa
