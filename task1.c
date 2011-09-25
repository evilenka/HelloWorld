#include <stdio.h>

int main(){
	if(printf("%s\n", "Hello World!")<12){
		return 1;
	}
	return 0;
}
