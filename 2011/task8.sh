set -e
nasm task8.asm -o task8 --prefix _
nasm -f win32 task8.lidt.asm -o task8.lidt
gcc -m32 -march=i386 -Wall -pedantic -std=c99 -c task8.c -o task8.obj
ld -m elf_i386 --oformat=elf32-i386 --script ldscript task8.obj 
task8.lidt -o task8.1.obj
objcopy -O binary task8.1.obj task8.bin
cat task8 task8.bin > task8boot.bin
