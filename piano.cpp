#include<stdio.h>
#include<math.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>
extern "C"{
extern void fft(short* data,unsigned int *result);
extern int za_init(int sr, int chn);
extern int za_read(char*buff,int frames);
extern int za_exit();
}

#define PI 3.1415926535897932384626433832795028841971 //定义圆周率值
#define FFT_N	2048
//频率表达能力为：SPR／FFT_N=

#define THIN	1
IplImage* piano = NULL;
IplImage* key = NULL;

void draw(IplImage*img,unsigned int *data)
{
	int i;
	int ww = img->width/36;
	//memset(img->imageData,0,img->width*img->height*img->nChannels);
	cvSet(img,CV_RGB(0,0,0));
	cvSet(key,CV_RGB(0,0,0));
	cvCopy(piano,key);
	for(i=0;i<FFT_N/2;i++){
		if(i<480){
			if(data[i] > 200000){
				cvRectangleR(key,cvRect((i/(480/36))*ww,0,ww,280),CV_RGB(0,0,255),2);
			}
		}
		cvLine(img,cvPoint(i*THIN,300),cvPoint(i*THIN,300 - data[i]/2000),CV_RGB(255, 0, 0));
		//cvRectangleR(img,cvRect(i*div,data[i]/1000,div,(100-data[i]/1000)),CV_RGB(255, 255, 255));
	}
	cvShowImage("ddd",img);
	cvShowImage("piano",key);
	char ch = cvWaitKey(10);
	if(ch=='c')exit(0);
}

int main()
{	
	int i,ret;
	short buff[FFT_N];
	unsigned int result[FFT_N];
	unsigned int showMe[FFT_N];
	int inited = 0;
	IplImage* img = cvCreateImage(cvSize(FFT_N*THIN/2,300),8,3);
	//IplImage* copy=cvCloneImage(img);

	piano = cvLoadImage("piano.jpg");
	key = cvCloneImage(piano);
	//piano = cvLoadImage("/Users/zhouhua/Pictures/piano.png");
	//cvResize(piano,copy);
	//cvSaveImage("piano.jpg",copy);
	//return 0;
	za_init(8000,1);
	memset(buff,0,FFT_N*2);
	while(1){
		ret = za_read((char*)buff,2048);
		if(ret == -1)continue;
		/*for(i=0;i<FFT_N;i++) //给结构体赋值
		{
			buff[i] = (1+sin(11*2*PI*i/FFT_N)+sin(14*2*PI*i/FFT_N))/2*1024;//(sin(11*2*PI*i/FFT_N)+sin(13*2*PI*i/512))/2 * 1024;//(1+sin(11*2*PI*i/FFT_N))/2; 
		}*/
		fft(buff,result);
		if(inited == 0){inited = 1;memcpy(showMe,result,sizeof(int)*FFT_N);}
		else{
			for(i=0;i<FFT_N;i++){
				//if(result[i] > showMe[i]){
					showMe[i] = result[i];
				//}else{
				//	if(showMe[i] > 100000)showMe[i]-=100000;
				//	else showMe[i] = 0;
				//}
			}
		}
		draw(img,showMe);
		//for(i=0;i<FFT_N/2;i++)printf("%d Hz:\t%d\n",i,result[i]/1000);
	}
	za_exit();
}

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
