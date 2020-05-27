#ifndef TEST_FUNCS_H
#define TEST_FUNCS_H

#define bool _Bool
#define true 1
#define false 0


typedef unsigned char  uchar;  /*<! unsigned char   8 bits [0,255] */
typedef unsigned char  uint8;  /*<! unsigned char   8 bits [0,255] */
typedef signed   char  int8;   /*<! signed   char   8 bits [-128,127] */
typedef unsigned short uint16; /*<! unsigned short 16 bits [0,65535]*/
typedef signed   short int16;  /*<! signed   short 16 bits [-32768,32767]*/
typedef unsigned long  uint32; /*<! unsigned long  32 bits [0,4294967295]*/
typedef signed   long  int32;  /*<! signed   long  32 bits [-2147483648, 2147483647]*/


void bf_ver_info_cmd_test(char* portname);

#endif //TEST_FUNCS_H