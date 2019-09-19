# Protocol Documentation
<a name="top"/>

## Table of Contents
* [ase-fep.proto](#ase-fep.proto)
 * [Event](#Proto.AseFep.Event)
 * [Req](#Proto.AseFep.Req)
 * [Resp](#Proto.AseFep.Resp)
 * [Event.Type](#Proto.AseFep.Event.Type)
* [ase-fep-ReqResp.proto](#ase-fep-ReqResp.proto)
 * [ReqResp](#Proto.AseFep.ReqResp)
* [common.proto](#common.proto)
 * [GenericResponse](#Proto.Core.GenericResponse)
 * [GenericResponse.Status](#Proto.Core.GenericResponse.Status)
* [core.proto](#core.proto)
 * [AseFepMessage](#Proto.Core.AseFepMessage)
 * [FepAseMessage](#Proto.Core.FepAseMessage)
* [dsp.proto](#dsp.proto)
 * [AbsoluteVolume](#Proto.Dsp.AbsoluteVolume)
 * [AudioMode](#Proto.Dsp.AudioMode)
 * [InternalSpeaker](#Proto.Dsp.InternalSpeaker)
 * [Mute](#Proto.Dsp.Mute)
 * [Parameter](#Proto.Dsp.Parameter)
 * [PositionSoundMode](#Proto.Dsp.PositionSoundMode)
 * [RelativeVolumeChange](#Proto.Dsp.RelativeVolumeChange)
 * [RequestAudioInput](#Proto.Dsp.RequestAudioInput)
 * [RequestInternalSpeakerCompensation](#Proto.Dsp.RequestInternalSpeakerCompensation)
 * [RequestPositionSoundMode](#Proto.Dsp.RequestPositionSoundMode)
 * [ResponseInternalSpeakerCompensation](#Proto.Dsp.ResponseInternalSpeakerCompensation)
 * [ResponsePositionSoundMode](#Proto.Dsp.ResponsePositionSoundMode)
 * [ToneTouch](#Proto.Dsp.ToneTouch)
 * [AudioMode.AudioMode](#Proto.Dsp.AudioMode.AudioMode)
 * [InternalSpeaker.Position](#Proto.Dsp.InternalSpeaker.Position)
 * [InternalSpeaker.Type](#Proto.Dsp.InternalSpeaker.Type)
 * [Parameter.Type](#Proto.Dsp.Parameter.Type)
 * [PositionSoundMode.Orientation](#Proto.Dsp.PositionSoundMode.Orientation)
 * [PositionSoundMode.Position](#Proto.Dsp.PositionSoundMode.Position)
 * [RequestAudioInput.AudioInput](#Proto.Dsp.RequestAudioInput.AudioInput)
 * [ResponseInternalSpeakerCompensation.Error](#Proto.Dsp.ResponseInternalSpeakerCompensation.Error)
 * [ResponsePositionSoundMode.Error](#Proto.Dsp.ResponsePositionSoundMode.Error)
* [fep-ase.proto](#fep-ase.proto)
 * [Event](#Proto.FepAse.Event)
 * [Req](#Proto.FepAse.Req)
 * [Resp](#Proto.FepAse.Resp)
 * [Event.Type](#Proto.FepAse.Event.Type)
* [fep-ase-ReqResp.proto](#fep-ase-ReqResp.proto)
 * [ReqResp](#Proto.FepAse.ReqResp)
* [player.proto](#player.proto)
 * [Data](#Proto.Player.Data)
 * [Data.Source](#Proto.Player.Data.Source)
 * [Data.State](#Proto.Player.Data.State)
* [production.proto](#production.proto)
 * [Tunnel](#Proto.Production.Tunnel)
* [Scalar Value Types](#scalar-value-types)

<a name="ase-fep.proto"/>
<p align="right"><a href="#top">Top</a></p>

## ase-fep.proto



<a name="Proto.AseFep.Event"/>
### Event
Event report from ASE to FEP .

| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [Event.Type](#Proto.AseFep.Event.Type) | optional |  |
| productionTunnel | [Tunnel](#Proto.Production.Tunnel) | optional |  |
| volume | [AbsoluteVolume](#Proto.Dsp.AbsoluteVolume) | optional |  |
| mute | [Mute](#Proto.Dsp.Mute) | optional |  |
| playerData | [Data](#Proto.Player.Data) | optional |  |
| dspToneTouch | [ToneTouch](#Proto.Dsp.ToneTouch) | optional |  |


<a name="Proto.AseFep.Req"/>
### Req


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [ReqResp](#Proto.AseFep.ReqResp) | optional | this should be copied from the request to the response/ to be able to pair messages |
| id | [uint32](#uint32) | optional | this should be copied from the request to the response/ to be able to pair messages |
| audioMode | [AudioMode](#Proto.Dsp.AudioMode) | optional |  |
| audioInput | [RequestAudioInput](#Proto.Dsp.RequestAudioInput) | optional |  |
| positionSoundMode | [RequestPositionSoundMode](#Proto.Dsp.RequestPositionSoundMode) | optional |  |
| internalSpeakerCompensation | [RequestInternalSpeakerCompensation](#Proto.Dsp.RequestInternalSpeakerCompensation) | optional |  |
| dspParameter | [Parameter](#Proto.Dsp.Parameter) | optional |  |
| dspAbsoluteVolume | [AbsoluteVolume](#Proto.Dsp.AbsoluteVolume) | optional |  |


<a name="Proto.AseFep.Resp"/>
### Resp


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [ReqResp](#Proto.FepAse.ReqResp) | optional | this should be copied from the request to the response/ to be able to pair messages |
| id | [uint32](#uint32) | optional | this should be copied from the request to the response/ to be able to pair messages |
| genericResponse | [GenericResponse](#Proto.Core.GenericResponse) | optional |  |
| volume | [AbsoluteVolume](#Proto.Dsp.AbsoluteVolume) | optional |  |



<a name="Proto.AseFep.Event.Type"/>
### Event.Type


| Name | Number | Description |
| ---- | ------ | ----------- |
| BOOTED | 0 | ASE has completed the boot sequence and is ready to receive messages from FEP |
| SYSTEM_STATUS_STANDBY | 1 | Event send when ASE enter source standby |
| SYSTEM_STATUS_ON | 2 | Event send when exiting standby (a source has started from standby) |
| TUNNEL | 6 | associated data in productionTunnel |
| VOLUME_CHANGED | 12 | associated data in field of type Dsp.AbsoluteVolume/ field fade_duration should be ignored. Use DSP_VOLUME_FADE request to fade volume |
| MUTE_CHANGED | 13 | associated data in field of type Dsp.Mute |
| PLAYER_DATA | 15 | associated data in field of type Player.Data |
| DSP_TONE_TOUCH | 27 | Event that is reporting currently set ToneTouch parameters./ Fep should apply parameters from data if possible/ associated data is of type Dsp.ToneTouch |




<a name="ase-fep-ReqResp.proto"/>
<p align="right"><a href="#top">Top</a></p>

## ase-fep-ReqResp.proto




<a name="Proto.AseFep.ReqResp"/>
### ReqResp


| Name | Number | Description |
| ---- | ------ | ----------- |
| POWER_REQUEST | 7 | has request data of type System.PowerRequest/ has generic response/ DONE if the power request has been accepted/ ERROR if the command failed |
| AUDIO_MODE | 8 | Generic response/ switch device to entertainment or massage modes |
| AUDIO_INPUT | 9 | Generic response/ Has request data of type Dsp.AudioInput |
| POSITION_SOUND_MODE | 10 | has request data of type Dsp.RequestPositionSoundMode/ has response data of type Dsp.ResponsePositionSoundMode |
| INTERNAL_SPEAKER_COMPENSATION | 11 | has request data of type Dsp.RequestInternalSpeakerCompensation/ has response data of type Dsp.ResponseInternalSpeakerCompensation |
| DSP_PARAMETER | 45 | Request DSP paramter change/ Request has value of type Dsp.Parameter/ Possible ERROR is set inside generic response |
| DSP_VOLUME_FADE | 56 | Request volume fade/ Input parameter Dsp.AbsoluteVolume with fade_duration field set/ Response: Generic response. If successful, this response should be delivered upon volume fade is finished |
| DSP_GET_INTERNAL_SPEAKER_COMPENSATION | 86 | Get Speakers Compensations/ Response has Dsp.ResponseInternalSpeakerCompensation |
| DSP_GET_SPEAKER_POSITION | 88 | Response has Dsp.ResponsePositionSoundMode |




<a name="common.proto"/>
<p align="right"><a href="#top">Top</a></p>

## common.proto



<a name="Proto.Core.GenericResponse"/>
### GenericResponse


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| status | [GenericResponse.Status](#Proto.Core.GenericResponse.Status) | optional |  |



<a name="Proto.Core.GenericResponse.Status"/>
### GenericResponse.Status


| Name | Number | Description |
| ---- | ------ | ----------- |
| DONE | 0 | This is sent when the request was handled |
| ERROR | 1 | This is sent when the request finished with error |
| NOT_SUPPORTED | 2 | This is sent when the request is not handled/ This is not meant to be used when reporting statuses from the user's handler |
| UNKNOWN_REQUEST | 3 | This is sent when the request cannot be parsed from the message/ This is not meant to be used when reporting statuses from the user's handler |




<a name="core.proto"/>
<p align="right"><a href="#top">Top</a></p>

## core.proto



<a name="Proto.Core.AseFepMessage"/>
### AseFepMessage


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| aseFepEvent | [Event](#Proto.AseFep.Event) | optional |  |
| aseFepReq | [Req](#Proto.AseFep.Req) | optional |  |
| aseFepResp | [Resp](#Proto.AseFep.Resp) | optional |  |


<a name="Proto.Core.FepAseMessage"/>
### FepAseMessage


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| fepAseEvent | [Event](#Proto.FepAse.Event) | optional |  |
| fepAseReq | [Req](#Proto.FepAse.Req) | optional |  |
| fepAseResp | [Resp](#Proto.FepAse.Resp) | optional |  |






<a name="dsp.proto"/>
<p align="right"><a href="#top">Top</a></p>

## dsp.proto



<a name="Proto.Dsp.AbsoluteVolume"/>
### AbsoluteVolume


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| volume | [uint32](#uint32) | optional |  |
| fade_duration | [uint32](#uint32) | optional | In milliseconds the duration of the fade operation |


<a name="Proto.Dsp.AudioMode"/>
### AudioMode


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| mode | [AudioMode.AudioMode](#Proto.Dsp.AudioMode.AudioMode) | optional | Configure the audio mode. |


<a name="Proto.Dsp.InternalSpeaker"/>
### InternalSpeaker


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| position | [InternalSpeaker.Position](#Proto.Dsp.InternalSpeaker.Position) | optional | Valid combinations for FS1:/ CENTRE - WOOFER/ CENTRE - FULLRANGE// Valid combinations for FS2:/ CENTRE - WOOFER/ LEFT - MIDRANGE/ RIGHT - MIDRANGE/ CENTRE - TWEETER |
| type | [InternalSpeaker.Type](#Proto.Dsp.InternalSpeaker.Type) | optional |  |
| compensation | [double](#double) | optional | The amount of gain compensation to apply to the speaker unit in dB |


<a name="Proto.Dsp.Mute"/>
### Mute


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| mute | [bool](#bool) | optional |  |


<a name="Proto.Dsp.Parameter"/>
### Parameter


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [Parameter.Type](#Proto.Dsp.Parameter.Type) | optional | Mandatory field - specifies which dsp parameter should be altered |
| value | [int32](#int32) | optional | Mandatory field - parameter value |


<a name="Proto.Dsp.PositionSoundMode"/>
### PositionSoundMode
Position sound mode setting

| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| position | [PositionSoundMode.Position](#Proto.Dsp.PositionSoundMode.Position) | optional |  |
| orientation | [PositionSoundMode.Orientation](#Proto.Dsp.PositionSoundMode.Orientation) | optional |  |


<a name="Proto.Dsp.RelativeVolumeChange"/>
### RelativeVolumeChange


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| volumeChange | [int32](#int32) | optional |  |


<a name="Proto.Dsp.RequestAudioInput"/>
### RequestAudioInput
Set audio input request

| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| input | [RequestAudioInput.AudioInput](#Proto.Dsp.RequestAudioInput.AudioInput) | optional | Configure the audio input path. |


<a name="Proto.Dsp.RequestInternalSpeakerCompensation"/>
### RequestInternalSpeakerCompensation
Set the internal speaker compensation gain for one or more speaker units.
/ If a speaker unit's performance deviates from the specifications, a gain value can be applied to compensated for the
/ deviation. The FEP or one of its sub-components persist the compensation gain in non-volatile memory. FEP replies
/ with RequestInternalSpeakerCompensation.

| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| internalSpeaker | [InternalSpeaker](#Proto.Dsp.InternalSpeaker) | required |  |


<a name="Proto.Dsp.RequestPositionSoundMode"/>
### RequestPositionSoundMode


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| positionSoundMode | [PositionSoundMode](#Proto.Dsp.PositionSoundMode) | optional |  |


<a name="Proto.Dsp.ResponseInternalSpeakerCompensation"/>
### ResponseInternalSpeakerCompensation
Response when setting or getting internal speaker compensation

| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| error | [ResponseInternalSpeakerCompensation.Error](#Proto.Dsp.ResponseInternalSpeakerCompensation.Error) | optional |  |
| internalSpeaker | [InternalSpeaker](#Proto.Dsp.InternalSpeaker) | required | Current internal speaker compensation |


<a name="Proto.Dsp.ResponsePositionSoundMode"/>
### ResponsePositionSoundMode


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| error | [ResponsePositionSoundMode.Error](#Proto.Dsp.ResponsePositionSoundMode.Error) | optional |  |
| mode | [PositionSoundMode](#Proto.Dsp.PositionSoundMode) | optional | Current position sound mode |


<a name="Proto.Dsp.ToneTouch"/>
### ToneTouch


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| Gx1 | [double](#double) | optional |  |
| Gy1 | [double](#double) | optional |  |
| Gx2 | [double](#double) | optional |  |
| Gy2 | [double](#double) | optional |  |
| Gz | [double](#double) | optional |  |
| k5 | [double](#double) | optional |  |
| k6 | [double](#double) | optional |  |
| enabled | [bool](#bool) | optional |  |



<a name="Proto.Dsp.AudioMode.AudioMode"/>
### AudioMode.AudioMode


| Name | Number | Description |
| ---- | ------ | ----------- |
| ENTERTAINMENT | 0 |  |
| MASSAGE | 1 |  |

<a name="Proto.Dsp.InternalSpeaker.Position"/>
### InternalSpeaker.Position


| Name | Number | Description |
| ---- | ------ | ----------- |
| LEFT | 0 |  |
| RIGHT | 1 |  |
| CENTRE | 2 |  |

<a name="Proto.Dsp.InternalSpeaker.Type"/>
### InternalSpeaker.Type


| Name | Number | Description |
| ---- | ------ | ----------- |
| TWEETER | 0 |  |
| MIDRANGE | 1 |  |
| WOOFER | 2 |  |
| FULLRANGE | 3 |  |

<a name="Proto.Dsp.Parameter.Type"/>
### Parameter.Type


| Name | Number | Description |
| ---- | ------ | ----------- |
| LOUDNESS | 0 |  |
| BASS | 1 |  |
| TREBLE | 2 |  |

<a name="Proto.Dsp.PositionSoundMode.Orientation"/>
### PositionSoundMode.Orientation


| Name | Number | Description |
| ---- | ------ | ----------- |
| NONE | 0 |  |
| HORIZONTAL | 1 |  |
| VERTICAL | 2 |  |

<a name="Proto.Dsp.PositionSoundMode.Position"/>
### PositionSoundMode.Position


| Name | Number | Description |
| ---- | ------ | ----------- |
| UNDEFINED | 0 |  |
| FREE | 1 |  |
| WALL | 2 |  |
| CORNER | 3 |  |
| TABLE | 4 |  |

<a name="Proto.Dsp.RequestAudioInput.AudioInput"/>
### RequestAudioInput.AudioInput


| Name | Number | Description |
| ---- | ------ | ----------- |
| WIFIBT | 0 |  |
| SDCARD | 1 |  |
| LINE | 2 |  |

<a name="Proto.Dsp.ResponseInternalSpeakerCompensation.Error"/>
### ResponseInternalSpeakerCompensation.Error


| Name | Number | Description |
| ---- | ------ | ----------- |
| NO_ERROR | 1 |  |
| POSITION_ERROR | 2 |  |
| TYPE_ERROR | 3 |  |
| POSITION_TYPE_COMBINATION_ERROR | 4 |  |
| GAIN_ERROR | 5 |  |

<a name="Proto.Dsp.ResponsePositionSoundMode.Error"/>
### ResponsePositionSoundMode.Error


| Name | Number | Description |
| ---- | ------ | ----------- |
| NO_ERROR | 1 |  |
| POSITION_ERROR | 2 |  |
| ORIENTATION_ERROR | 3 |  |
| COMBINATION_ERROR | 4 |  |




<a name="fep-ase.proto"/>
<p align="right"><a href="#top">Top</a></p>

## fep-ase.proto



<a name="Proto.FepAse.Event"/>
### Event
Event report from FEP to ASE (Local UI).

| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [Event.Type](#Proto.FepAse.Event.Type) | optional |  |
| productionTunnel | [Tunnel](#Proto.Production.Tunnel) | optional |  |


<a name="Proto.FepAse.Req"/>
### Req


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [ReqResp](#Proto.FepAse.ReqResp) | optional | this should be copied from the request to the response/ to be able to pair messages |
| id | [uint32](#uint32) | optional | this should be copied from the request to the response/ to be able to pair messages |


<a name="Proto.FepAse.Resp"/>
### Resp


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| type | [ReqResp](#Proto.AseFep.ReqResp) | optional | this should be copied from the request to the response/ to be able to pair messages |
| id | [uint32](#uint32) | optional | this should be copied from the request to the response/ to be able to pair messages |
| genericResponse | [GenericResponse](#Proto.Core.GenericResponse) | optional |  |



<a name="Proto.FepAse.Event.Type"/>
### Event.Type


| Name | Number | Description |
| ---- | ------ | ----------- |
| LINE_IN_SENSE_ACTIVE | 0 |  |
| LINE_IN_SENSE_INACTIVE | 1 |  |
| TUNNEL | 10 | associated data in productionTunnel |




<a name="fep-ase-ReqResp.proto"/>
<p align="right"><a href="#top">Top</a></p>

## fep-ase-ReqResp.proto




<a name="Proto.FepAse.ReqResp"/>
### ReqResp


| Name | Number | Description |
| ---- | ------ | ----------- |
| PING | 0 | Generic response |




<a name="player.proto"/>
<p align="right"><a href="#top">Top</a></p>

## player.proto



<a name="Proto.Player.Data"/>
### Data


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| state | [Data.State](#Proto.Player.Data.State) | optional |  |
| source | [Data.Source](#Proto.Player.Data.Source) | optional |  |



<a name="Proto.Player.Data.Source"/>
### Data.Source


| Name | Number | Description |
| ---- | ------ | ----------- |
| UNKNOWN | 0 |  |
| BLUETOOTH | 1 |  |

<a name="Proto.Player.Data.State"/>
### Data.State


| Name | Number | Description |
| ---- | ------ | ----------- |
| PLAYING | 0 |  |
| PAUSED | 1 |  |
| STOPPED | 2 |  |




<a name="production.proto"/>
<p align="right"><a href="#top">Top</a></p>

## production.proto



<a name="Proto.Production.Tunnel"/>
### Tunnel


| Field | Type | Label | Description |
| ----- | ---- | ----- | ----------- |
| data | [bytes](#bytes) | optional |  |







<a name="scalar-value-types"/>
## Scalar Value Types

| .proto Type | Notes | C++ Type | Java Type | Python Type |
| ----------- | ----- | -------- | --------- | ----------- |
| <a name="double"/> double |  | double | double | float |
| <a name="float"/> float |  | float | float | float |
| <a name="int32"/> int32 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint32 instead. | int32 | int | int |
| <a name="int64"/> int64 | Uses variable-length encoding. Inefficient for encoding negative numbers – if your field is likely to have negative values, use sint64 instead. | int64 | long | int/long |
| <a name="uint32"/> uint32 | Uses variable-length encoding. | uint32 | int | int/long |
| <a name="uint64"/> uint64 | Uses variable-length encoding. | uint64 | long | int/long |
| <a name="sint32"/> sint32 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int32s. | int32 | int | int |
| <a name="sint64"/> sint64 | Uses variable-length encoding. Signed int value. These more efficiently encode negative numbers than regular int64s. | int64 | long | int/long |
| <a name="fixed32"/> fixed32 | Always four bytes. More efficient than uint32 if values are often greater than 2^28. | uint32 | int | int |
| <a name="fixed64"/> fixed64 | Always eight bytes. More efficient than uint64 if values are often greater than 2^56. | uint64 | long | int/long |
| <a name="sfixed32"/> sfixed32 | Always four bytes. | int32 | int | int |
| <a name="sfixed64"/> sfixed64 | Always eight bytes. | int64 | long | int/long |
| <a name="bool"/> bool |  | bool | boolean | boolean |
| <a name="string"/> string | A string must always contain UTF-8 encoded or 7-bit ASCII text. | string | String | str/unicode |
| <a name="bytes"/> bytes | May contain any arbitrary sequence of bytes. | string | ByteString | str |
