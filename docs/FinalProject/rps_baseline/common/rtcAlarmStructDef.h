#ifndef DEF_S_RTCALARMSTRUCT_h
    #define DEF_S_RTCALARMSTRUCT_h
typedef struct{
    char             c_alarm_wav_file[13];
    char             c_alarm_clk_file[13];
    uint8            u8_alarm_state;
    uint8            u8_alarm_match;
    uint8            u8_month;                          // 01-12
    uint8            u8_date;                           // 01-07
    uint8            u8_hours;                          // 00-23
    uint8            u8_mins;                           // 00-59
    uint8            u8_secs;                           // 00-59
}s_rtcAlarmStruct, *ps_rtcAlarmStruct;

#endif

