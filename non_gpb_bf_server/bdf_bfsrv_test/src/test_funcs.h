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
void bf_get_power_cmd_test(char* portname);
void bf_set_power_cmd_test(char* portname, uint8 power_mode);
void bf_get_volume_cmd_test(char* portname);
void bf_set_volume_nofade_cmd_test(char* portname, uint8 target_volume);
void bf_set_volume_fade_cmd_test(char* portname, uint8 target_volume, uint16 duration);
void bf_get_mute_cmd_test(char* portname);
void bf_set_mute_cmd_test(char* portname, uint8 mute);
void bf_get_audio_mode_cmd_test(char* portname);
void bf_set_audio_mode_cmd_test(char* portname, uint8 audio_mode);
void bf_get_audio_source_cmd_test(char* portname);
void bf_set_audio_source_cmd_test(char* portname, uint8 source);
void bf_get_dsp_parameters_cmd_test(char* portname);
void bf_set_dsp_parameters_cmd_test(char* portname, uint8 loudness, uint8 treble, uint8 bass);
void bf_get_audio_signal_level_cmd_test(char* portname);
void bf_get_ntc_values_cmd_test(char* portname);
void bf_set_dsp_tone_touch_ceff_cmd_test(char* portname, uint8 gx_sign, uint8 gx_int, uint16 gx_frac,
                                                            uint8 gy_sign, uint8 gy_int, uint16 gy_frac,
                                                            uint8 gz_sign, uint8 gz_int, uint16 gz_frac);







#endif //TEST_FUNCS_H