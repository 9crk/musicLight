#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//hz to mel: Mel(f) = 2595*log10(1+f/700) 
//mel to hz: F(mel) = 700.0 * (10**(mel / 2595.0)) - 700.0
int main(int argc,char* argv[])
{
	int i,j;
	int f_min,f_max,Nbank,fft_half_len,sampleRate;
	double m_min,m_max,delta;
	double deltaFreq;
	int thisFreq;
	
	double freq[100];//储存每个mel的中心频点，一共使用Nbank+2个，为了存储左右边缘
	double melbank_array[80000];//临时储存一个三角滤波器的值
	double freq_array[80000];//储存每个FFT结果点所代表的频率

	FILE* fp;
	char filename[100];
	if(argc != 6){
		printf("%s f_min f_max Nbank fft_half_len sampleRate\n",argv[0]);
		return 0;
	}
	f_min = atoi(argv[1]);
	f_max = atoi(argv[2]);	
	Nbank = atoi(argv[3]);
	fft_half_len = atoi(argv[4]);
	sampleRate = atoi(argv[5]);

	if(fft_half_len>80000){printf("error fft len\n");return 0;}

	printf("f_min = %d\n",f_min);
	printf("f_max = %d\n",f_max);
	printf("Nbank = %d\n",Nbank);
	m_min = 2595*log10(1+f_min/700.0);
	m_max = 2595*log10(1+f_max/700.0);
	delta = (m_max - m_min)/(Nbank+1);//这里加1是为了计算每个三角形的左右边缘

	printf("center freq:");
	for(i=0;i<(Nbank+2);i++){//这里加2是为了计算三角形的左右边缘，左右边+中点，一共Nbank+2个频点
		freq[i] = 700.0*pow(10,(m_min + i*delta)/2595.0) - 700.0;
		printf("%f ",freq[i]);
	}
	printf("\n");
	deltaFreq = (sampleRate/2.0)/(fft_half_len*1.0);
	for(j=0;j<fft_half_len;j++){
		freq_array[j] = j*deltaFreq;
	}
	sprintf(filename,"mel_%d_%d_%d_%d_%d.dat",f_min,f_max,Nbank,fft_half_len,sampleRate);
	fp = fopen(filename,"wb");
	for(i=0;i<Nbank;i++){
		for(j=0;j<fft_half_len;j++){
			//制造一个三角波滤波器
			if(freq_array[j] < freq[i]){//____
				melbank_array[j] = 0.0;
			}else if(freq_array[j] > freq[i]){
				if(freq_array[j] < freq[i+1]){//上升
					melbank_array[j] = (1.0/(freq[i+1] - freq[i])*(freq_array[j]-freq[i]));
				}else if(freq_array[j] > freq[i+1]){
					if(freq_array[j] < freq[i+2]){//下降
						melbank_array[j] = (-1.0/(freq[i+2] - freq[i+1])*(freq_array[j]-freq[i+2]));
					}else{//_____
						melbank_array[j] = 0.0;
					}
				}
			}
			printf("%.03f ",melbank_array[j]);
			fwrite(&melbank_array[j],1,sizeof(double),fp);
		}
		printf("\n");
	}
	fclose(fp);
	return 0;
}
/*
void loadMel(char* filename,double *data,int fft_half_num,int banks)
{
	int i,j;
	FILE*fp = fopen(filename,"rb");
	fread(data,fft_half_num,banks*sizeof(double),fp);
	fclose(fp);
	for(i=0;i<banks;i++){
		for(j=0;j<fft_half_num;j++){
			printf("%03f ",data[i*fft_half_num+j]);
		}
		printf("\n");
	}
}
*/
