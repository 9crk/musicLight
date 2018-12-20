#include<stdio.h>
#include<math.h>
#include<opencv/cv.h>
#include<opencv/highgui.h>

int main(int argc,char* argv[])
{	
	if(argc != 2){printf("%s file.pcm\n",argv[0]);exit(0);}
	FILE*fp = fopen(argv[1],"rb");
	fseek(fp,0L,SEEK_END);  
    int size=ftell(fp);  

	short *pcm = (short*)malloc(size/2);
	fread(pcm,100,size/100,fp);
	fclose(fp);
	printf("size=%d\n",size/2);
	IplImage* img = cvCreateImage(cvSize(size/2,200),8,1);
	
	
	for(int i=0;i<size/2;i++){
		pcm[i] = pcm[i]/65;
		cvLine(img,cvPoint(i,100),cvPoint(i,100-pcm[i]),CV_RGB(255, 255, 255));		
	}
	cvShowImage("sss",img);
	cvWaitKey(0);
}
