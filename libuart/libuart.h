/*
 * =====================================================================================
 *
 *       Filename:  api_uart.h
 *
 *    Description:  serial init
 *        Created:  2017-10-23
 *         Author:  Dr. Eason.Jie
 *   Organization:  mt001
 *
 * =====================================================================================
 */
#ifndef __LIB_UART_H__
#define __LIB_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
//#include <error.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <sys/select.h>
#include <sys/time.h>
#define CMSPAR    010000000000

/*	BAUD rate setting	*/
#define BAUD_50			0x00
#define BAUD_75			0x01
#define BAUD_110		0x02
#define BAUD_134		0x03
#define BAUD_150		0x04
#define BAUD_300		0x05
#define BAUD_600		0x06
#define BAUD_1200		0x07
#define BAUD_1800		0x08
#define BAUD_2400		0x09
#define BAUD_4800		0x0A
#define BAUD_7200		0x0B
#define BAUD_9600		0x0C
#define BAUD_19200		0x0D
#define BAUD_38400		0x0E
#define BAUD_57600		0x0F
#define BAUD_115200 	0x10
#define BAUD_230400 	0x11
#define BAUD_460800 	0x12
#define BAUD_921600 	0x13

#define  B0     0000000         /* hang up */
  #define  B50    0000001
  #define  B75    0000002
  #define  B110   0000003
  #define  B134   0000004
  #define  B150   0000005
  #define  B200   0000006
  #define  B300   0000007
  #define  B600   0000010
  #define  B1200  0000011
  #define  B1800  0000012
  #define  B2400  0000013
  #define  B4800  0000014
  #define  B9600  0000015
  #define  B19200 0000016
  #define  B38400 0000017

  #define  BOTHER 0010000
  #define  B57600 0010001
  #define  B115200 0010002


  #define B3000000        0010003
  #define B6000000        0010004
  #define B12000000       0010005
  #define   B230400       0010006
  #define   B460800       0010007
  #define   B500000       0010010
  #define   B576000       0010011
  #define   B921600       0010012

/*	MODE setting		*/
#define BIT_5			0x00			/* Word length define	    */
#define BIT_6			0x01
#define BIT_7			0x02
#define BIT_8			0x03

#define STOP_1			0x00			/* Stop bits define	        */
#define STOP_2			0x04

#define P_EVEN			0x18			/* Parity define	        */
#define P_ODD			0x08
#define P_SPC			0x38
#define P_MRK			0x28
#define P_NONE			0x00

/* Flow control setting 	*/
#define F_NONE          0x00            /* No flow control          */
#define F_CTS			0x01			/* CTS flow control         */
#define F_RTS			0x02			/* RTS flow control         */
#define F_HW			0x03			/* Hardware flowcontrol     */
#define F_TXSW			0x04			/* Tx XON/XOFF flow control */
#define F_RXSW			0x08			/* Rx XON/XOFF flow control */
#define F_SW			0x0C			/* Sofeware flowcontrol     */


/* error code */
#define UART_OK				0
#define UART_BADPORT			-1		/* no such port or port not opened          */
#define UART_OUTCONTROL		-2		/* can't control the board                  */
#define UART_NODATA			-4		/* no data to read or no buffer to write    */
#define UART_OPENFAIL		-5		/* no such port or port has be opened       */
#define UART_RTS_BY_HW		-6      /* RTS can't set because H/W flowctrl       */
#define UART_BADPARM			-7		/* bad parameter                            */
#define UART_WIN32FAIL		-8		/* call win32 function fail, please call    */
									/* GetLastError to get the error code       */
#define UART_BOARDNOTSUPPORT -9		/* Does not support this board              */
#define UART_FAIL			-10		/* PComm function run result fail           */
#define UART_ABORTWRITE		-11		/* write has blocked, and user abort write  */
#define UART_WRITETIMEOUT    -12 	/* write timeoue has happened               */
#define UART_SETERROR		-13       /*set the port error*/
#define SET_BAUNDNULL		-14      /*the speed is no exist*/




int libuart_init(char *cSerialName,int speed,int mode) ;
int libuart_exit(int fd);
#endif

/*-----------------------------EOF-----------------------------------*/
