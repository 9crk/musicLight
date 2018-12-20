#include<stdio.h>
#include<math.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>
extern "C"{
extern void drawEnergy(unsigned int *data,unsigned char* rgb,int led_num);
extern void drawScroll(unsigned int *data,unsigned char* rgb,int led_num);
extern void drawSpectrum(unsigned int *data,unsigned char* rgb,int led_num);

extern void fft(short* data,unsigned int *result);
extern int za_init(int sr, int chn);
extern int za_read(char*buff,int frames);
extern int za_exit();
}

#define PI 3.1415926535897932384626433832795028841971 //定义圆周率值
#define FFT_N	2014
#define MEL 24
#define LED_LEFT 8
#define LED_UP   15
#define LED_RIGHT 8
//频率表达能力为：SPR／FFT_N=

#define THIN	2

static double mel[MEL][FFT_N/2];
static void loadMel(char* filename,double *data,int fft_half_num,int banks)
{
	int i,j;
	FILE*fp = fopen(filename,"rb");
	fread(data,fft_half_num,banks*sizeof(double),fp);
	fclose(fp);
}
void initMel()
{
	int i,j;
	loadMel("mel_100_4000_24_256_8000.dat",*mel,FFT_N/2,MEL);
	for(i=0;i<MEL;i++){
		for(j=0;j<FFT_N/2;j++){
		    //printf("%.03f ",mel[i][j]);	
		}
		//printf("\n");
	}
}
void draw(IplImage*img,unsigned int *data)
{
	int i;
	memset(img->imageData,0,img->width*img->height);
	for(i=0;i<FFT_N/2;i++){
		cvLine(img,cvPoint(i*THIN,200),cvPoint(i*THIN,200 - data[i]/2000),CV_RGB(255, 255, 255));
		//cvRectangleR(img,cvRect(i*div,data[i]/1000,div,(100-data[i]/1000)),CV_RGB(255, 255, 255));
	}
	cvShowImage("ddd",img);
	char ch = cvWaitKey(10);
	if(ch=='c')exit(0);
}
void draw2(IplImage*img,unsigned int *data)
{
	int i,j,sum;
	int div=10;
	int thin = FFT_N/div/2;
	memset(img->imageData,0,img->width*img->height);
	for(i=0;i<div;i++){
		sum=0;
		for(j=0;j<thin;j++)sum+=data[i*thin+j];
		cvLine(img,cvPoint(i*thin,200),cvPoint(i*thin,200 - sum/50000),CV_RGB(0, 0, sum/50000),thin/2);
	}
	cvShowImage("ddd2",img);
	char ch = cvWaitKey(10);
	if(ch=='c')exit(0);
}
void draw3(IplImage*img,unsigned int *data)
{
	int i,j;
	double mel_sum[MEL];
	for(i=0;i<MEL;i++){
		mel_sum[i] = 0;
		for(j=0;j<FFT_N/2;j++){
			mel_sum[i] += data[i]*mel[i][j];
		}
		//mel_sum[i] =mel_sum[i]*mel_sum[i];
		printf("%f ",mel_sum[i]);
	}
	printf("\n");

	memset(img->imageData,0,img->width*img->height);
	for(i=0;i<MEL;i++){
		cvLine(img,cvPoint(i*(img->width/(MEL+1)),img->height),cvPoint(i*(img->width/(MEL+1)),img->height - mel_sum[i]/50000),CV_RGB(0, 0, 255),20);
	}
	cvShowImage("mel",img);
	char ch = cvWaitKey(10);
	if(ch=='c')exit(0);
}

static void showHist(IplImage* img,unsigned char *data,int len,int type)
{
	int i;
	int delta = img->width/len;
	memset(img->imageData,0,img->width*img->height*3);
	for(i=0;i<len;i++){
		if(type){
			cvLine(img,cvPoint(i*delta+delta/2,img->height),cvPoint(i*delta+delta/2,0),CV_RGB(data[i*3],data[i*3+1],data[i*3+2]),delta);
		}else{
			cvLine(img,cvPoint(i*delta+delta/2,img->height),cvPoint(i*delta+delta/2,(img->height - data[i])),CV_RGB(255,255,255),delta);
		}
	}
	cvShowImage("led",img);
}
void draw5(IplImage*img,unsigned int *data)
{
	int i;
	int led_num = 16;
	unsigned char rgb[31*3];
	memset(rgb,0,31*3);
	memset(img->imageData,0,img->width*img->height*3);

	drawSpectrum(data,&rgb[8*3],16);

	showHist(img,&rgb[8*3],16,1);
}
void genBoom(IplImage* img,short *audio)
{
	int i;
	int sum = 0;
	static short lastBuf[FFT_N*2];
	
}
#if 0
int main()
{	
	int i,ret;
	short buff[FFT_N];
	unsigned int result[FFT_N];
	IplImage* img = cvCreateImage(cvSize(FFT_N*THIN/2,200),8,1);
	IplImage* img2 = cvCreateImage(cvSize(FFT_N*THIN/2,200),8,1);
	IplImage* img3 = cvCreateImage(cvSize(FFT_N*THIN/2,200),8,1);
	
	//test
	/*for(i=0;i<FFT_N;i++){
		result[i]=i;
	}
	draw(img,result);
	cvWaitKey(0);
	*///test
	za_init(8000,1);
	while(1){
		ret = za_read((char*)buff,FFT_N);
		if(ret == -1)continue;
		/*for(i=0;i<FFT_N;i++) //给结构体赋值
		{
			buff[i] = (1+sin(11*2*PI*i/FFT_N)+sin(14*2*PI*i/FFT_N))/2*1024;//(sin(11*2*PI*i/FFT_N)+sin(13*2*PI*i/512))/2 * 1024;//(1+sin(11*2*PI*i/FFT_N))/2; 
		}*/
		genBoom(img3,buff);
		fft(buff,result);
		draw(img,result);
		draw2(img2,result);
		//for(i=0;i<FFT_N/2;i++)printf("%d Hz:\t%d\n",i,result[i]/1000);
	}
	za_exit();
}
#else
int main()
{	
	int i,ret;
	short buff[FFT_N];
	unsigned int result[FFT_N];
	unsigned int showMe[FFT_N];
	int inited = 0;
	IplImage* img = cvCreateImage(cvSize(FFT_N*THIN/2,200),8,1);
	IplImage* img2 = cvCreateImage(cvSize(FFT_N*THIN/2,200),8,1);
	IplImage* img3 = cvCreateImage(cvSize(FFT_N*THIN/2,256),8,1);
	IplImage* img4 = cvCreateImage(cvSize(FFT_N*THIN/2,256),8,3);
	
	za_init(8000,1);
	initMel();
	memset(buff,0,FFT_N*2);
	while(1){
		ret = za_read((char*)buff,320);
		if(ret == -1)continue;
		/*for(i=0;i<FFT_N;i++) //给结构体赋值
		{
			buff[i] = (1+sin(11*2*PI*i/FFT_N)+sin(14*2*PI*i/FFT_N))/2*1024;//(sin(11*2*PI*i/FFT_N)+sin(13*2*PI*i/512))/2 * 1024;//(1+sin(11*2*PI*i/FFT_N))/2; 
		}*/
		fft(buff,result);
		if(inited == 0){inited = 1;memcpy(showMe,result,sizeof(int)*FFT_N);}
		else{
			for(i=0;i<FFT_N;i++){
				if(result[i] > showMe[i]){
					showMe[i] = result[i];
				}else{
					if(showMe[i] > 100000)showMe[i]-=100000;
					else showMe[i] = 0;
				}
			}
		}
		draw(img,showMe);
		draw2(img2,result);
		draw5(img4,result);
		//draw3(img3,result);
		//draw4(img4,result);

		//for(i=0;i<FFT_N/2;i++)printf("%d Hz:\t%d\n",i,result[i]/1000);
	}
	za_exit();
}
#endif

#if 0
int main()
{
	#define LEN 40
	int i;
	int three[3];

	unsigned char rgb[LEN/2*3];
	three[0] = 100;
	three[1] = 10;
	three[2] = 250;
	genRGB(three[0],three[1],three[2],rgb,LEN);
	for(i=0;i<LEN/2;i++){
		printf("%02x ",rgb[i*3]);
	}
	printf("\n");
	for(i=0;i<LEN/2;i++){
		printf("%02x ",rgb[i*3+1]);
	}
	printf("\n");
	for(i=0;i<LEN/2;i++){
		printf("%02x ",rgb[i*3+2]);
	}
	printf("\n");
}

#endif
