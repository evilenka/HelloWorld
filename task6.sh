nasm -f win32 task6.1.asm -o task6.1.obj
gcc -c task6.2.c -o task6.2.obj
gcc task6.2.obj task6.1.obj -o task6.exe
./task6.exe
