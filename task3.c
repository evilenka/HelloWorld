#include <stdio.h>

int main(int argc, char* argv[]){
	if(argc==1){
		fprintf(stderr,"No args\n");
		return 1;
	}
	int n=1;
	unsigned char buffer[512];
	FILE* f=fopen(argv[1],"r");
	if(f==NULL){
		fprintf(stderr,"No file\n");
		return 1;
	}
	if(fseek(f,510,SEEK_SET)!=0){
		fprintf(stderr,"Can't seek\n");
		exit(1);
	}
	unsigned char prev=fgetc(f);
	unsigned char c=fgetc(f);
	while(feof(f)==0){
		if(c==0xaa && prev==0x55){
			if(fseek(f,-512,SEEK_CUR)!=0){
				prev=c;
				c=fgetc(f);
				continue;
			}
			int read=fread(buffer, sizeof(char), 512, f);
			if(read < 512){
				fseek(f, 512-read, SEEK_CUR);
				prev=c;
				c=fgetc(f);
				continue;
			}
			char name[10];
			sprintf(name, "boot%d", n);
			FILE* boot=fopen(name, "w");
			if(boot==NULL){
				prev=c;
				c=fgetc(f);
				continue;
			}
			fwrite(buffer, sizeof(char), 512, boot);
			fclose(boot);
			char command[100];
			sprintf(command, "ndisasm %s > %sdisasm",name,name);
			system(command);
			++n;
		}
		prev=c;
		c=fgetc(f);
	}
	fclose(f);
	return 0;
}
