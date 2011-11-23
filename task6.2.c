#include <stdio.h>

extern double multiply(double, int);

void print(double f){
	printf("%f", f);
}


int main(){
	print(multiply(1.234,2));
	return 0;
}
