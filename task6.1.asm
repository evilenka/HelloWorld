cpu 386
global _multiply
section .text	

_multiply:
;	xor eax, eax
		
;push bp
;mov bp, sp
finit
fld qword [esp+4]
fimul dword [esp+12]
;pop bp
ret 
