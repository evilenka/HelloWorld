nasm task8.asm -o task8
nasm -f win32 task8.lidt.asm -o task8.lidt
gcc -c task8.c -o task8.obj
ld --script ldscript task8.obj task8.lidt -o task8.1.obj
objcopy -O binary task8.1.obj task8.bin
cat task8 task8.bin > task8boot.bin
