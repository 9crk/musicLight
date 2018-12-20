#include"libuart.h"

int main(int argc,char* argv[])
{
	char data[100];
	printf("start\n");
	int fd = libuart_init(argv[1],BAUD_115200,BIT_8 | STOP_1 |P_NONE ) ;
	printf("open %s = %d\n",argv[1],fd);
	while(1){
		printf("siawefoawoef\n");		
		write(fd,"ssss00000ppp",10);
		sleep(1);
	}
}

