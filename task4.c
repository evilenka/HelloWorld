#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv){
	int pid=fork();
	if(pid==-1){
		perror("Can't fork");
		return 1;
	}
	if(pid>0){
		//computing
		int n=6;
		int f1[36], f2[36], f3[36];
	}
	else{
		//printing
	}
	return 0;
}
