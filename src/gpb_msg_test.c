#include <stdio.h>
#include "commonTypes.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "yahdlc.h"

#include "core.pb.h"

#include "uart/uart.h"


Proto_Core_FepAseMessage fepAseCmd;

Proto_Core_AseFepMessage aseFepCmd;

Proto_Core_AseFepMessage aseFepCmd_decode;

#define MAX_GPB_MSG                 (300)
#define YAHDLC_SEQ_NO_MAX           (7)

static uint8_t buffer[MAX_GPB_MSG];
static uint8 dest_buff[MAX_GPB_MSG];
static yahdlc_control_t control;

static uint8 buff[MAX_GPB_MSG];
static uint32 write_len = 0;



static uint32 message_to_data(Proto_Core_FepAseMessage* p_message, char* buff, uint32 size_of_buff)
{
    /* On FS1 v4.0.6, when send ASETK version, this function need
     *  472 (0x1D8) bytes on call stack
     */
    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer((uint8*)buff, size_of_buff);

    bool status = pb_encode(&stream, Proto_Core_FepAseMessage_fields, p_message);
//    printf("status : %d\n", status);

    uint32 message_length = stream.bytes_written;

    /* Then just check for any errors.. */
    if (!status)
    {
        printf("Protocol_buffer: Encoding failed\r\n");
        return 0;
    }
    return message_length;
}


static uint32 message_to_data2(Proto_Core_AseFepMessage* p_message, char* buff, uint32 size_of_buff)
{
    /* On FS1 v4.0.6, when send ASETK version, this function need
     *  472 (0x1D8) bytes on call stack
     */
    /* Create a stream that will write to our buffer. */
    pb_ostream_t stream = pb_ostream_from_buffer((uint8*)buff, size_of_buff);

    bool status = pb_encode(&stream, Proto_Core_AseFepMessage_fields, p_message);
//    printf("status : %d\n", status);

    uint32 message_length = stream.bytes_written;

    /* Then just check for any errors.. */
    if (!status)
    {
        printf("Protocol_buffer: Encoding failed\r\n");
        return 0;
    }
    return message_length;
}


static int yahdlc_seq_advance(int seq)
{
    const int yahdlc_seq_max = 7;   // yahdlc_control_t.seq_no with length of 3 bits

    if (yahdlc_seq_max <= seq)
    {
        seq = 0;
    }
    else
    {
        ++seq;
    }

    return seq;
}


void dbg_hex_output(char* msg, uint8* pdata, uint32 size)
{
    uint8_t i = 0;
    printf("\n%s : ", msg);
    for (i = 0; i < size; i++)
    {
        printf("0x%x, ", pdata[i]);
    }
    printf("\n");
}

void dbg_hex_output_for_gtkterm(char* msg, uint8* pdata, uint32 size)
{
    uint8_t i = 0;
    printf("\n%s : ", msg);
    for (i = 0; i < size; i++)
    {
        printf("%x ", pdata[i]);
    }
    printf("\n");
}


int fepAse_encode_frame(Proto_Core_FepAseMessage* p_message, char* msg_name)
{
    printf("%s\n", msg_name);
    static int seq = 0;
    memset(buffer, 0, sizeof(buffer));
    size_t message_length = message_to_data(p_message, (char*)buffer, sizeof(buffer));
    dbg_hex_output("message_to_data : ", (uint8*)&buffer[0], message_length); 

    if(message_length>0)
    {

        uint32 size = 0;
//        yahdlc_control_t control;
        /* framing the data streaming */
        control.frame = YAHDLC_FRAME_DATA;
        control.seq_no = seq;
        seq = yahdlc_seq_advance(seq);
        yahdlc_frame_data(&control, (char*)buffer, message_length, (char*)dest_buff, (unsigned int*)&size);

        dbg_hex_output("yahdlc_frame_data : ", &dest_buff[0], size); 

        return size;
    }
return -1;
}



int aseFep_encode_frame(Proto_Core_AseFepMessage* p_message, char* msg_name)
{
    printf("%s\n", msg_name);
    static int seq = 0;
    memset(buffer, 0, sizeof(buffer));
    size_t message_length = message_to_data2(p_message, (char*)buffer, sizeof(buffer));
    dbg_hex_output("message_to_data : ", (uint8*)&buffer[0], message_length); 

    if(message_length>0)
    {
        uint32 size = 0;
        /* framing the data streaming */
        control.frame = YAHDLC_FRAME_DATA;
        control.seq_no = seq;
        seq = yahdlc_seq_advance(seq);
        yahdlc_frame_data(&control, (char*)buffer, message_length, (char*)dest_buff, (unsigned int*)&size);

        dbg_hex_output("yahdlc_frame_data : ", &dest_buff[0], size); 
        
        return size;
    }
return -1;
}
/*****************************************************************************************************************************/

//  FEP to ASE

/*
 Proto_FepAse_ReqResp_PING
message_to_data :  : 0x12, 0x4, 0x8, 0x0, 0x10, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x10, 0x12, 0x4, 0x8, 0x0, 0x10, 0x0, 0x2c, 0xd3, 0x7e, 
*/
void fepAse_Req_Ping()
{
    memset((void*)&fepAseCmd, 0, sizeof(fepAseCmd));

    fepAseCmd.which_OneOf  = Proto_Core_FepAseMessage_fepAseReq_tag;
    fepAseCmd.OneOf.fepAseReq.has_type = true;
    fepAseCmd.OneOf.fepAseReq.type = Proto_FepAse_ReqResp_PING;
    fepAseCmd.OneOf.fepAseReq.has_id = true;
    fepAseCmd.OneOf.fepAseReq.id = Proto_FepAse_ReqResp_PING;  // wont use it anyway

    fepAse_encode_frame(&fepAseCmd, "Proto_FepAse_ReqResp_PING");
}

/*
Proto_FepAse_Event_Type_LINE_IN_SENSE_ACTIVE
message_to_data :  : 0xa, 0x2, 0x8, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x12, 0xa, 0x2, 0x8, 0x0, 0xbb, 0xf, 0x7e, 
*/
void fepAse_Event_LineIn_Active()
{
    memset((void*)&fepAseCmd, 0, sizeof(fepAseCmd));

    fepAseCmd.which_OneOf  = Proto_Core_FepAseMessage_fepAseEvent_tag;
    fepAseCmd.OneOf.fepAseEvent.has_type = true;
    fepAseCmd.OneOf.fepAseEvent.type = Proto_FepAse_Event_Type_LINE_IN_SENSE_ACTIVE;

    fepAse_encode_frame(&fepAseCmd, "Proto_FepAse_Event_Type_LINE_IN_SENSE_ACTIVE");

}

/*
Proto_FepAse_Event_Type_LINE_IN_SENSE_INACTIVE
message_to_data :  : 0xa, 0x2, 0x8, 0x1, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x14, 0xa, 0x2, 0x8, 0x1, 0xaa, 0x25, 0x7e,
*/
void fepAse_Event_LineIn_InActive()
{
    memset((void*)&fepAseCmd, 0, sizeof(fepAseCmd));

    fepAseCmd.which_OneOf  = Proto_Core_FepAseMessage_fepAseEvent_tag;
    fepAseCmd.OneOf.fepAseEvent.has_type = true;
    fepAseCmd.OneOf.fepAseEvent.type = Proto_FepAse_Event_Type_LINE_IN_SENSE_INACTIVE;

    fepAse_encode_frame(&fepAseCmd, "Proto_FepAse_Event_Type_LINE_IN_SENSE_INACTIVE");

}

/*
Proto_FepAse_Event_Type_TUNNEL
message_to_data :  : 0xa, 0xc, 0x8, 0xa, 0x52, 0x8, 0xa, 0x6, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x16, 0xa, 0xc, 0x8, 0xa, 0x52, 0x8, 0xa, 0x6, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0xcd, 0xf1, 0x7e, 
*/
void fepAse_Event_Tunnel()
{
    uint8 data[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    memset((void*)&fepAseCmd, 0, sizeof(fepAseCmd));
    fepAseCmd.which_OneOf = Proto_Core_FepAseMessage_fepAseEvent_tag;
    fepAseCmd.OneOf.fepAseEvent.has_type = true;
    fepAseCmd.OneOf.fepAseEvent.type = Proto_FepAse_Event_Type_TUNNEL;
    fepAseCmd.OneOf.fepAseEvent.which_data = Proto_FepAse_Event_productionTunnel_tag;
    fepAseCmd.OneOf.fepAseEvent.data.productionTunnel.has_data = TRUE;
    fepAseCmd.OneOf.fepAseEvent.data.productionTunnel.data.size = 6;

    memcpy((uint8*)&fepAseCmd.OneOf.fepAseEvent.data.productionTunnel.data.bytes[0], &data[0], 6);

    fepAse_encode_frame(&fepAseCmd, "Proto_FepAse_Event_Type_TUNNEL");
}

/*
Proto_FepAse_Event_Type_OVERHEAT
message_to_data :  : 0xa, 0xc, 0x8, 0xb, 0x5a, 0x8, 0x8, 0x29, 0x10, 0x1a, 0x18, 0x1a, 0x20, 0x1e, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x10, 0xa, 0xc, 0x8, 0xb, 0x5a, 0x8, 0x8, 0x29, 0x10, 0x1a, 0x18, 0x1a, 0x20, 0x1e, 0x4a, 0xa4, 0x7e,
*/
void fepAse_Event_Overheat()
{
    memset((void*)&fepAseCmd, 0, sizeof(fepAseCmd));
    fepAseCmd.which_OneOf = Proto_Core_FepAseMessage_fepAseEvent_tag;
    fepAseCmd.OneOf.fepAseEvent.has_type = true;
    fepAseCmd.OneOf.fepAseEvent.type = Proto_FepAse_Event_Type_OVERHEAT;
    fepAseCmd.OneOf.fepAseEvent.which_data = Proto_FepAse_Event_overheat_info_tag;
    fepAseCmd.OneOf.fepAseEvent.data.overheat_info.amp1 = 41;
    fepAseCmd.OneOf.fepAseEvent.data.overheat_info.amp2 = 26;
    fepAseCmd.OneOf.fepAseEvent.data.overheat_info.wfl = 26;
    fepAseCmd.OneOf.fepAseEvent.data.overheat_info.wfr = 30;

    fepAse_encode_frame(&fepAseCmd, "Proto_FepAse_Event_Type_OVERHEAT");
}


/*
Proto_Core_GenericResponse_Status_DONE
message_to_data :  : 0x1a, 0x7, 0x10, 0xb9, 0x60, 0x1a, 0x2, 0x8, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x18, 0x1a, 0x7, 0x10, 0xb9, 0x60, 0x1a, 0x2, 0x8, 0x0, 0x94, 0x74, 0x7e, 
*/
void fepAse_Resp_Generic()
{
    memset((void*)&fepAseCmd, 0, sizeof(fepAseCmd));
    fepAseCmd.which_OneOf = Proto_Core_FepAseMessage_fepAseResp_tag;
    fepAseCmd.OneOf.fepAseResp.has_id = true;
    fepAseCmd.OneOf.fepAseResp.id = 12345; // should be same with request id
    fepAseCmd.OneOf.fepAseResp.has_type = false;
    //fepAseCmd.OneOf.fepAseResp.type = type;
    fepAseCmd.OneOf.fepAseResp.has_genericResponse = true;
    fepAseCmd.OneOf.fepAseResp.genericResponse.has_status = true;
    fepAseCmd.OneOf.fepAseResp.genericResponse.status = Proto_Core_GenericResponse_Status_DONE;

    fepAse_encode_frame(&fepAseCmd, "Proto_Core_GenericResponse_Status_DONE");

}


//  ASE to FEP 

/*
Proto_AseFep_Event_Type_BOOTED
message_to_data :  : 0xa, 0x2, 0x8, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x10, 0xa, 0x2, 0x8, 0x0, 0x33, 0x19, 0x7e,
*/
void aseFep_Event_Booted()
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_BOOTED;

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_BOOTED");
    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }

}

/*
Proto_AseFep_Event_Type_SYSTEM_STATUS_STANDBY
message_to_data :  : 0xa, 0x2, 0x8, 0x1, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x12, 0xa, 0x2, 0x8, 0x1, 0x32, 0x1e, 0x7e, */
void aseFep_Event_StatusStandby()
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_SYSTEM_STATUS_STANDBY;

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_SYSTEM_STATUS_STANDBY");
    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }

}

/*
Proto_AseFep_Event_Type_SYSTEM_STATUS_ON
message_to_data :  : 0xa, 0x2, 0x8, 0x2, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x14, 0xa, 0x2, 0x8, 0x2, 0x31, 0x17, 0x7e, 
*/
void aseFep_Event_StatusOn()
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_SYSTEM_STATUS_ON;

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_SYSTEM_STATUS_ON");
    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }

}


/*
Proto_AseFep_Event_Type_TUNNEL
message_to_data :  : 0xa, 0xc, 0x8, 0x6, 0x52, 0x8, 0xa, 0x6, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x16, 0xa, 0xc, 0x8, 0x6, 0x52, 0x8, 0xa, 0x6, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff, 0x32, 0x13, 0x7e, 
*/
void aseFep_Event_Tunnel()
{
    uint8 data[6] = {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_TUNNEL;
    aseFepCmd.OneOf.aseFepEvent.which_data = Proto_AseFep_Event_productionTunnel_tag;
    aseFepCmd.OneOf.aseFepEvent.data.productionTunnel.has_data = true;
    aseFepCmd.OneOf.aseFepEvent.data.productionTunnel.data.size = 6;
    memcpy((uint8*)&aseFepCmd.OneOf.aseFepEvent.data.productionTunnel.data.bytes[0], &data[0], 6);

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_TUNNEL");
    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }

}

/*
Proto_AseFep_Event_Type_VOLUME_CHANGED
message_to_data :  : 0xa, 0x6, 0x8, 0xc, 0x5a, 0x2, 0x8, 0x13, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x10, 0xa, 0x6, 0x8, 0xc, 0x5a, 0x2, 0x8, 0x13, 0xf6, 0xc4, 0x7e, 
*/
void aseFep_Event_VolumeChange(uint8 volume)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_VOLUME_CHANGED;
    aseFepCmd.OneOf.aseFepEvent.which_data = Proto_AseFep_Event_volume_tag;
    aseFepCmd.OneOf.aseFepEvent.data.volume.has_volume = true;
    aseFepCmd.OneOf.aseFepEvent.data.volume.volume = volume;

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_VOLUME_CHANGED");

    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }


}


/*
Proto_AseFep_Event_Type_MUTE_CHANGED
message_to_data :  : 0xa, 0x6, 0x8, 0xd, 0x5a, 0x2, 0x8, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x1a, 0xa, 0x6, 0x8, 0xd, 0x5a, 0x2, 0x8, 0x0, 0x98, 0x9, 0x7e,*/
void aseFep_Event_MuteChange(uint8 mute)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_MUTE_CHANGED;
    aseFepCmd.OneOf.aseFepEvent.which_data = Proto_AseFep_Event_mute_tag;
    aseFepCmd.OneOf.aseFepEvent.data.mute.has_mute = true;
    aseFepCmd.OneOf.aseFepEvent.data.mute.mute = mute;

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_MUTE_CHANGED");
    if (len != -1)
    {
        dbg_hex_output_for_gtkterm("write_uart", &dest_buff[0], len);
        write_uart(&dest_buff[0], len);
    }

}

/*
Proto_AseFep_Event_Type_PLAYER_DATA
message_to_data :  : 0xa, 0x6, 0x8, 0xf, 0x72, 0x2, 0x8, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x1c, 0xa, 0x6, 0x8, 0xf, 0x72, 0x2, 0x8, 0x0, 0x84, 0xd1, 0x7e,
state : >>>
typedef enum _Proto_Player_Data_State {
    Proto_Player_Data_State_PLAYING = 0,
    Proto_Player_Data_State_PAUSED = 1,
    Proto_Player_Data_State_STOPPED = 2
} Proto_Player_Data_State;

*/
void aseFep_Event_PlayerData(uint8 state)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_PLAYER_DATA;
    aseFepCmd.OneOf.aseFepEvent.which_data = Proto_AseFep_Event_playerData_tag;
    aseFepCmd.OneOf.aseFepEvent.data.playerData.has_state = true;
    aseFepCmd.OneOf.aseFepEvent.data.playerData.state = state;

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_PLAYER_DATA");
    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }

}


/*
Proto_AseFep_Event_Type_DSP_TONE_TOUCH
message_to_data :  : 0xa, 0x45, 0x8, 0x1b, 0x7a, 0x41, 0x9, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xd3, 0x3f, 0x11, 0xcd, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xec, 0x3f, 0x19, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xb9, 0x3f, 0x21, 0xcd, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xec, 0x3f, 0x29, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xe6, 0x3f, 0x31, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xc9, 0x3f, 0x39, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xe9, 0x3f, 0x40, 0x1, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x1e, 0xa, 0x45, 0x8, 0x1b, 0x7a, 0x41, 0x9, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xd3, 0x3f, 0x11, 0xcd, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xec, 0x3f, 0x19, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xb9, 0x3f, 0x21, 0xcd, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xec, 0x3f, 0x29, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0xe6, 0x3f, 0x31, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xc9, 0x3f, 0x39, 0x9a, 0x99, 0x99, 0x99, 0x99, 0x99, 0xe9, 0x3f, 0x40, 0x1, 0xde, 0x6f, 0x7e,
*/
void aseFep_Event_ToneTouch(double Gx1, double Gx2, double Gy1, double Gy2, double Gz, double k5, double k6)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepEvent_tag;
    aseFepCmd.OneOf.aseFepEvent.has_type = true;
    aseFepCmd.OneOf.aseFepEvent.type = Proto_AseFep_Event_Type_DSP_TONE_TOUCH;
    aseFepCmd.OneOf.aseFepEvent.which_data = Proto_AseFep_Event_dspToneTouch_tag;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_enabled = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.enabled = 1;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_Gx1 = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.Gx1 = Gx1;//0.3;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_Gx2 = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.Gx2 = Gx2;//0.1;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_Gy1 = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.Gy1 = Gy1;//0.9;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_Gy2 = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.Gy2 = Gy2;//0.9;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_Gz = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.Gz = Gz;//0.7;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_k5 = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.k5 = k5;//0.2;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.has_k6 = true;
    aseFepCmd.OneOf.aseFepEvent.data.dspToneTouch.k6 = k6;//0.8;
    
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_Event_Type_DSP_TONE_TOUCH");
    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }

}



/*
Proto_AseFep_ReqResp_POWER_REQUEST
message_to_data :  : 0x12, 0x4, 0x8, 0x7, 0x10, 0x7, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x10, 0x12, 0x4, 0x8, 0x7, 0x10, 0x7, 0x96, 0x2b, 0x7e, 
*/
void aseFep_Req_PowerOff()
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_POWER_REQUEST;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_POWER_REQUEST; // can be any number 

    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_POWER_REQUEST");

    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
    
}

/*
Proto_AseFep_ReqResp_AUDIO_MODE
message_to_data :  : 0x12, 0x8, 0x8, 0x8, 0x10, 0x8, 0x62, 0x2, 0x8, 0x1, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x12, 0x12, 0x8, 0x8, 0x8, 0x10, 0x8, 0x62, 0x2, 0x8, 0x1, 0x70, 0x21, 0x7e, 
mode : >>>
typedef enum _Proto_Dsp_AudioMode_AudioMode {
    Proto_Dsp_AudioMode_AudioMode_ENTERTAINMENT = 0,
    Proto_Dsp_AudioMode_AudioMode_MASSAGE = 1
} Proto_Dsp_AudioMode_AudioMode;

*/
void aseFep_Req_AudioMode(uint8 mode)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_AUDIO_MODE;
    aseFepCmd.OneOf.aseFepReq.which_data = Proto_AseFep_Req_audioMode_tag;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_AUDIO_MODE; // can be any number 
    aseFepCmd.OneOf.aseFepReq.data.audioMode.has_mode = true;
    aseFepCmd.OneOf.aseFepReq.data.audioMode.mode = mode;//Proto_Dsp_AudioMode_AudioMode_MASSAGE;
    
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_AUDIO_MODE");

    if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
    
}


/*
Proto_AseFep_ReqResp_AUDIO_INPUT
message_to_data :  : 0x12, 0x8, 0x8, 0x9, 0x10, 0x9, 0x6a, 0x2, 0x8, 0x0, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x14, 0x12, 0x8, 0x8, 0x9, 0x10, 0x9, 0x6a, 0x2, 0x8, 0x0, 0xc7, 0xb4, 0x7e, 
input : >>>
typedef enum _Proto_Dsp_RequestAudioInput_AudioInput {
    Proto_Dsp_RequestAudioInput_AudioInput_WIFIBT = 0,
    Proto_Dsp_RequestAudioInput_AudioInput_SDCARD = 1,
    Proto_Dsp_RequestAudioInput_AudioInput_LINE = 2
} Proto_Dsp_RequestAudioInput_AudioInput;

*/
void aseFep_Req_AudioInput(uint8 input)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_AUDIO_INPUT;
    aseFepCmd.OneOf.aseFepReq.which_data = Proto_AseFep_Req_audioInput_tag;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_AUDIO_INPUT; // can be any number 
    aseFepCmd.OneOf.aseFepReq.data.audioInput.has_input = true;
    aseFepCmd.OneOf.aseFepReq.data.audioInput.input = input;//Proto_Dsp_RequestAudioInput_AudioInput_WIFIBT;
    
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_AUDIO_INPUT");

     if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
  

}


/*
Proto_AseFep_ReqResp_POSITION_SOUND_MODE
message_to_data :  : 0x12, 0xa, 0x8, 0xa, 0x10, 0xa, 0x72, 0x4, 0xa, 0x2, 0x8, 0x1, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x10, 0x12, 0xa, 0x8, 0xa, 0x10, 0xa, 0x72, 0x4, 0xa, 0x2, 0x8, 0x1, 0x42, 0xb3, 0x7e,
position : >>>
typedef enum _Proto_Dsp_PositionSoundMode_Position {
    Proto_Dsp_PositionSoundMode_Position_UNDEFINED = 0,
    Proto_Dsp_PositionSoundMode_Position_FREE = 1,
    Proto_Dsp_PositionSoundMode_Position_WALL = 2,
    Proto_Dsp_PositionSoundMode_Position_CORNER = 3,
    Proto_Dsp_PositionSoundMode_Position_TABLE = 4
} Proto_Dsp_PositionSoundMode_Position;

*/
void aseFep_Req_Position(uint8 position)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_POSITION_SOUND_MODE;
    aseFepCmd.OneOf.aseFepReq.which_data = Proto_AseFep_Req_positionSoundMode_tag;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_POSITION_SOUND_MODE; // can be any number 
    aseFepCmd.OneOf.aseFepReq.data.positionSoundMode.has_positionSoundMode = true;
    aseFepCmd.OneOf.aseFepReq.data.positionSoundMode.positionSoundMode.has_position = true;
    aseFepCmd.OneOf.aseFepReq.data.positionSoundMode.positionSoundMode.position = position;//Proto_Dsp_PositionSoundMode_Position_FREE;

    
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_POSITION_SOUND_MODE");

     if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
}

/*
Proto_AseFep_ReqResp_INTERNAL_SPEAKER_COMPENSATION
message_to_data :  : 0x12, 0x11, 0x8, 0xb, 0x10, 0xb, 0x7a, 0xb, 0xa, 0x9, 0x19, 0x14, 0xae, 0x47, 0xe1, 0x7a, 0x14, 0xf2, 0x3f, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x12, 0x12, 0x11, 0x8, 0xb, 0x10, 0xb, 0x7a, 0xb, 0xa, 0x9, 0x19, 0x14, 0xae, 0x47, 0xe1, 0x7a, 0x14, 0xf2, 0x3f, 0x9f, 0x81, 0x7e, 
type: >>>
typedef enum _Proto_Dsp_InternalSpeaker_Type {
    Proto_Dsp_InternalSpeaker_Type_TWEETER = 0,
    Proto_Dsp_InternalSpeaker_Type_MIDRANGE = 1,
    Proto_Dsp_InternalSpeaker_Type_WOOFER = 2,
    Proto_Dsp_InternalSpeaker_Type_FULLRANGE = 3
} Proto_Dsp_InternalSpeaker_Type;

position: >>>
typedef enum _Proto_Dsp_InternalSpeaker_Position {
    Proto_Dsp_InternalSpeaker_Position_LEFT = 0,
    Proto_Dsp_InternalSpeaker_Position_RIGHT = 1,
    Proto_Dsp_InternalSpeaker_Position_CENTRE = 2
} Proto_Dsp_InternalSpeaker_Position;


*/
void aseFep_Req_IntrSpeakerCompensation(uint8 type, uint8 position, double compensation)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_INTERNAL_SPEAKER_COMPENSATION;
    aseFepCmd.OneOf.aseFepReq.which_data = Proto_AseFep_Req_internalSpeakerCompensation_tag;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_INTERNAL_SPEAKER_COMPENSATION; // can be any number 
    
    aseFepCmd.OneOf.aseFepReq.data.internalSpeakerCompensation.internalSpeaker.has_type = true;
    aseFepCmd.OneOf.aseFepReq.data.internalSpeakerCompensation.internalSpeaker.type = type;

    aseFepCmd.OneOf.aseFepReq.data.internalSpeakerCompensation.internalSpeaker.has_position = true;
    aseFepCmd.OneOf.aseFepReq.data.internalSpeakerCompensation.internalSpeaker.position = position;

    aseFepCmd.OneOf.aseFepReq.data.internalSpeakerCompensation.internalSpeaker.has_compensation = true;
    aseFepCmd.OneOf.aseFepReq.data.internalSpeakerCompensation.internalSpeaker.compensation = compensation;//1.13;

   
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_INTERNAL_SPEAKER_COMPENSATION");

     if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
}


/*
Proto_AseFep_ReqResp_DSP_PARAMETER
message_to_data :  : 0x12, 0xb, 0x8, 0x2d, 0x10, 0x2d, 0x9a, 0x2, 0x4, 0x8, 0x1, 0x10, 0xa, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x14, 0x12, 0xb, 0x8, 0x2d, 0x10, 0x2d, 0x9a, 0x2, 0x4, 0x8, 0x1, 0x10, 0xa, 0x1a, 0x43, 0x7e, 
type : >>>
typedef enum _Proto_Dsp_Parameter_Type {
    Proto_Dsp_Parameter_Type_LOUDNESS = 0,
    Proto_Dsp_Parameter_Type_BASS = 1,
    Proto_Dsp_Parameter_Type_TREBLE = 2
} Proto_Dsp_Parameter_Type;


*/
void aseFep_Req_DspParam(uint8 type, uint8 value)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_DSP_PARAMETER;
    aseFepCmd.OneOf.aseFepReq.which_data = Proto_AseFep_Req_dspParameter_tag;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_DSP_PARAMETER; // can be any number 
    aseFepCmd.OneOf.aseFepReq.data.dspParameter.has_type = true;
    aseFepCmd.OneOf.aseFepReq.data.dspParameter.type = type;//Proto_Dsp_Parameter_Type_BASS;
    aseFepCmd.OneOf.aseFepReq.data.dspParameter.has_value = true;
    aseFepCmd.OneOf.aseFepReq.data.dspParameter.value = value;//10;
   
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_DSP_PARAMETER");

     if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
}

/*
Proto_AseFep_ReqResp_DSP_VOLUME_FADE
message_to_data :  : 0x12, 0xc, 0x8, 0x38, 0x10, 0x38, 0xc2, 0x2, 0x5, 0x8, 0x20, 0x10, 0xac, 0x2, 
yahdlc_frame_data :  : 0x7e, 0xff, 0x16, 0x12, 0xc, 0x8, 0x38, 0x10, 0x38, 0xc2, 0x2, 0x5, 0x8, 0x20, 0x10, 0xac, 0x2, 0x31, 0x6c, 0x7e, 
fade_duration in ms
target_volume [0..32]
*/
void aseFep_Req_DspVolumeFade(uint8 target_volume, uint16 fade_duration)
{
    memset((void*)&aseFepCmd, 0, sizeof(aseFepCmd));

    aseFepCmd.which_OneOf = Proto_Core_AseFepMessage_aseFepReq_tag;
    aseFepCmd.OneOf.aseFepReq.has_type = true;
    aseFepCmd.OneOf.aseFepReq.type = Proto_AseFep_ReqResp_DSP_VOLUME_FADE;
    aseFepCmd.OneOf.aseFepReq.which_data = Proto_AseFep_Req_dspAbsoluteVolume_tag;
    aseFepCmd.OneOf.aseFepReq.has_id = true;
    aseFepCmd.OneOf.aseFepReq.id = Proto_AseFep_ReqResp_DSP_VOLUME_FADE; // can be any number 
    aseFepCmd.OneOf.aseFepReq.data.dspAbsoluteVolume.has_fade_duration = true;
    aseFepCmd.OneOf.aseFepReq.data.dspAbsoluteVolume.fade_duration = fade_duration;//300;
    aseFepCmd.OneOf.aseFepReq.data.dspAbsoluteVolume.has_volume = true;
    aseFepCmd.OneOf.aseFepReq.data.dspAbsoluteVolume.volume = target_volume;//32;
   
    int len = aseFep_encode_frame(&aseFepCmd, "Proto_AseFep_ReqResp_DSP_VOLUME_FADE");
    dbg_hex_output_for_gtkterm("write_uart", &dest_buff[0], len);

     if (len != -1)
    {
        write_uart(&dest_buff[0], len);
    }
}

void aseFep_ACK()
{
    uint8 seq_no = 1;
    yahdlc_control_t control;
    uint32 size = 0;
    uint8 dest_buff[MAX_GPB_MSG];
    uint8* fake_src = dest_buff;    // fake src pointer as yahdlc does not accept src to be null

    control.frame = YAHDLC_FRAME_ACK;
    if (seq_no >= YAHDLC_SEQ_NO_MAX)
    {
        control.seq_no = 0;
    }
    else
    {
        control.seq_no= seq_no + 1;
    }
    yahdlc_frame_data(&control, (char*)fake_src, 1, (char*)dest_buff, (unsigned int*)&size);

    dbg_hex_output("yahdlc_frame_data : ", &dest_buff[0], size); 

     if (size != -1)
    {
        write_uart(&dest_buff[0], size);
    }

}

void aseFep_NACK()
{
    uint8 seq_no = 1;
    yahdlc_control_t control;
    uint32 size = 0;
    uint8 dest_buff[MAX_GPB_MSG];
    uint8* fake_src = dest_buff;    // fake src pointer as yahdlc does not accept src to be null

    control.frame = YAHDLC_FRAME_NACK;
    control.seq_no= seq_no;
    yahdlc_frame_data(&control, (char*)fake_src, 1, (char*)dest_buff, (unsigned int*)&size);

    dbg_hex_output("yahdlc_frame_data : ", &dest_buff[0], size); 

     if (size != -1)
    {
        write_uart(&dest_buff[0], size);
    }
}

























