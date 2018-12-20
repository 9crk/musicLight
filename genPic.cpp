
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include "opencv/cv.h"
#include "opencv/highgui.h"

int main(int argc, char* argv[])
{
        IplImage* img= cvCreateImage(cvSize(1024,768),8,3);
        int i,j;
        for(i=0;i<8;i++){
                cvSetImageROI(img,cvRect(i*120,0,120,768));
                cvSet(img,CV_RGB(pow(2,i),0,0));
                cvResetImageROI(img);
        }
        cvSaveImage("ttt.jpg",img);
}
