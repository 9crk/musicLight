#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>


#include <sys/time.h>
static int t1,t2;
static long z_time()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}
static void time_record()
{
    t1 = z_time();
}
static void time_report(char *str)
{
    t2 = z_time();
    //__android_log_print(ANDROID_LOG_ERROR,"TIME_USE","%s %ld us",str,t2-t1);
    printf("%s,usetime: %u us\n",str,t2-t1);
}



#define PI 3.1415926535897932384626433832795028841971 //定义圆周率值
#define FFT_N 2048 //定义福利叶变换的点数 512 point 

double cosTab[FFT_N];
double sinTab[FFT_N];
double windowTab[FFT_N];

int bTabInited = 0;
struct compx {double real,imag;}; //定义一个复数结构
struct compx s[FFT_N]; //FFT输入和输出：从S[1]开始存放，根据大小自己定义

/*******************************************************************
函数原型：struct compx EE(struct compx b1,struct compx b2) 
函数功能：对两个复数进行乘法运算
输入参数：两个以联合体定义的复数a,b
输出参数：a和b的乘积，以联合体的形式输出
*******************************************************************/
struct compx EE(struct compx a,struct compx b) 
{
	struct compx c;
	c.real=a.real*b.real-a.imag*b.imag;
	c.imag=a.real*b.imag+a.imag*b.real;
	return(c);
}
/*****************************************************************
函数原型：void initTab()
函数功能：初始化sin和cos表
输入参数：
*****************************************************************/
static void fft_init()
{
	int i;
	for(i=0;i<FFT_N;i++){
		cosTab[i] = cos(PI/i);
		sinTab[i] = sin(PI/i);
		windowTab[i]= 0.54-0.46*cos(2*i*PI/(FFT_N-1));
	}
}
/*****************************************************************
函数原型：void FFT(struct compx *xin,int N)
函数功能：对输入的复数组进行快速傅里叶变换（FFT）
输入参数：*xin复数结构体组的首地址指针，struct型
*****************************************************************/
void FFT(struct compx *xin)
{
	int f,m,nv2,nm1,i,k,l,j=0;
	struct compx u,w,t;
	//变址运算，即把自然顺序变成倒位序，采用雷德算法
	nv2=FFT_N/2; 
	nm1=FFT_N-1; 
	for(i=0;i<nm1;i++){
		if(i<j){ //如果i<j,即进行变址
			t=xin[j];
			xin[j]=xin[i];
			xin[i]=t;
		}
		k=nv2; //求j的下一个倒位序
		while(k<=j){ //如果k<=j,表示j的最高位为1 
			j=j-k; //把最高位变成0
			k=(k>>1); //k/2，比	较次高位，依次类推，逐个比较，直到某个位为0
		}
		j=j+k; //把0改为1
	}
	//三重交换蝶形运算
	{
		int le,lei,ip; //FFT运算核，使用蝶形运算完成FFT运算
		f=FFT_N;
		for(l=1;(f=f/2)!=1;l++);//计算l的值，即计算蝶形级数
		for(m=1;m<=l;m++){ // 控制蝶形结级数
		    //m表示第m级蝶形，l为蝶形级总数l=log（2）N
			le=2<<(m-1); //le蝶形结距离，即第m级蝶形的蝶形结相距le点
			lei=le/2; //同一蝶形结中参加运算的两点的距离
			u.real=1.0; //u为蝶形结运算系数，初始值为1
			u.imag=0.0;
			//w.real=cos(PI/lei); //w为系数商，即当前系数与前一个系数的商
			//w.imag=-sin(PI/lei);
			w.real=cosTab[lei];//cos(PI/lei); //w为系数商，即当前系数与前一个系数的商
			w.imag=-sinTab[lei];//sin(PI/lei);
			for(j=0;j<=lei-1;j++){//控制计算不同种蝶形结，即计算系数不同的蝶形结
				for(i=j;i<=FFT_N-1;i=i+le){ //控制同一蝶形结运算，即计算系数相同蝶形结
					ip=i+lei; //i，ip分别表示参加蝶形运算的两个节点
					t=EE(xin[ip],u); //蝶形运算，详见公式
					xin[ip].real=xin[i].real-t.real;
					xin[ip].imag=xin[i].imag-t.imag;
					xin[i].real=xin[i].real+t.real;
					xin[i].imag=xin[i].imag+t.imag;
				}
				u=EE(u,w); //改变系数，进行下一个蝶形运算
			}
		}
	}
}

void fft(short* data,unsigned int *result)
{
	int i;
	if(bTabInited==0){
		fft_init();
		bTabInited = 1;
	}
	for(i=0;i<FFT_N;i++)
	{
		s[i].real=data[i]*windowTab[i];
		s[i].imag= 0;
	}
	FFT(s);
	for(i=0;i<FFT_N;i++){
		result[i]=sqrt(s[i].real*s[i].real+s[i].imag*s[i].imag);
	}
}
/************************************************************
函数原型：void main() 
函数功能：测试FFT变换，演示函数使用方法
输入参数：无
输出参数：无
************************************************************/
#if 0
int main() 
{ 
	int i;
	short buff[512];

	fft_init();
	//产生一个信号sin(n*2*PI*i/FFT_N)，放在实部
	for(i=0;i<FFT_N;i++) //给结构体赋值
	{
		buff[i] = (1+sin(11*2*PI*i/FFT_N)+sin(13*2*PI*i/FFT_N))/2 * 1024;
	}
	for(i=0;i<FFT_N;i++) //给结构体赋值
	{
		s[i].real= (int)(1+sin(11*2*PI*i/FFT_N)+sin(13*2*PI*i/FFT_N))/2 * 1024;;//buff[i];//(1+sin(11*2*PI*i/FFT_N))/2; 
		s[i].imag=0; //虚部为0
	}
time_record();
	FFT(s); //进行快速福利叶变换
time_report("all");
	//变换后的值需要开根号
	for(i=0;i<FFT_N;i++) //求变换后结果的模值，存入复数的实部部分
		s[i].real=sqrt(s[i].real*s[i].real+s[i].imag*s[i].imag);
	//显示出来
	for(i=0;i<FFT_N;i++)
		printf("%d Hz:\t%d\n",i,(int)s[i].real);
}
#endif


#if 0
#define FILTER_NUM 11
const double gussian_filter[]={0.045662,0.060493,0.075284,0.088016,0.096667,0.099736,0.096667,0.088016,0.075284,0.060493,0.045662};
#endif

#if 1
#define FILTER_NUM 5
const double gussian_filter[]={0.013991,0.101971,0.698942,0.101971,0.013991};
#endif

static int gDrawSpectrum_inited = 0;
static unsigned int lastSpectrum[100];
static int gDrawEnergy_inited=0;
static unsigned char rgb_store[100*3];
static int gDrawScroll_inited = 0;
static int gWavUp,gWavDown;


static double getBand(int i,unsigned char* data,int all)
{
	if(i < 0)return data[0];
	else if(i >=all){
		return 0.0;
	}else return (double)data[i];
}
static void genRGB(int r,int g,int b,unsigned char* rgb,int len)
{
	int i,j;
	double ret;
	unsigned char* band = (unsigned char*)malloc(len/2);
	r = (int)(r*(len/2.0/255.0)+0.5);
	g = (int)(g*(len/2.0/255.0)+0.5);
	b = (int)(b*(len/2.0/255.0)+0.5);
	
	for(i=0;i<len/2;i++){
		if(i < r)band[i] = 255;
		else band[i] = 0;
		//printf("%02x ",band[i]);
	}//printf("\n");
	for(i=0;i<len/2;i++){
		ret = 0;
		for(j=0;j<=FILTER_NUM;j++){
			ret += gussian_filter[j]*getBand(i+j-(FILTER_NUM/2),band,len/2);			
		}
		ret/=(FILTER_NUM*1.0);
		rgb[i*3] = (unsigned char)ret;
	}
	for(i=0;i<len/2;i++){
		if(i < g)band[i] = 255;
		else band[i] = 0;
		//printf("%02x ",band[i]);
	}//printf("\n");
	for(i=0;i<len/2;i++){ 
		ret = 0;
		for(j=0;j<=FILTER_NUM;j++){
			ret += gussian_filter[j]*getBand(i+j-(FILTER_NUM/2),band,len/2);			
		}
		ret/=(FILTER_NUM*1.0);
		rgb[i*3+1] = (unsigned char)ret;
	}
	for(i=0;i<len/2;i++){
		if(i < b)band[i] = 255;
		else band[i] = 0;
		//printf("%02x ",band[i]);
	}//printf("\n");
	for(i=0;i<len/2;i++){ 
		ret = 0;
		for(j=0;j<=FILTER_NUM;j++){
			ret += gussian_filter[j]*getBand(i+j-(FILTER_NUM/2),band,len/2);			
		}
		ret/=(FILTER_NUM*1.0);
		rgb[i*3+2] = (unsigned char)ret;
	}
	for(i=(len/2);i<len;i++){
		rgb[i*3] = rgb[(i-(len/2))*3];
		rgb[i*3+1] = rgb[(i-(len/2))*3+1];
		rgb[i*3+2] = rgb[(i-(len/2))*3+2];	
	}
	for(i=(len/2-1);i>=0;i--){
		rgb[i*3] = rgb[(len-i-1)*3];
		rgb[i*3+1] = rgb[(len-i-1)*3+1];
		rgb[i*3+2] = rgb[(len-i-1)*3+2];	
	}
	free(band);
}
static void genRGB_ex(int r,int g,int b,unsigned char* rgb,int len)
{
	int i;
	unsigned char *t_rgb = (unsigned char*)malloc(len*3);
	genRGB(r,g,b,t_rgb,len);
	if(gDrawEnergy_inited == 0){
		gDrawEnergy_inited = 1;
		memcpy(rgb_store,t_rgb,len*3);
		memcpy(rgb,t_rgb,len*3);
	}else{
		//memcpy(rgb_store,t_rgb,len*3);
		for(i=0;i<len;i++){
			if(t_rgb[i*3] > rgb_store[i*3]){
				rgb_store[i*3] = t_rgb[i*3];
			}else{
				if(rgb_store[i*3] > 2)rgb_store[i*3]-=3;
			}
			if(t_rgb[i*3+1] > rgb_store[i*3+1]){
				rgb_store[i*3+1] = t_rgb[i*3+1];
			}else{
				if(rgb_store[i*3+1] > 2)rgb_store[i*3+1]-=3;
			}
			if(t_rgb[i*3+2] > rgb_store[i*3+2]){
				rgb_store[i*3+2] = t_rgb[i*3+2];
			}else{
				if(rgb_store[i*3+2] > 2)rgb_store[i*3+2]-=3;
			}
			if(rgb_store[i*3] < 10)rgb_store[i*3]=0;
			if(rgb_store[i*3+1] < 10)rgb_store[i*3+1]=0;
			if(rgb_store[i*3+2] < 10)rgb_store[i*3+2]=0;			
		}
		memcpy(rgb,rgb_store,len*3);
	}
	free(t_rgb);
}
void drawEnergy(unsigned int *data,unsigned char* rgb,int led_num)
{
	int i;
	int three[3];

	gDrawSpectrum_inited = 0;
	gDrawScroll_inited = 0;

	three[0] = 0;
	three[1] = 0;
	three[2] = 0;

	for(i=0;i<FFT_N/2/6;i++){
		three[0]+=data[i];
	}
	for(i=FFT_N/2/6;i<FFT_N/2/2;i++){
		three[1]+=data[i];
	}
	for(i=FFT_N/2/2;i<FFT_N/2;i++){
		three[2]+=data[i];
	}
	three[0]/=(FFT_N/2/2);
	three[1]/=(FFT_N/2/2);
	three[2]/=(FFT_N/2/2);
	
	three[0]/=2000;
	three[1]/=2000;
	three[2]/=2000;


	three[0] = three[0]*three[0];
	three[1] = three[1]*three[1];
	three[2] = three[2]*three[2];
	
	printf("r=%d g=%d b=%d\n",three[0],three[1],three[2]);
	genRGB_ex(three[0],three[1],three[2],rgb,led_num);
	
	for(i=0;i<led_num;i++){
		printf("%02x ",rgb[i*3]);
	}
	printf("\n");
	for(i=0;i<led_num;i++){
		printf("%02x ",rgb[i*3+1]);
	}
	printf("\n");
	for(i=0;i<led_num;i++){
		printf("%02x ",rgb[i*3+2]);
	}
	printf("\n");
}
void drawSpectrum(unsigned int *data,unsigned char* rgb,int len)
{
	int i,j;
	unsigned int spectrum[100];
	int div = FFT_N/2/len;
	
	gDrawEnergy_inited = 0;
	gDrawScroll_inited = 0;

	for(i=0;i<FFT_N/2/div;i++){
		spectrum[i] = 0;
		for(j=0;j<div;j++){
			spectrum[i] += data[i*div + j];
		}
		spectrum[i] /= div;
		spectrum[i] /=10000;
		spectrum[i] = spectrum[i]*spectrum[i];
		if(spectrum[i]>255)spectrum[i]=255;
		printf("%02x ",spectrum[i]);
	}
	printf("\n");
	
	if(gDrawSpectrum_inited == 0){
		gDrawSpectrum_inited = 1;
		memcpy(lastSpectrum,spectrum,sizeof(unsigned int)*len);
	}else{
		for(i=0;i<FFT_N/2/div;i++){
			if(spectrum[i] > lastSpectrum[i])lastSpectrum[i] = spectrum[i];
			else{
				if(lastSpectrum[i] > 4)lastSpectrum[i]-=5;
			}
		}
	}
	for(i=0;i<FFT_N/2/div;i++){
		rgb[i*3] = lastSpectrum[i];
		rgb[i*3+1] = 0;
		rgb[i*3+2] = 0;			
	}

	for(i=(len/2);i<len;i++){
		rgb[i*3] = rgb[(i-(len/2))*3];
		rgb[i*3+1] = rgb[(i-(len/2))*3+1];
		rgb[i*3+2] = rgb[(i-(len/2))*3+2];	
	}
	for(i=(len/2-1);i>=0;i--){
		rgb[i*3] = rgb[(len-i-1)*3];
		rgb[i*3+1] = rgb[(len-i-1)*3+1];
		rgb[i*3+2] = rgb[(len-i-1)*3+2];	
	}

	for(i=0;i<len;i++){
		printf("%02x ",rgb[i*3]);
	}
	printf("\n");
	for(i=0;i<len;i++){
		printf("%02x ",rgb[i*3+1]);
	}
	printf("\n");
	for(i=0;i<len;i++){
		printf("%02x ",rgb[i*3+2]);
	}
	printf("\n");
}
void drawScroll(unsigned int *data,unsigned char* rgb,int len)//16 led
{
	int i,j;
	unsigned int spectrum[100];
	int div = FFT_N/2/len;
	
	gDrawEnergy_inited = 0;
	gDrawSpectrum_inited = 0;

	for(i=0;i<FFT_N/2/div;i++){
		spectrum[i] = 0;
		for(j=0;j<div;j++){
			spectrum[i] += data[i*div + j];
		}
		spectrum[i] /= div;
		spectrum[i] /=10000;
		spectrum[i] = spectrum[i]*spectrum[i];
		if(spectrum[i]>255)spectrum[i]=255;
		printf("%02x ",spectrum[i]);
	}
	printf("\n");
	
	if(gDrawScroll_inited == 0){
		gDrawScroll_inited = 1;
		memcpy(lastSpectrum,spectrum,sizeof(unsigned int)*len);
	}else{
		//shifting
		for(i=FFT_N/2/div;i>0;i--){
			lastSpectrum[i] = lastSpectrum[i-1];
		}
		lastSpectrum[0] = 0;
		for(i=0;i<FFT_N/2/div;i++){
			if(spectrum[i] > lastSpectrum[i])lastSpectrum[i] = spectrum[i];
			else{
				if(lastSpectrum[i] > 10)lastSpectrum[i]-=11;
			}
		}
	}
	for(i=0;i<FFT_N/2/div;i++){
		if(lastSpectrum[i] > 100)rgb[i*3] = lastSpectrum[i];
		else rgb[i*3] = 0;
		rgb[i*3+1] = 0;
		rgb[i*3+2] = 0;			
	}
	for(i=(len/2);i<len;i++){
		rgb[i*3] = rgb[(i-(len/2))*3];
		rgb[i*3+1] = rgb[(i-(len/2))*3+1];
		rgb[i*3+2] = rgb[(i-(len/2))*3+2];	
	}
	for(i=(len/2-1);i>=0;i--){
		rgb[i*3] = rgb[(len-i-1)*3];
		rgb[i*3+1] = rgb[(len-i-1)*3+1];
		rgb[i*3+2] = rgb[(len-i-1)*3+2];	
	}
	for(i=0;i<len;i++){
		printf("%02x ",rgb[i*3]);
	}
	printf("\n");
	for(i=0;i<len;i++){
		printf("%02x ",rgb[i*3+1]);
	}
	printf("\n");
	for(i=0;i<len;i++){
		printf("%02x ",rgb[i*3+2]);
	}
	printf("\n");
}