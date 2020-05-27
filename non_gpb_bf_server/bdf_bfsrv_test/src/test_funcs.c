#include <stdio.h>
#include <string.h>

#include "test_funcs.h"
#include "uart.h"

typedef enum
{
    SIGN_IDX,
    SIGN2_IDX,
    SEQ_IDX,
    CMD_IDX,
    DATA_SIZE_IDX,
    DATA_START_IDX,
}eBFMsgIndx;

#define DMSG_MIN_SIZE   0x07
#define START_SIGN      0x55  // package signature
#define START_SIGN2      0xAA  // package signature

typedef enum
{
    ver_info_cmd                = 0x01,
    get_power_cmd               = 0x02,
    set_power_cmd               = 0x03,
    get_volume_cmd              = 0x04,
    set_volume_nofade_cmd       = 0x05,
    set_volume_fade_cmd         = 0x06,
    get_mute_cmd                = 0x07,
    set_mute_cmd                = 0x08,
    get_audio_mode_cmd          = 0x09,
    set_audio_mode_cmd          = 0x0A,
    get_audio_source_cmd        = 0x0B,
    set_audio_source_cmd        = 0x0C,
    get_dsp_parameters_cmd      = 0x0D,
    set_dsp_parameters_cmd      = 0x0E,
    get_audio_signal_level_cmd  = 0x0F,
    get_ntc_values_cmd          = 0x10,
    set_dsp_tone_touch_ceff_cmd = 0x11,
}eBFcmds;


static uint8 goutbuffer[32];
static uint8 ginbuffer[32];

static uint8 seq = 0;
/*______________________________________________________________________________________________________*/
void dispbuff(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("0x%x, ",ginbuffer[i]);
    }
    printf("\n");
}
void dispbuff0(int n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("0x%x, ",goutbuffer[i]);
    }
    printf("\n");
}

/********************************************************************************************************/
static const unsigned short crc16tab[256] =
        {
                0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
                0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
                0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
                0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
                0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
                0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
                0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
                0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
                0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
                0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
                0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
                0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
                0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
                0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
                0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
                0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
                0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
                0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
                0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
                0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
                0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
                0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
                0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
                0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
                0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
                0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
                0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
                0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
                0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
                0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
                0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
                0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
        };

unsigned int crc16(const unsigned char* buf, int len)
{
    unsigned counter;
//	unsigned int crc = 0;
    unsigned int crc = 0xFFFF;
    for( counter = 0; counter < len; counter++)
        crc = (crc<<8) ^ crc16tab[((crc>>8) ^ *(char *)buf++)&0x00FF];
    return crc;
}
static unsigned char get_msb( unsigned short x ){ return (x>>8)&0xFF; }
static unsigned char get_lsb( unsigned short x ){ return x&0xFF; }
static void calc_crc_msb_lsb( const unsigned char *byte_stream, unsigned length, unsigned char *msb, unsigned char *lsb )
{
    unsigned short crc = crc16( byte_stream, length );
    *msb = get_msb(crc);
    *lsb = get_lsb(crc);
}
static bool checkCRC( const unsigned char *byte_stream, unsigned length )
{
    unsigned short calc_crc = crc16( byte_stream, length-2 );
    unsigned short rec_crc = (((unsigned short)byte_stream[length-1])<<8) | byte_stream[length-2] ;
    if( rec_crc==calc_crc )
    {
        return true;
    }
    return false;
}

/********************************************************************************************************/

static int createMessage(uint8 cmd, uint8 seq, uint8* data, uint8 data_size)
{
    uint8 crc_msb, crc_lsb;
    goutbuffer[SIGN_IDX] = START_SIGN;
    goutbuffer[SIGN2_IDX] = START_SIGN2;
    goutbuffer[SEQ_IDX] = seq;
    goutbuffer[CMD_IDX] = cmd;
    goutbuffer[DATA_SIZE_IDX] = data_size;
    memcpy((void*)&goutbuffer[DATA_START_IDX], (void*)data, data_size);

    calc_crc_msb_lsb( goutbuffer, data_size + 5, &crc_msb, &crc_lsb );

    goutbuffer[data_size + 5] = crc_lsb;
    goutbuffer[data_size + 6] = crc_msb;
    return (data_size + 7);
}
void check_header(int size)
{
    if (!checkCRC(&ginbuffer[0], size))
    {
        printf("CRC16 error!\n");
        return;
    }
    if ((goutbuffer[SIGN_IDX] != START_SIGN) ||
            (goutbuffer[SIGN2_IDX] != START_SIGN2))
    {
        printf("Incorrect message sign - error\n!");
        return;
    }
    if (seq != ginbuffer[SEQ_IDX])
    {
        printf("Incorrect message seq - error\n!");
        return;
    }
    printf("Header is ok\n");
}

void bf_ver_info_cmd_test(char* portname)
{
    printf("starting bf_ver_info_cmd_test....\n");
    uint8 data = 0;
    int dsize = createMessage(ver_info_cmd, seq, &data, 0);
    printf("*****\n");
    dispbuff0(dsize);
    int read_bytes = write_read_uart(portname, &goutbuffer[0], dsize, &ginbuffer[0], 32);
    printf("Read %d bytes in response.\n", read_bytes);
    check_header(read_bytes);
    dispbuff(read_bytes);
    if (ver_info_cmd != ginbuffer[CMD_IDX])
    {
        printf("Incorrect message command - error\n!");
        return;
    }
    printf("Message test - OK.\nsw:%d.%d.%d.%d; hw:%d\n", ginbuffer[DATA_START_IDX], ginbuffer[DATA_START_IDX+1],
                                        ginbuffer[DATA_START_IDX+2], ginbuffer[DATA_START_IDX+3]);
    memset((void*)&ginbuffer[0], 0, 32);
    seq++;
}