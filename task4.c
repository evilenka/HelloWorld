#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <poll.h>

void recompute(int act, int work, int shm){
	int i=0;
	for(i=0; i<400; ++i){
		int x=i%20;
		int y=i/20;
		if(x==0||y==0||x==19||y==19){
			continue;
		}
		int start=(y-1)*20+x-1;
		char buf[3], cell;
		int sum=0;
		for(; start<=(y+1)*20+x-1; start+=20){
			lseek(shm, act*400+start, SEEK_SET);
			read(shm, &buf, 3);
			sum+=(int)(buf[0]+buf[1]+buf[2]);
			if(start==y*20+x-1){
				cell=buf[1];
				sum-=(int)cell;
			}
		}
		//printf("%d, %d: sum=%d, val=%d\n", x, y, sum, cell);
		lseek(shm, work*400+20*y+x ,SEEK_SET);
		if(cell){
			if(sum<2||sum>3){
				cell=0;
			}
		}
		else{
			if(sum==3){
				cell=1;
			}
		}
		write(shm, &cell, 1);;
	}
}

void print(int act, int shm){
	lseek(shm, act*400, SEEK_SET);
	char buf[400];
	read(shm, buf, 400);
	int i, j;
	printf("**********\n");
	for(i=0; i<20; ++i){
		for(j=0; j<20; ++j){
			char c=buf[20*i+j];
			if(c){
				printf("+");
			}
			else{
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("**********\n");
}

int main(int argc, char** argv){
	int n=20, act=-1, work=-1, printed=-1, printflag=0;
	int fd=shm_open("gameoflife", O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd==-1){
		perror("Can't shm_open");
		return 1;
	}
	int c2p[2], p2c[2];
	if(pipe(c2p)==-1){
		perror("Can't make pipe");
		close(fd);
		shm_unlink("gameoflife");
		return 1;
	}
	if(pipe(p2c)==-1){
		perror("Can't make pipe");
		close(fd);
		shm_unlink("gameoflife");
		close(c2p[0]);
		close(c2p[1]);
		return 1;
	}
	if(ftruncate(fd, sysconf(_SC_PAGESIZE))==-1){
		perror("Can't ftruncate");
		close(fd);
		shm_unlink("gameoflife");
		close(c2p[0]);
		close(c2p[1]);
		close(p2c[0]);
		close(p2c[1]);
		return 1;
	}
	int pid=fork();
	if(pid==-1){
		perror("Can't fork");
		close(fd);
		shm_unlink("gameoflife");
		close(c2p[0]);
		close(c2p[1]);
		close(p2c[0]);
		close(p2c[1]);
		return 1;
	}
	if(pid>0){
		//computing
		char buf[400]={
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,1,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
		};
		write(fd,buf,400);
		write(fd,buf,400);
		write(fd,buf,400);
		act=0;
		work=1;
		int flags=fcntl(p2c[0], F_GETFL, 0);
		fcntl(p2c[0], F_SETFL, flags|O_NONBLOCK);
		while(1){
			write(c2p[1], &act, 4);
//			print(act, fd);
			poll(NULL,0,1000);
			recompute(act, work, fd);
			int x=act;
			act=work;
			work=x;
			if(read(p2c[0], &printflag, 1)>0){
				if(printflag==1){
					printed=act;
					switch(printed){
						case 0:
							if(work==1){
								work=2;
							}
							else{
								work=1;
							}
							break;
						case 1:
							if(work){
								work=0;
							}
							else{
								work=2;
							}
							break;
						case 2:
							if(work){
								work=0;
							}
							else{
								work=1;
							}
					}
				}
			}
		}
	}
	else{
		//printing
		int flags=fcntl(c2p[0], F_GETFL, 0);
		fcntl(c2p[0], F_SETFL, flags|O_NONBLOCK);
		struct pollfd pollfds[1];
		pollfds[0].fd=c2p[0];
		pollfds[0].events=POLLIN;
		pollfds[1].fd=0;
		pollfds[1].fd=POLLIN;
		int pollres;
		while(pollres=poll(pollfds, 2, 3000)>0){
			char input[10];
			read(c2p[0], &act, 1);
			if(read(0, input, 10)>0){
				if(input[0]==10){
					printflag=1;
					write(p2c[1], &printflag, 4);
					print(act, fd);
					printflag=0;
					write(p2c[1], &printflag, 4);
				}
			}
		}
		if(pollres==-1){
			perror("Printer can't poll");
			shm_unlink("gameoflife");
			close(fd);
			close(p2c[0]);
			close(p2c[1]);
			close(c2p[0]);
			close(c2p[1]);
			return 1;
		}
	}
	shm_unlink("gameoflife");
	close(fd);
	close(p2c[0]);
	close(p2c[1]);
	close(c2p[0]);
	close(c2p[1]);
	return 0;
}
