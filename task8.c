void idtloading(unsigned int, unsigned int);
void printGPF();

void idtform(){
	struct IDTdescr {
        	unsigned short offset_1;
	        unsigned short selector;
        	unsigned char zero;
	        unsigned char type_attr;
        	unsigned short offset_2;
	} idt[256];
	int i;
	for(i=0;i<256;++i){
		idt[i].offset_1=0;
		idt[i].offset_2=0;
		idt[i].zero=0;
		idt[i].selector=0;
		idt[i].type_attr=0;
	}
	idt[13].offset_1=(short)printGPF;
	idt[13].offset_2=(int)printGPF>>16;
	idt[13].zero=0;
	idt[13].selector=8;
	idt[13].type_attr=0x8E;
	idtloading((256*8-1)<<16,(unsigned int)idt); //IDT is loaded
	*(char*)0x100000='1'; //must be GPF, nothing happens=(
}

extern void printing(){
	char msg[4]={'G','P','F',0};
	int i;
	for(i=0; msg[i]; ++i){
		*(short*)(0xB8000+2*i)=0x0700+msg[i];
	}
}
