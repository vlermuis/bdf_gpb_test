#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "uart.h"
#include "test_funcs.h"


/*
 * Test command parameters following bdf_bfsrv_test :
 *
 * /dev/ttyUSB0 ver_info_cmd
 * /dev/ttyUSB0 get_volume_cmd
 * /dev/ttyUSB0 set_volume_nofade_cmd 20   // set volume to 20
 * /dev/ttyUSB0 set_volume_fade_cmd 20 300 // set volume to 20 within 300ms
 * /dev/ttyUSB0 get_mute_cmd
 * /dev/ttyUSB0 set_mute_cmd  1             // 1 - mute, 0 - unmute
 * /dev/ttyUSB0 get_audio_mode_cmd
 * /dev/ttyUSB0 set_audio_mode_cmd          // 1 - entertainment; 0 - massage
 * /dev/ttyUSB0 get_audio_source_cmd
 * /dev/ttyUSB0 set_audio_source_cmd 1      // 0 - wifibt; 1 - sdcard; 2 - spdif; 3 - aux;
 * /dev/ttyUSB0 get_dsp_parameters_cmd
 * /dev/ttyUSB0 set_dsp_parameters_cmd 1 11 15 // loudness[0..1]; treble [0..20]; bass [0..20]
 * /dev/ttyUSB0 get_audio_signal_level_cmd
 * /dev/ttyUSB0 get_ntc_values_cmd
 * /dev/ttyUSB0 set_dsp_tone_touch_ceff_cmd -0.345 0.67 0.98
 *
 */

uint8 getPartsFromCharDouble(char* param, int8* pg_int, uint16* pg_frac)
{
    double gx = atof(param);
    uint8 slen;
    char tnum[12];
    char tnum2[2];
    uint16 gx_frac;
    uint8 gx_int;
    slen = strlen(param);
    if (gx < 0)
    {
        memcpy((void*)&tnum[0], (void*)&param[3], slen - 3);
        memcpy((void*)&tnum2[0], (void*)&param[1], 1);
    } else
    {
        memcpy((void*)&tnum[0], (void*)&param[2], slen - 2);
        memcpy((void*)&tnum2[0], (void*)&param[0], 1);
    }
    gx_frac = atoi(tnum);
    gx_int = atoi(tnum2);
    *pg_int = gx_int;
    *pg_frac = gx_frac;
    if (gx < 0)
    {
        return 1;
    } else
    {
        return 0;
    }


}

int main(int argc, char **argv)
{
    printf("BDF BFsrv test ver: 0.0.9\n");
    if (argc < 3)
    {
        printf("Incorrect input!\nbdf_bfsrv_test <uart port> <command> [parameters]\n");
        return -1;
    }
    if ((strcmp(argv[2], "ver_info_cmd") == 0))
    {
        bf_ver_info_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "get_power_cmd") == 0))
    {
        bf_get_power_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_power_cmd") == 0))
    {
        uint8 power_mode = atoi(argv[3]);
        bf_set_power_cmd_test(argv[1], power_mode);
        return 0;
    }

    if ((strcmp(argv[2], "get_volume_cmd") == 0))
    {
        bf_get_volume_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_volume_nofade_cmd") == 0))
    {
        uint8 target_volume = atoi(argv[3]);
        bf_set_volume_nofade_cmd_test(argv[1], target_volume);
        return 0;
    }
    if ((strcmp(argv[2], "set_volume_fade_cmd") == 0))
    {
        uint8 target_volume = atoi(argv[3]);
        uint16 duration = atoi(argv[4]);
        bf_set_volume_fade_cmd_test(argv[1], target_volume, duration);
        return 0;
    }
    if ((strcmp(argv[2], "get_mute_cmd") == 0))
    {
        bf_get_mute_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_mute_cmd") == 0))
    {
        uint8 mute = atoi(argv[3]);
        bf_set_mute_cmd_test(argv[1], mute);
        return 0;
    }
    if ((strcmp(argv[2], "get_audio_mode_cmd") == 0))
    {
        bf_get_audio_mode_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_audio_mode_cmd") == 0))
    {
        uint8 audio_mode = atoi(argv[3]);
        bf_set_audio_mode_cmd_test(argv[1], audio_mode);
        return 0;
    }
    if ((strcmp(argv[2], "get_audio_source_cmd") == 0))
    {
        bf_get_audio_source_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_audio_source_cmd") == 0))
    {
        uint8 source = atoi(argv[3]);
        bf_set_audio_source_cmd_test(argv[1], source);
        return 0;
    }
    if ((strcmp(argv[2], "get_dsp_parameters_cmd") == 0))
    {
        bf_get_dsp_parameters_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_dsp_parameters_cmd") == 0))
    {
        uint8 loudness = atoi(argv[3]);
        uint8 treble = atoi(argv[3]);
        uint8 bass = atoi(argv[3]);
        bf_set_dsp_parameters_cmd_test(argv[1], loudness, treble, bass);
        return 0;
    }
    if ((strcmp(argv[2], "get_audio_signal_level_cmd") == 0))
    {
        bf_get_audio_signal_level_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "get_ntc_values_cmd") == 0))
    {
        bf_get_ntc_values_cmd_test(argv[1]);
        return 0;
    }
    if ((strcmp(argv[2], "set_dsp_tone_touch_ceff_cmd") == 0))
    {
        uint8 gx_int;
        uint16 gx_frac;
        uint8 gx_sign = getPartsFromCharDouble(argv[3], &gx_int, &gx_frac);
    //    printf("%d : %d\n", gx_int, gx_frac);
        uint8 gy_int;
        uint16 gy_frac;
        uint8 gy_sign = getPartsFromCharDouble(argv[4], &gy_int, &gy_frac);
    //    printf("%d : %d\n", gy_int, gy_frac);
        uint8 gz_int;
        uint16 gz_frac;
        uint8 gz_sign = getPartsFromCharDouble(argv[5], &gz_int, &gz_frac);
    //    printf("%d : %d\n", gz_int, gz_frac);
        bf_set_dsp_tone_touch_ceff_cmd_test(argv[1], gx_sign, gx_int, gx_frac, gy_sign, gy_int, gy_frac, gz_sign, gz_int, gz_frac);
        return 0;
    }

  return 0;
}