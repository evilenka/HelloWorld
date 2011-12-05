cpu 386

global _multiply
extern _writedouble

section .text

_multiply:
	push ebp
	mov ebp, esp
	sub esp, byte 8
	finit
	fld qword [ebp+8]
	fimul dword [ebp+16]
	fstp qword [ebp-8]
	call _writedouble
	mov esp, ebp
	pop ebp
	ret
