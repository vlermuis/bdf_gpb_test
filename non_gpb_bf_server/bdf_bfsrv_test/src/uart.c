#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "uart.h"

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    //tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag |= PARENB;
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
//    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
//    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
//    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

char data[] = {0x7e, 0xff, 0x10, 0xa, 0x2, 0x8, 0x0, 0x33, 0x19, 0x7e};


int write_uart(char* portname, char* pdara, int size)
{
    //char *portname = "/dev/ttyUSB0";
    int fd;
    int wlen;

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 57600, 8 bits, no parity , 1 stop bit */
    set_interface_attribs(fd, B57600);
    //set_mincount(fd, 0);                /* set to pure timed read */

    /* simple output */
    //wlen = write(fd, "Hello!\n", 7);
    wlen = write(fd, pdara, size);

    if (wlen != size) {
        printf("Error from write: %d, %d\n", wlen, errno);
    }
    tcdrain(fd);    /* delay for output */

    close(portname);

    return 0;
}

int write_read_uart(unsigned char* portname, char* pdara, int write_size, unsigned char* read_buffer, int read_size)
{
    //char *portname = "/dev/ttyUSB0";
    int i;
    int fd;
    int wlen;

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", portname, strerror(errno));
        return -1;
    }
    /*baudrate 57600, 8 bits, no parity , 1 stop bit */
    set_interface_attribs(fd, B57600);
    //set_mincount(fd, 0);                /* set to pure timed read */

    /* simple output */
    //wlen = write(fd, "Hello!\n", 7);
    wlen = write(fd, pdara, write_size);

    if (wlen != write_size) {
        printf("Error from write: %d, %d\n", wlen, errno);
    }
    tcdrain(fd);    /* delay for output */


    int bytes_read = read(fd, read_buffer, read_size);

    printf("\n +----------------------------------+\n");

    for(i=0;i<bytes_read;i++)   /*printing only the received characters*/
        printf("0x%x,",read_buffer[i]);

    printf("\n +----------------------------------+\n\n\n");


    close(portname);

    tcdrain(fd);    /* delay for output */

    return bytes_read;
}

//int uart_receive(char* read_buffer, int size)
//{
//    char read_buffer[32];   /* Buffer to store the data received              */
//    int  bytes_read = 0;    /* Number of bytes read by the read() system call */
//    int i = 0;

//    return read(fd,&read_buffer,size); /* Read the data                   */

//    printf("\n\n  Bytes Rxed %d", bytes_read); /* Print the number of bytes read */
//    printf("\n\n  ");

//    for(i=0;i<bytes_read;i++)   /*printing only the received characters*/
//        printf("%c",read_buffer[i]);

//    printf("\n +----------------------------------+\n\n\n");

//}

//int main()
//{
//    write_uart(&data[0], 10);

//    return 0;
//}