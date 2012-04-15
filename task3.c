#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>

int fd[2];

void sahandler(int signo, siginfo_t *info, void *constext){
//	printf("SIGCHLD!\n");
	int status;
	int success;
	int w=waitpid(-1, &status, WNOHANG);
	if(w==-1){
		perror("Error in waitpid");
		success=0;
	}
	if(w>0 && WIFEXITED(status) && WEXITSTATUS(status)==0){
		success=1;
	}
	else{
		success=0;
	}
	write(fd[0], &success, sizeof(success));
}

int main(int argc, char** argv){
	FILE * input;

//reading URLs

	if(argc>1){
		input=fopen(argv[1], "r");
		if(input==NULL){
			fprintf(stderr, "Couldn't read input file\n");
			return 1;
		}
	}
	else{
		printf("Input URLs:\n");
		input=stdin;
	}
	char url[300];
	int n=0;

//Hanging signal handler

	struct sigaction sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags=SA_SIGINFO;
	sa.sa_sigaction=&sahandler;
	struct sigaction previoussa;
	if(sigaction(SIGCHLD, &sa, &previoussa)<0){
		perror("Failed sigaction");
		fclose(input);
		return 1;
	}

//Creating a pipe

	if(pipe(fd)==-1){
		perror("Failed to create a pipe");
		fclose(input);
		return 1;
	}

	while(fscanf(input, "%s\n", url)>0){
		n++;
		int p=fork();
		if(p==-1){
			perror("Can't fork");
		}
		if(p==0){
			char filename[5];
			sprintf(filename, "%d", n);
			int output=open(filename,O_WRONLY|O_CREAT);
			if(output==-1){
				fprintf(stderr, "Couldn't write to %s\n", filename);
				close(output);
				fclose(input);
				return 1;
			}
			dup2(output, 1);
			
			int null=open("/dev/null",O_WRONLY|O_CREAT);
			if(null!=-1){
				dup2(null, 2);
			}
			execlp("curl", "curl", url, NULL);
		}
	}
	fclose(input);
	int i;
	for(i=0;i<n;++i){
		int success;
		read(fd[1], &success, sizeof(success));
		if(success==1){
			printf("Download successful\n");
		}
		else{
			printf("Download failed\n");
		}
	}
	return 0;
}
