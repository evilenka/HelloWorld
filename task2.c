#include <stdio.h>
#include <unistd.h>

int main(){
	while(1){
		int p=fork();
		if(p){
			//parent
		}
		else{
			//child
			int e=setpgid(p, 40000);
			if(e==-1){
				fprintf(stderr, "Can't set pgid\n");
				continue;
			}
			e=getpgid(p);
			if(e==-1){
				fprintf(stderr, "Can't get pgid\n");
				continue;
			}
			if(printf("%d\n", e)<1){
				fprintf(stderr, "Can't print\n");
				continue;
			}
		}
	}
	return 0;
}
