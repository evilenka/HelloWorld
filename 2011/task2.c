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
	int capacity=argc-1;
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
		while(fscanf(file,"%d", &x)!=0 && feof(file)==0){
			if(n+1 <= capacity){
				capacity=capacity<<1;
				ints=(int*)realloc(ints, capacity*sizeof(int));
			}
			if(ints!=NULL){
				insert(ints, n, x);
				n++;
			}
			number++;
		}
		if(feof(file)==0){
			ok=0;
			n-=number;
		}
		if(complete){
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
