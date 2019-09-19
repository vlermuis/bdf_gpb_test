#include <stdio.h>
#include <stdlib.h>

#include "commonTypes.h"
#include <pb_encode.h>
#include <pb_decode.h>
#include "yahdlc.h"

#include "core.pb.h"


#define MAX_GPB_MSG                 (300)


static Proto_Core_FepAseMessage rMessage2;


static bool data_to_message2(Proto_Core_FepAseMessage* p_message, char* buff, uint32 size)
{
    bool status = false;

    /* Create a stream that reads from the buffer. */
    pb_istream_t stream = pb_istream_from_buffer((uint8*)buff, size);

    {
        /* Create a stream that reads from the buffer. */
        pb_istream_t stream = pb_istream_from_buffer((uint8*)buff, size);

        /* Now we are ready to decode the message. */
        status = pb_decode(&stream, Proto_Core_FepAseMessage_fields, p_message);
    }


    /* Check for errors... */
    if (!status)
    {
        uint32 ttype;
        printf("Protocol_buffer: Can not analyze the protocol buffer\r\n");
        switch(p_message->which_OneOf)
        {
        case Proto_Core_AseFepMessage_aseFepEvent_tag:
          ttype = p_message->OneOf.fepAseEvent.type;
          break;
        case Proto_Core_AseFepMessage_aseFepReq_tag:
          ttype = p_message->OneOf.fepAseReq.type;
          break;
        case Proto_Core_AseFepMessage_aseFepResp_tag:
          ttype = p_message->OneOf.fepAseResp.type;
          break;
        default:
          break;
        }
        asm("nop");
        printf("Pb_decode error! which_one_of: %d; ty pe: %d; \r\n", p_message->which_OneOf, ttype);
        return FALSE;
    }
    return TRUE;
}


void test_fepAseEvent_processing(Proto_Core_FepAseMessage * message)
{
    if (message->OneOf.fepAseEvent.has_type)
    {
        switch(message->OneOf.fepAseEvent.type)
        {
            case Proto_FepAse_Event_Type_LINE_IN_SENSE_ACTIVE:
                printf("Proto_FepAse_Event_Type_LINE_IN_SENSE_ACTIVE\n");
                break;
            case Proto_FepAse_Event_Type_LINE_IN_SENSE_INACTIVE:
                printf("Proto_FepAse_Event_Type_LINE_IN_SENSE_INACTIVE\n");
                break;
            case Proto_FepAse_Event_Type_TUNNEL:
                printf("Proto_FepAse_Event_Type_TUNNEL\n");
                break;
            case Proto_FepAse_Event_Type_OVERHEAT:
                printf("Proto_FepAse_Event_Type_OVERHEAT\n");
                break;

            default:
            break;
        }
    }
    
}
void test_fepAseReq_processing(Proto_Core_FepAseMessage * message)
{
    if (message->OneOf.fepAseReq.has_type)
    {
        switch(message->OneOf.fepAseReq.type)
        {
            case Proto_FepAse_ReqResp_PING:
                printf("Proto_FepAse_ReqResp_PING\n");
                break;

            default:
            break;
        }
    }
    
}
void test_fepAseResp_processing(Proto_Core_FepAseMessage * message)
{
    if (message->OneOf.fepAseResp.has_genericResponse && message->OneOf.fepAseResp.genericResponse.has_status)
    {
        switch (message->OneOf.fepAseResp.genericResponse.status)
        {
            case Proto_Core_GenericResponse_Status_DONE:
                if (message->OneOf.fepAseResp.has_id)
                {
                    printf("Proto_Core_GenericResponse_Status_DONE; request id : %d\n", message->OneOf.fepAseResp.id);
                }
                break;
            Proto_Core_GenericResponse_Status_ERROR:
                if (message->OneOf.fepAseResp.has_id)
                {
                    printf("Proto_Core_GenericResponse_Status_ERROR; request id : %d\n", message->OneOf.fepAseResp.id);
                }
                break;
            Proto_Core_GenericResponse_Status_NOT_SUPPORTED:
                if (message->OneOf.fepAseResp.has_id)
                {
                    printf("Proto_Core_GenericResponse_Status_NOT_SUPPORTED; request id : %d\n", message->OneOf.fepAseResp.id);
                }
                break;
            Proto_Core_GenericResponse_Status_UNKNOWN_REQUEST:
                if (message->OneOf.fepAseResp.has_id)
                {
                    printf("Proto_Core_GenericResponse_Status_UNKNOWN_REQUEST; request id : %d\n", message->OneOf.fepAseResp.id);
                }
                break;
            break;
        
        default:
            break;
        }
    }
    if (message->OneOf.fepAseResp.has_type)
    {
        switch(message->OneOf.fepAseResp.type)
        {
            case Proto_AseFep_ReqResp_DSP_GET_INTERNAL_SPEAKER_COMPENSATION:
                printf("Proto_AseFep_ReqResp_DSP_GET_INTERNAL_SPEAKER_COMPENSATION\n");
                break;

            case Proto_AseFep_ReqResp_DSP_GET_SPEAKER_POSITION:
                printf("Proto_AseFep_ReqResp_DSP_GET_SPEAKER_POSITION\n");
                break;
            default:
            break;
        }
    }
}


void test_decode(uint8 * incoming_buffer, uint16 size)
{
    static yahdlc_control_t control;
    static uint8 buff[MAX_GPB_MSG];
    static uint32 write_len = 0;

    int ret = yahdlc_get_data(&control, (char*)incoming_buffer, size, (char*)buff, (unsigned int*)&write_len);
    if (ret >= 0)
    {
        if(data_to_message2(&rMessage2, (char*)buff, write_len)  && (control.frame == YAHDLC_FRAME_DATA))
        {/* decoding is successful */
            switch(rMessage2.which_OneOf)
            {
                case Proto_Core_FepAseMessage_fepAseEvent_tag:
                    test_fepAseEvent_processing(&rMessage2);
                break;
                case Proto_Core_FepAseMessage_fepAseReq_tag:
                    test_fepAseReq_processing(&rMessage2);
                break;
                case Proto_Core_FepAseMessage_fepAseResp_tag:
                    test_fepAseResp_processing(&rMessage2);
                break;
                default:
                break;
            }
        }

    }
    else if (ret == -EIO)
    {/* incorrect CRC */

    }

}

