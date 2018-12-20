#include<stdio.h>
void loadMel(char* filename,double *data,int fft_half_num,int banks)
{
	int i,j;
	FILE*fp = fopen(filename,"rb");
	fread(data,fft_half_num,banks*sizeof(double),fp);
	fclose(fp);
	/*for(i=0;i<banks;i++){
		for(j=0;j<fft_half_num;j++){
			printf("%03f ",data[i*fft_half_num+j]);
		}
		printf("\n");
	}*/
}
double mel[13][256];
int main(int argc,char* argv[])
{
	int i,j;
	loadMel(argv[1],*mel,256,13);
	for(i=0;i<13;i++){
		for(j=0;j<256;j++){
		    printf("%.03f ",mel[i][j]);	
		}
		printf("\n");
	}
}
