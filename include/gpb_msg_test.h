void aseFep_Event_VolumeChange(uint8 volume);
void aseFep_Event_MuteChange(uint8 mute);
void aseFep_Event_PlayerData(uint8 state);
void aseFep_Event_ToneTouch(double Gx1, double Gx2, double Gy1, double Gy2, double Gz, double k5, double k6);
void aseFep_Req_AudioMode(uint8 mode);
void aseFep_Req_AudioInput(uint8 input);
void aseFep_Req_Position(uint8 position);
void aseFep_Req_IntrSpeakerCompensation(uint8 type, uint8 position, double compensation);
void aseFep_Req_DspParam(uint8 type, uint8 value);
void aseFep_Req_DspVolumeFade(uint8 target_volume, uint16 fade_duration);


void aseFep_ACK();
void aseFep_NACK();
