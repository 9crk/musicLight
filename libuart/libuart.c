/*
 * =====================================================================================
 *
 *       Filename:  api_uart.C
 *
 *    Description:  serial init
 *        Version:  1.0
 *        Created:  2017-10-23
 *         Author:  Eason.Jie
 *   Organization:  mt001
 *
 * =====================================================================================
 */
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include "libuart.h"
#define msleep(x) usleep(x*1000)
static int serial_flowctrl(int fd, int CtrlFlow)
{
    char *buf ;
    struct termios options;
    struct termios oldoptions;

    if( fd < 0 )
        return UART_BADPORT;
    if( CtrlFlow < 0 || CtrlFlow > 0x0F )
	{
		printf("ctrl flow input error, must be 0x0~0xf\n");
		return UART_BADPARM;
	}
    tcgetattr(fd, &options);
    oldoptions = options;
    options.c_cflag &= ~CRTSCTS;
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    if( CtrlFlow & F_HW )
        options.c_cflag |= CRTSCTS;
    if( CtrlFlow & F_SW )
        options.c_iflag |= (IXON | IXOFF);

    tcsetattr(fd, TCSANOW, &options);
    return UART_OK;
}



static int serial_init(int fd,int nspeed,int mode)
{
    struct termios options;
    speed_t speed;
   //mode = (BIT_8 | STOP_1 |P_NONE );
printf("-1\n");
    tcgetattr(fd, &options);
printf("0\n");
    switch( nspeed)
    {
		case BAUD_50:       speed = B50;        break;
		case BAUD_75:       speed = B75;        break;
		case BAUD_110:      speed = B110;       break;
		case BAUD_134:      speed = B134;       break;
		case BAUD_150:      speed = B150;       break;
		case BAUD_300:      speed = B300;       break;
		case BAUD_600:      speed = B600;       break;
		case BAUD_1200:     speed = B1200;      break;
		case BAUD_1800:     speed = B1800;      break;
		case BAUD_2400:     speed = B2400;      break;
		case BAUD_4800:     speed = B4800;      break;
		case BAUD_7200:     speed = B0;         break;
		case BAUD_9600:     speed = B9600;      break;
		case BAUD_19200:    speed = B19200;     break;
		case BAUD_38400:    speed = B38400;     break;
		case BAUD_57600:    speed = B57600;     break;
		case BAUD_115200:   speed = B115200;    break;
		case BAUD_230400:   speed = B230400;    break;
		case BAUD_460800:   speed = B460800;    break;
		case BAUD_921600:   speed = B921600;    break;
		default:            	      speed = B0;         break;
    }
    cfsetispeed(&options, speed);
printf("0.5\n");
    cfsetospeed(&options, speed);

    options.c_cflag &= ~CSIZE;
    switch( mode & 0x03 )
    {
        case BIT_5: options.c_cflag |= CS5;     break;
        case BIT_6: options.c_cflag |= CS6;     break;
        case BIT_7: options.c_cflag |= CS7;     break;
        case BIT_8: options.c_cflag |= CS8;     break;
        default:    options.c_cflag |= CS8;     break;
    }


    switch( mode & 0x04 )
    {
        case STOP_1:    options.c_cflag &= ~CSTOPB; break;
        case STOP_2:    options.c_cflag |= CSTOPB;  break;
        default:        options.c_cflag |= CSTOPB;  break;
    }


    options.c_cflag &= ~(PARENB | PARODD | CMSPAR);
    switch( mode & 0x38 )
    {
        case P_EVEN:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            break;
        case P_ODD:
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            break;
        case P_SPC:
            options.c_cflag |= PARENB;
            options.c_cflag &= ~PARODD;
            options.c_cflag |= CMSPAR;
            break;
        case P_MRK:
            options.c_cflag |= PARENB;
            options.c_cflag |= PARODD;
            options.c_cflag |= CMSPAR;
            break;
        case P_NONE:
        default:
            options.c_cflag &= ~PARENB;
            break;
    }
printf("1\n");
	tcsetattr(fd, TCSANOW, &options);
printf("2\n");

	serial_flowctrl(fd, 0);
printf("3\n");
	return UART_OK;
}
/***************************************************************************
 * Open a uart.
 * Return fd
 */
int libuart_init(char *cSerialName,int speed,int mode)
{
	struct termios options;
	char fname[256];
	int fd;
	printf("000\n");
	if( (fd= open( cSerialName, O_RDWR | O_NOCTTY )) < 0 )
	{
		printf("can't open %s ret=%d\n", cSerialName, fd);
		return UART_OPENFAIL;
	}
	printf("001\n");

	tcgetattr(fd, &options);
	printf("002\n");

	options.c_lflag = 0;
	options.c_iflag = 0;
	options.c_oflag = 0;
	options.c_iflag |= IGNBRK ;
	options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cc[VMIN] = 0;   //less read 1 byte
	options.c_cc[VTIME] = 20;

	tcsetattr(fd, TCSANOW, &options);
	serial_init(fd,speed,mode);
	return fd;
}

/*-----------------------------EOF-----------------------------------*/

int libuart_exit(int fd)
{
	close(fd);
}
