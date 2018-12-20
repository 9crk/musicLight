#include<stdio.h>
#include<math.h>
#include<stdlib.h>
int main(int argc,char* argv[])
{
    int i,u,num;
    double sigma,y;
    
    if(argc != 3){
       printf("%s num sigma\n",argv[0]);
       return 0;
    }
    num = atoi(argv[1]);
    sigma = atof(argv[2]);
    if(num%2 == 0){
       printf("u must be odd;\n");
       return 0;
    }
    u = num/2;
    for(i=0;i<num;i++){
        y = 1.0/(sqrt(2*3.1415926)*sigma)*exp(-(i-u)*(i-u)/(2*sigma*sigma));
        printf("%f,",y);
    }
    printf("\n");     
}

