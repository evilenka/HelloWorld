#include <stdio.h>

char* code="org 0x7c00\n"
"xor ax, ax\n"
"cli\n"
"mov ds, ax\n"
"mov es, ax\n"
"mov ss, ax\n"
"pushf\n"
"xor ah, ah\n"
"push ax\n"
"popf\n"
"pushf\n"
"pop ax\n"
"and ah, 0xF0\n"
"cmp ah, 0xF0\n"
"je _4\n"
"mov ah, 0x70\n"
"push ax\n"
"popf\n"
"pushf\n"
"pop ax\n"
"and ah, 0x70\n"
"jz _4\n"
"popf\n"
"sti\n"
"mov ah, 0x02\n"
"mov al, [sectors]\n"
"mov cx, 0x0002\n"
"mov bx, str2\n"
"int 0x13\n"
"\n"
"mov cx, end2-str2\n"
"mov si, str2\n"
"cld\n"
"_1:\n"
"        xor eax, eax\n"
"        lodsb\n"
"        mov ebx, [crcval]\n"
"        xor eax, ebx\n"
"        and eax, 0x000000FF\n"
"        push cx\n"
"        mov cx, 8\n"
"_2:\n"
"        shr eax, 1\n"
"        jnc _3\n"
"        xor eax, [crcpoly]\n"
"_3:\n"
"        loop _2\n"
"        shr ebx, 8\n"
"        xor eax, ebx\n"
"        mov [crcval], eax\n"
"        pop cx\n"
"        loop _1\n"
"        mov eax, [crcval]\n"
"        xor eax, 0xFFFFFFFF\n"
"        mov [crcval], eax\n"
"        cmp eax, [crc32]\n"
"        jnz _4\n"
"_5:\n"
"mov ax, 0x0300\n"
"xor bx, bx\n"
"int 0x10\n"
"\n"
"mov ax, 0x1301\n"
"mov bx, 0x0007\n"
"mov cx, end2-str2\n"
"mov bp, str2\n"
"int 0x10\n"
"_4:\n"
"cli\n"
"hlt\n"
"crc32:\n"
"dd 0x%X\n"
"crcpoly:\n"
"        dd 0xEDB88320\n"
"crcval:\n"
"        dd 0xFFFFFFFF\n"
"sectors:\n"
"db %d\n"
"end:\n"
"times 510-($-$$) db 0\n"
"db 0x55, 0xaa\n"
"str2:\n";

char* code2="end2:\n"
"        db 0\n"
"times %d-($-$$) db 0";
//512*(sectors+1)

uint_least32_t Crc32(unsigned char *buf, size_t len)
{
    uint_least32_t crc_table[256];
    uint_least32_t crc; int i, j;
 
    for (i = 0; i < 256; i++)
    {
        crc = i;
        for (j = 0; j < 8; j++){
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
	}
 
        crc_table[i] = crc;
    };
 
    crc = 0xFFFFFFFFUL;
 
	while (len--) {
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	}

 
    return crc ^ 0xFFFFFFFFUL;
}


int main(int argc, char* argv[]){
        if(argc==1){
                printf("%s\n","No args");
                return 1;
        }
        unsigned char buffer[16384];
        FILE* f=fopen(argv[1],"r");
        if(f==NULL){
                printf("%s\n","No file");
                return 1;
        }
	fseek(f, 0, SEEK_END);
	if(ftell(f)>30720){
		printf("%s\n", "File too long");
	}
	fseek(f, 0, SEEK_SET);
        int read=fread(buffer, 1, 30720, f);
        fclose(f);
	int sectors=(read+1)>>9;
	if((read+1)&511){
		sectors++;
	}
	f=fopen("check", "w");
	if(f==NULL){
		printf("%s\n","Can't write assembler source");
		return 1;
	}
	fprintf(f,code,Crc32(buffer, read),sectors);
	int j;
	for(j=0;j<read;++j){
		fprintf(f,"db 0x%X\n",buffer[j]);
	}
	fprintf(f, code2, 512*(sectors+1));
	fclose(f);
	system("nasm check -o checkbin");
	return 0;
}
