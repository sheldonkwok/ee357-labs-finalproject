#ifndef DEF_TIME_DECLAR_H
    #define DEF_TIME_DECLAR_H
    #ifndef DEF_TIMEDATE
        #define DEF_TIMEDATE
        typedef struct TimeDate                                         // rtc time structure
        {                                                               // fields are BCD unless noted otherwise
            uint8             u8_year;                                  // 00-99
            uint8             u8_month;                                 // 01-12
            uint8             u8_date;                                  // 01-31
            uint8             u8_day;                                   // 01-07
            uint8             u8_hours;                                 // 00-23
            uint8             u8_mins;                                  // 00-59
            uint8             u8_secs;                                  // 00-59
            uint16            u16_rtc_clock_tic;                        // one millisecond clock tic
        }TimeDateStruct, *TimeDateStructPtr;
    #endif
//----------------------------------------------------------------------------------------------------------------------
#pragma options align=packed
typedef union{
    struct{
        struct{
            uint8    u8_sub_seconds;
            uint8    u8_seconds;
            uint8    u8_mins;
            uint8    u8_hours;
            uint8    u8_day;
            uint8    u8_date;
            uint8    u8_month;
            uint8    u8_year;
        }s_rtc_timedate_struct;
        uint8    u8_rtc_calibration;
        uint8    u8_rtc_watchdog;
        struct{
            uint8    u8_alarm_month;
            uint8    u8_alarm_date;
            uint8    u8_alarm_hours;
            uint8    u8_alarm_minutes;
            uint8    u8_alarm_seconds;
        }s_rtc_alarm_timedate_struct;
        uint8    u8_rtc_flags;
        uint8    u8_rtc_reserved_1;
        uint8    u8_rtc_reserved_2;
        uint8    u8_rtc_reserved_3;
        uint8    u8_rtc_square_wave;
    }s_rtc;
    uint8   au8_rtc_buffer[21];
}m41t81s_rtc_union, *m41t81s_rtc_union_ptr;
#pragma options align=reset

#pragma options align=packed
typedef union{
    struct{
        struct{
            uint8    u8_seconds;
            uint8    u8_mins;
            uint8    u8_hours;
            uint8    u8_day;
            uint8    u8_date;
            uint8    u8_month;
            uint8    u8_year;
        }s_rtc_timedate_struct;
        uint8    u8_rtc_cal_cfg1;
        uint8    u8_rtc_tch2;
        uint8    u8_rtc_cfg2;
    }s_rtc;
    uint8   au8_rtc_buffer[11];
}rtc_bq32000_union, *rtc_bq32000_union_ptr;
#pragma options align=reset

#pragma options align=packed
typedef union{
    struct{
        struct{
            uint16       u16_value;
        }s_ts_temp;
        uint8       u8_config;
        uint8       u8_tos;
        uint8       u8_hyst;
    }s_ts;
    uint8   au8_ts_buffer[5];
}ts_lm75_union, *ts_lm75_union_ptr;
#pragma options align=reset

#pragma options align=packed
typedef union{
    struct{
        uint8       u8_xout_l;
        uint8       u8_xout_h;
        uint8       u8_yout_l;
        uint8       u8_yout_h;
        uint8       u8_zout_l;
        uint8       u8_zout_h;
    }s_accel_data;
    uint8   au8_accel_data_buffer[6];
}accel_data_union, *accel_data_union_ptr;
#pragma options align=reset

#pragma options align=packed
typedef union{
    struct{
        uint8       u8_xout_l;
        uint8       u8_xout_h;
        uint8       u8_yout_l;
        uint8       u8_yout_h;
        uint8       u8_zout_l;
        uint8       u8_zout_h;
        uint8       u8_xout_8;
        uint8       u8_yout_8;
        uint8       u8_zout_8;
        uint8       u8_status;
        uint8       u8_detsrc;
        uint8       u8_tout;
        uint8       u8_reserve1;
        uint8       u8_i2cad;
        uint8       u8_usrinf;
        uint8       u8_whoami;
        uint8       u8_xoff_l;
        uint8       u8_xoff_h;
        uint8       u8_yoff_l;
        uint8       u8_yoff_h;
        uint8       u8_zoff_l;
        uint8       u8_zoff_h;
        uint8       u8_mctl;
        uint8       u8_intrst;
        uint8       u8_ctl1;
        uint8       u8_ctl2;
        uint8       u8_ldth;
        uint8       u8_pdth;
        uint8       u8_pw;
        uint8       u8_lt;
        uint8       u8_tw;
        uint8       u8_reserve2;
    }s_accel_block;
    uint8   au8_accel_block_buffer[32];
}accel_block_union, *accel_block_union_ptr;
#pragma options align=reset


#endif
