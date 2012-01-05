cpu 386
bits 32

global _idtloading
global _printGPF

extern _printing

section .text

_idtloading:
	push ebp
	mov ebp, esp
	lidt [ebp+10]
	pop ebp
	ret

_printGPF:
;	cli
;	hlt
;	jmp _printGPF
	call _printing
	iret
