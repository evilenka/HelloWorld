#include <stdio.h>

void insert(int* a, int n, int number){
	int i;
	for(i=n-1;i>=0;--i){
		if(a[i]>number){
			a[i+1]=a[i];
		}
		else{
			a[i+1]=number;
			return;
		}
	}
	a[0]=number;
}

int printarr(FILE* file, int * a, int n){
	int i;
	int ok=1;
	for(i=0; i<n; ++i){
		if(fprintf(file, "%d\n", a[i])==0){
			ok=0;
		}
	}
	return 1-ok;
}

int main(int argc, char* argv[]){
	int* ints = (int*) malloc((argc-1)*sizeof(int));
	if(ints==NULL){
		return 1; //no memory
	}
	int n=0;
	int i;
	for(i=1; i<argc-1; ++i){
		int ok=1;
		int complete=0;
		FILE* file=fopen(argv[i],"r");
		if(file==NULL){
			ok=0;
			continue;
		}
		complete++;
		int number=0;
		int x;
		while(feof(file)==0&&fscanf(file,"%d", &x)!=0){
			number++;
		}
		if(feof(file)==0){
			ok=0;
		}
		else{
			complete++;
			number--;
		}
		switch(complete){
			case 2:
				if(fseek(file, 0, SEEK_SET)==0){
					ints=(int*)realloc(ints, (n+number)*sizeof(int));
					while(ints!=NULL&&fscanf(file, "%d", &x)&&feof(file)==0){
						insert(ints, n, x);
						++n;
					}
				}
			case 1:
				fclose(file);
		}
	}
	int ok=1;
	FILE* file=fopen(argv[argc-1],"w");
	if(file==NULL){
		return 2;
	}
	if(printarr(file, ints, n)==1){
		ok=0;
	}
	fclose(file);
	free(ints);
	if(ok){
		return 0; //ok
	}
	else{
		return 2; //can't print
	}
}
