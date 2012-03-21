nasm task7.asm -o task7.1.bin
gcc -c task7.c -o task7.obj
ld --script ldscript task7.obj -o task7.2.obj
objcopy -O binary task7.2.obj task7.2.bin
cat task7.1.bin task7.2.bin > task7.bin

