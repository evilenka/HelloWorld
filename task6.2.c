#include <stdio.h>

void multiply(double, int);

extern void writedouble(double f){
	printf("%f", f);
}


int main(){
	multiply(1.234,2);
	return 0;
}
