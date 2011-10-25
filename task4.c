#include <stdio.h>

char* strings[]={
"org 0x7c00",
"xor ax, ax",
"cli",
"mov ds, ax",
"mov es, ax",
"mov ss, ax",
"sti",
"mov ah, 0x02",
"mov al, [sectors]",
"mov cx, 0x0002",
"mov bx, str2",
"int 0x13",

"mov cx, end2-str2",
"mov si, str2",
"cld",
"_1:",
"        xor eax, eax",
"        lodsb",
"        mov ebx, [crcval]",
"        xor eax, ebx",
"        and eax, 0x000000FF",
"        push cx",
"        mov cx, 8",
"_2:",
"        shr eax, 1",
"        jnc _3",
"        xor eax, [crcpoly]",
"_3:",
"        loop _2",
"        shr ebx, 8",
"        xor eax, ebx",
"        mov [crcval], eax",
"        pop cx",
"        loop _1",
"        mov eax, [crcval]",
"        xor eax, 0xFFFFFFFF",
"        mov [crcval], eax",
"        cmp eax, [crc32]",
"        jnz _4",
"_5:",
"mov ax, 0x0300",
"xor bx, bx",
"int 0x10",

"mov ax, 0x1301",
"mov bx, 0x0007",
"mov cx, end2-str2",
"mov bp, str2",
"int 0x10",
"_4:",
"crc32:",
//dd 0xololo, pos 49
"crcpoly:",
"        dd 0xEDB88320",
"crcval:",
"        dd 0xFFFFFFFF",
"sectors:",
//db sectors, pos 54
"end:",
"times 510-($-$$) db 0",
"db 0x55, 0xaa",
"str2:",
//db 0xololo\n db 0xololo\n ..., pos 58
"end2:",
"        db 0"
//times 512*(sectors+1)-($-$$) db 0, total 61
};

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
        int read=fread(buffer, 1, 16384, f);
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
	int i;
	for(i=0; i<61; ++i){
		fprintf(f,"%s\n",strings[i]);
		if(i==49){
			fprintf(f, "dd 0x%X\n", Crc32(buffer, read));
		}
		if(i==54){
			fprintf(f,"db %d\n", sectors);
		}
		if(i==58){
			int j;
			for(j=0;j<read;++j){
				fprintf(f,"db 0x%X\n",buffer[j]);
			}
		}
	}
	fprintf(f,"times %d-($-$$) db 0", 512*(sectors+1));
	fclose(f);
	system("nasm check -o checkbin");
	return 0;
}
