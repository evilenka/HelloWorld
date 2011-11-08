volatile int M1[2048][2048];
volatile int M2[2048][2048];
volatile int M1M2[2048][2048];

int main(){
	memset(M1,rand()%2048,2048*2048);
	memset(M2,rand()%2048,2048*2048);
	int i;
	for(i=0; i<2048;++i){
		int j;
		for(j=0; j<2048; ++j){
			M1M2[i][j]=0;
			int k;
			for(k=0; k<2048; ++k){
				M1M2[i][j]+=M1[i][k]*M2[k][j];
			}
		}
	}
	return M1M2[rand()%2048][rand()%2048];
}
