#include <stdio.h>
#include <stdlib.h>

#include "commonTypes.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "yahdlc.h"

#include "core.pb.h"

#include "uart/uart.h"
#include "gpb_msg_test.h"

uint8 test_decode_data0[] = {0x7e, 0xff, 0x10, 0x12, 0x4, 0x8, 0x0, 0x10, 0x0, 0x2c, 0xd3, 0x7e}; // Proto_FepAse_ReqResp_PING
uint8 test_decode_data1[] = {0x7e, 0xff, 0x12, 0xa, 0x2, 0x8, 0x0, 0xbb, 0xf, 0x7e};              // Proto_FepAse_Event_Type_LINE_IN_SENSE_ACTIVEc
uint8 test_decode_data2[] = {0x7e, 0xff, 0x14, 0xa, 0x2, 0x8, 0x1, 0xaa, 0x25, 0x7e};             // Proto_FepAse_Event_Type_LINE_IN_SENSE_INACTIVE
uint8 test_decode_data3[] = {0x7e, 0xff, 0x16, 0xa, 0xc, 0x8, 0xa, 0x52, 0x8, 0xa, 0x6, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0xcd, 0xf1, 0x7e};   //Proto_FepAse_Event_Type_TUNNEL
uint8 test_decode_data4[] = {0x7e, 0xff, 0x10, 0xa, 0xc, 0x8, 0xb, 0x5a, 0x8, 0x8, 0x29, 0x10, 0x1a, 0x18, 0x1a, 0x20, 0x1e, 0x4a, 0xa4, 0x7e}; //Proto_FepAse_Event_Type_OVERHEAT

int main(int argc, char **argv)
{
    // test_decode(&test_decode_data0[0], sizeof(test_decode_data0));
    // test_decode(&test_decode_data1[0], sizeof(test_decode_data1));
    // test_decode(&test_decode_data2[0], sizeof(test_decode_data2));
    // test_decode(&test_decode_data3[0], sizeof(test_decode_data3));
    // test_decode(&test_decode_data4[0], sizeof(test_decode_data4));

    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_BOOTED") == 0))
    {
        aseFep_Event_Booted();
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_SYSTEM_STATUS_STANDBY") == 0))
    {
        aseFep_Event_StatusStandby();
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_SYSTEM_STATUS_ON") == 0))
    {
        aseFep_Event_StatusOn();
        return 0;
    }

    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_VOLUME_CHANGED") == 0))
    {
        aseFep_Event_VolumeChange(atoi(argv[2]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_MUTE_CHANGED") == 0))
    {
        aseFep_Event_MuteChange(atoi(argv[2]));
        return 0;
    }
    if ((strcmp(argv[1], " ") == 0))
    {
        aseFep_Event_PlayerData(atoi(argv[2]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_Event_Type_DSP_TONE_TOUCH") == 0))
    {
        aseFep_Event_ToneTouch(atof(argv[2]), atof(argv[3]), atof(argv[4]), atof(argv[5]), atof(argv[6]), atof(argv[7]), atof(argv[8]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_POWER_REQUEST") == 0))
    {
        aseFep_Req_PowerOff();
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_AUDIO_MODE") == 0))
    {
        aseFep_Req_AudioMode(atoi(argv[2]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_AUDIO_INPUT") == 0))
    {
        aseFep_Req_AudioInput(atoi(argv[2]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_POSITION_SOUND_MODE") == 0))
    {
        aseFep_Req_Position(atoi(argv[2]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_INTERNAL_SPEAKER_COMPENSATION") == 0))
    {
        aseFep_Req_IntrSpeakerCompensation(atoi(argv[2]), atoi(argv[3]), atof(argv[4]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_DSP_PARAMETER") == 0))
    {
        aseFep_Req_DspParam(atoi(argv[2]), atoi(argv[3]));
        return 0;
    }
    if ((strcmp(argv[1], "Proto_AseFep_ReqResp_DSP_VOLUME_FADE") == 0))
    {
        aseFep_Req_DspVolumeFade(atoi(argv[2]), atoi(argv[3]));
        return 0;
    }
    if ((strcmp(argv[1], "ACK") == 0))
    {
        aseFep_ACK();
        return 0;
    }
    if ((strcmp(argv[1], "NACK") == 0))
    {
        aseFep_NACK();
        return 0;
    }
   return 0;
}