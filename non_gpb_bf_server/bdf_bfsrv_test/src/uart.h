#ifndef UART_H
#define UART_H
int write_uart(char* portname, char* pdara, int size);
int write_read_uart(unsigned char* portname, char* pdara, int write_size, unsigned char* read_buffer, int read_size);
#endif //UART_H

