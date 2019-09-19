/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.7 at Wed Sep 18 09:51:57 2019. */

#ifndef PB_PROTO_CORE_CORE_PB_H_INCLUDED
#define PB_PROTO_CORE_CORE_PB_H_INCLUDED
#include <pb.h>

#include "ase-fep.pb.h"

#include "fep-ase.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Struct definitions */
typedef struct _Proto_Core_AseFepMessage {
    pb_size_t which_OneOf;
    union {
        Proto_AseFep_Event aseFepEvent;
        Proto_AseFep_Req aseFepReq;
        Proto_AseFep_Resp aseFepResp;
    } OneOf;
/* @@protoc_insertion_point(struct:Proto_Core_AseFepMessage) */
} Proto_Core_AseFepMessage;

typedef struct _Proto_Core_FepAseMessage {
    pb_size_t which_OneOf;
    union {
        Proto_FepAse_Event fepAseEvent;
        Proto_FepAse_Req fepAseReq;
        Proto_FepAse_Resp fepAseResp;
    } OneOf;
/* @@protoc_insertion_point(struct:Proto_Core_FepAseMessage) */
} Proto_Core_FepAseMessage;

/* Default values for struct fields */

/* Initializer values for message structs */
#define Proto_Core_AseFepMessage_init_default    {0, {Proto_AseFep_Event_init_default}}
#define Proto_Core_FepAseMessage_init_default    {0, {Proto_FepAse_Event_init_default}}
#define Proto_Core_AseFepMessage_init_zero       {0, {Proto_AseFep_Event_init_zero}}
#define Proto_Core_FepAseMessage_init_zero       {0, {Proto_FepAse_Event_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define Proto_Core_AseFepMessage_aseFepEvent_tag 1
#define Proto_Core_AseFepMessage_aseFepReq_tag   2
#define Proto_Core_AseFepMessage_aseFepResp_tag  3
#define Proto_Core_FepAseMessage_fepAseEvent_tag 1
#define Proto_Core_FepAseMessage_fepAseReq_tag   2
#define Proto_Core_FepAseMessage_fepAseResp_tag  3

/* Struct field encoding specification for nanopb */
extern const pb_field_t Proto_Core_AseFepMessage_fields[4];
extern const pb_field_t Proto_Core_FepAseMessage_fields[4];

/* Maximum encoded size of messages (where known) */
/* Proto_Core_AseFepMessage_size depends on runtime parameters */
/* Proto_Core_FepAseMessage_size depends on runtime parameters */

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define CORE_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
