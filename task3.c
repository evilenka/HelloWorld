#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc==1){
		printf("%s\n","No args");
		return 1;
	}
	unsigned char buffer[10000];
	FILE* f=fopen(argv[1],"r");
	if(f==NULL){
		printf("%s\n","No file");
		return 1;
	}
	int read=fread(buffer, 1, 10000, f);
	if(read<10000){
		printf("%s\n","Can't read, read");
		return 1;
	}
	fclose(f);
	int i;
	unsigned char boot[512];
	for(i=0;i<10000;++i){
		if(buffer[i-1]==0x55&&buffer[i]==0xaa&&i>=511){
			int j;
			for(j=i-511;j<=i;++j){
				boot[j-i+511]=buffer[j];
			}
			break;
		}
	}
	f=fopen("boot","w");
	if(f==NULL){
		printf("%s\n","Can't write bytes");
		return 1;
	}
	int written=fwrite(boot,1,512,f);
	if(written<512){
		printf("%s\n","Not enough bytes written");
		return 1;
	}
	fclose(f);
	system("ndisasm boot>disasm");
	return 0;
}
