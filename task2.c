#include <stdio.h>

void insert(int* a, int n, int number){
	int i;
	for(i=n-2;i>=0;--i){
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

int printarr(int * a, int n){
	int i;
	int ok=1;
	for(i=0; i<n; ++i){
		if(printf("%d\n", a[i])==0){
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
	for(i=1; i<argc; ++i){
		int ok=1;
		int complete=0;
		FILE* file=fopen(argv[i],"r");
		if(file==NULL){
			ok=0;
			continue;
		}
		complete++;
		int number;
		if(fscanf(file,"%d", &number)==0){
			ok=0;
		}
		else{
			complete++;
			n++;
		}
		switch(complete){
			case 2:
				insert(ints, n, number);
			case 1:
				fclose(file);
		}
	}
	int ok=1;
	if(printarr(ints, n)==1){
		ok=0;
	}
	free(ints);
	if(ok){
		return 0; //ok
	}
	else{
		return 2; //can't print
	}
}
