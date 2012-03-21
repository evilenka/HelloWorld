volatile int M1[2048*2048];
volatile int M2[2048*2048];
volatile int M1M2[2048*2048];

/*
	4KB=4B[int]*1024=4B*32*32 => 1 tile - 32*32
	2048*2048=4096*1024 => 4096 tiles => 64*64 tiles

	M(i,j) position:

	i -> i/32 - number of 64-tiles row |
					   | => ((i/32)*64+(j/32))*1024 - tile
	j -> j/32 - number of 64-tiles col |

	i%32 - row number in tile |
				  | => M(i,j)=M[((i/32)*64+(j/32))*1024+i%32*32+j%32]
	j%32 - col number in tile |

	M(i,j)=M[((i>>5)<<6+j>>5)<<10+(i&31)<<5+j&31]

*/

int main(){
	memset(M1,rand()%2048,2048*2048);
	memset(M2,rand()%2048,2048*2048);
	int i;
	for(i=0; i<2048;++i){
		int j;
		for(j=0; j<2048; ++j){
			int ind=((i>>5)<<6+j>>5)<<10+(i&31)<<5+j&31;
			M1M2[ind]=0;
			int k;
			for(k=0; k<2048; ++k){
				M1M2[ind]+=M1[((i>>5)<<6+k>>5)<<10+(i&31)<<5+k&31]*M2[((k>>5)<<6+j>>5)<<10+(k&31)<<5+j&31];
			}
		}
	}
	return M1M2[rand()%2048*2048+rand()%2048];
}
