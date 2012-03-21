cpu 386
bits 32

global idtloading
global printGPF

extern printing

section .text

idtloading:
push ebp
mov ebp, esp
lidt [ebp+10]
pop ebp
ret

printGPF:
call printing
_printGPF_loop:
cli
hlt
jmp _printGPF_loop
iret
