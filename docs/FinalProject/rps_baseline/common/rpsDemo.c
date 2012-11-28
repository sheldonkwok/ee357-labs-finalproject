#include "PE_Const.h"
#include "IO_Map.h"

//===============================================================================================
#include "RPS_BldOptions.h"

#include <string.h>

#include "TypeDef.h"

#include "FcnDef.h"
#include "ExtGlobalVarDeclar.h"
#include "lcdConstDef.h"

#include "EnumDef.h"
#include "fonts.h"
#include "grphEnumDef.h"

#include "SwitchStructDef.h"

#include "lcdStructDef.h"
#include "DeviceRegStructDef.h"

#include "sdEnumDef.h"

#include "sdCmd.h"
#include "sdFileSys.h"
#include "sdFcnDef.h"
#include "sdCmd.h"
#include "sdCS.h"

#include "rtcAlarmStructDef.h"

#include "grphSCL.h"
#include "grphRS.h"
#include "grphSI.h"
#include "grphRS.h"
#include "grphRESET.h"

extern      File_iMn                     File1;
extern      FilePtr                      FilePtr1;
extern      s_rtcAlarmStruct             s_rtc_alarm;   // structure for rtc alarms
extern      ps_rtcAlarmStruct            ps_rtc_alarm;
extern	    uint16                       u16_block_size, u16_data_bytes_snt, *pu16_data_bytes_snt;
extern      uint8                       *pu8_data_block;             // remove and use au8_master below

//----------------------------------------------------------------------------------------------
extern TimeDateStruct               SystemTimeTable;     

extern TimeDateStructPtr            SystemTimeTablePtr;
//----------------------------------------------------------------------------------------------

extern SwStatusStructPtr            swStatusFlagsPtr;
extern SwStatusStruct               swStatusFlags;
//----------------------------------------------------------------------------------------------

extern lcdTimeDateStaticStructPtr   lcdTimeDateStaticPtr;
extern lcdTimeDateDynStructPtr      lcdTimeDateDynPtr;

void        MainMenu            (void);
void        TestSound           (void);
void        clear_sd_buff       (void);
void        ScriptTest          (void);
void        vFn_accel_test      (void);
void        vFn_Bin2BCD         (int16  i16_num);
void        vFn_TestCalibAccel  (void);

//================================================================================================
//
//      RTC clock on i2c bus
//
//================================================================================================
#include "PE_Types.h"

extern  uint8               u8_status;
extern                      File_iMn                     File1;
extern                      FilePtr                      FilePtr1;
extern  uint16              u16_interrupt_status;

uint8                       u8_status;

uint8 static                read_char;
uint8 static                u8_read_byte;
uint16 static               u16_slave_data_rcvd;

uint32                      test_id;
uint8 static                status_reg;

uint16 static               u16_debug_loop_cnt =1;

/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( DebugSys ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

DebugSys
    Function:   DebugSys
    
    Input:      void
    
    Return:     void
    
    Notes:      device test fcns, see RPS_BldOptions.h
--------------------------------------------------------------------------------------------------*/
void rpsDemo(void){
#ifdef DEMO_TEST
//                          123456789012345678901
uint8   mssg1[4][25] =  { {"switch test"},
                          {"rt,lft,up,dwn,sel"},
                          {"              "},
                          {"             "}
                        };

uint8   mssg2[4][25] =  { {"playing song"},
                          {"railroad.wav"},
                           {"              "},
                           {"              "}
                        };

uint8   static  u8_flshStatus;
uint16  static  u16_status;
uint32  static  u32_flshID, u32_number_blocks, u32_addr;

char            c8_temp_value = 0x3E;
int             i16_temp_buf;

typedef union{
    struct{
        char    ac8_value_ascii[6];
        char    ac8_var_name[10];
    }string_struct;
    char    ac8_buffer[16];
}lcd_sensor_union, *lcd_sensor_union_ptr;

#ifdef RPS_FM1_9_1_2_TEST_A

    ts_lm75_union               u_ts_lm75;              // create struct for Temp Sensor
    ts_lm75_union_ptr           pu_ts_lm75;

    accel_block_union           u_accel_block;
    accel_block_union_ptr       pu_accel_block;

    lcd_sensor_union            u_lcd_sensor;
    lcd_sensor_union_ptr        pu_lcd_sensor;

    pu_ts_lm75              = &u_ts_lm75;
    pu_accel_block          = &u_accel_block;
//rwl      ps_lcd_sensor_string    = &s_lcd_sensor_string;


//rwl      sprintf(c8_temp,"%d", c8_temp_value);

    vFn_lcdBkLite(ge_LED_ON);
    TimeDelay(ge_T_1_SEC);
    vFn_lcdBkLite(ge_LED_OFF);
    TimeDelay(ge_T_1_SEC);
    vFn_lcdBkLite(ge_LED_ON);
//------------------------------------- Switch Tests -------------------------------------------------------------------
    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines

//-----------------------------------rt switch -------------------------------------------------------------------------
    vFn_swResetAndEnable();                             // init all switches for use, (or could do just one used since we wait on only one)
    vFn_swPoll
        ( &(swStatusFlags.Rt.Status),
         ge_WAIT_ON_SW);                                // wait on just UP switch
    ledIndicator
        (ge_LED_ON,
         ge_LED_RED,
         NULL_CHAR);

    memset(&mssg1, 0x20, sizeof(mssg1));                // clear the input string array

    strncpy( (char *)&mssg1[2][0], "RT Sw Detected", sizeof("RT Sw Detected") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
//-----------------------------------Lft switch -------------------------------------------------------------------------
    vFn_swResetAndEnable();                             // init all switches for use, (or could do just one used since we wait on only one)
    vFn_swPoll
        ( &(swStatusFlags.Lft.Status),
         ge_WAIT_ON_SW);                                // wait on just UP switch
    ledIndicator
        (ge_LED_ON,
         ge_LED_GREEN,
         NULL_CHAR);

    memset(&mssg1, 0x20, sizeof(mssg1));            // clear the input string array

    strncpy( (char *)&mssg1[2][0], "Lft Sw Detected", sizeof("Lft Sw Detected") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
//-----------------------------------Up switch -------------------------------------------------------------------------
    vFn_swResetAndEnable();                             // init all switches for use, (or could do just one used since we wait on only one)
    vFn_swPoll
        ( &(swStatusFlags.Up.Status),
         ge_WAIT_ON_SW);                                // wait on just UP switch
    ledIndicator
        (ge_LED_OFF,
         ge_LED_RED,
         NULL_CHAR);

    memset(&mssg1, 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[2][0], "Up Sw Detected", sizeof("Up Sw Detected") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
//-----------------------------------Dwn switch -------------------------------------------------------------------------
    vFn_swResetAndEnable();                             // init all switches for use, (or could do just one used since we wait on only one)
    vFn_swPoll
        ( &(swStatusFlags.Dwn.Status),
         ge_WAIT_ON_SW);                                // wait on just UP switch
    ledIndicator
        (ge_LED_OFF,
         ge_LED_GREEN,
         NULL_CHAR);

    memset(&mssg1, 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[2][0], "Dwn Sw Detected", sizeof("Dwn Sw Detected") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
//-----------------------------------Sel switch -------------------------------------------------------------------------
    vFn_swResetAndEnable();                             // init all switches for use, (or could do just one used since we wait on only one)
    vFn_swPoll
        ( &(swStatusFlags.Sel.Status),
         ge_WAIT_ON_SW);                                // wait on just UP switch
    ledIndicator
        (ge_LED_TOGGLE,
         ge_LED_BOTH,
         0x03);

    memset(&mssg1[0][0], 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[2][0], "Sel Sw Detected", sizeof("Sel Sw Detected") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines

        TimeDelay(ge_T_1_SEC);                              //Delay before rereading to ensure that values are written
//------------------------------------ Display the RTC -------------------------------------
    vFn_lcdBkLite(ge_LED_ON);
    ledIndicator
        (ge_LED_TOGGLE,
         ge_LED_BOTH,
         0x03);

    memset(&mssg1[0][0], 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[0][0], "RTC - Rt to exit", sizeof("RTC - Rt to exit") );

    vFn_grphClrView(FontSizeCurr);                      // clean draw outline
    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
    DispClockStatic
       (lcdTimeDateStaticPtr,
        ge_TIME_ENABLED,
        ge_DATE_ENABLED,
        FontSizeCurr);

//rwl      vFn_rtc_m41t81s_test();

    do{
        vFn_rtcClockDispUpdate
            (SystemTimeTablePtr,
             lcdTimeDateDynPtr,
             FONT_NINE_DOT); 
        
        vFn_swPoll
            ( &(swStatusFlags.Rt.Status),
             ge_READ_ONCE );                            // wait on one sw
    }while (!(swStatusFlagsPtr->Rt.Status & ge_SW_FLAG_SET));       // exit if rt sw
    vFn_grphClrView(FontSizeCurr);                      // clean draw outline
//------------------------------------ Play a Wave file from the SD Card ----------------------------------------------
//rwl      memset(&mssg1[0][0], 0x20, sizeof(mssg1));   // clear the input string array
//rwl  
//rwl      strncpy( (char *)&mssg1[0][0], "Wave File Test", sizeof("Wave File Test") );
//rwl  
//rwl      vFn_DispLineSet
//rwl          ( &(mssg1[0][0]),                               // where mssg starts
//rwl           sizeof(mssg1[0]),                              // size of line
//rwl           FONT_SEVEN_DOT,
//rwl           ge_CLEAR_SCREEN);                              // clear screen before write of lines
//rwl      u16Fn_WaveOut
//rwl         ("fwM822.wav",
//rwl            ge_SD_FILE_STREAM,
//rwl            0x00);
//------------------------------------- Copy a file from the SD to the external Flash Memory ---------------------------
    memset(&mssg1[0][0], 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[0][0], "SD-Flash Write", sizeof("SD-Flash Write") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
    u16_status = u16Fn_flshWriteToFlash
                     ("railroad.wav",                   // name is not user, here for clarity, index used
                       ge_SD_FILE_STREAM,               // you can do a verify to validate and display the correct status, we assume it did
                       0x00);                           // copy a wave file from the SD to external flash memory
//------------------------------------- Play the file in flash through the Amplifier -----------------------------------
    memset(&mssg1, 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[0][0], "Play Flash wave", sizeof("Play Flash wave") );
    strncpy( (char *)&mssg1[1][0], "Any Switch-Cont", sizeof("Any Switch-Cont") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
    u16Fn_WaveOut
       ("railroad.wav",
          ge_FLASH_MEMORY_STREAM,
          0x00);                                        // play file, uses index, file name is not used
//------------------------------------ Display the i2c Temperature Sensor reading -------------------------------------
    memset(&mssg1[0][0], 0x20, sizeof(mssg1));   // clear the input string array

    strncpy( (char *)&mssg1[0][0], "TempSensor", sizeof("TempSensor") );
    strncpy( (char *)&mssg1[1][0], "TS/Lft read,Rt ex", sizeof("TS/Lft read,Rt ex") );

    vFn_DispLineSet
        ( &(mssg1[0][0]),                               // where mssg starts
         sizeof(mssg1[0]),                              // size of line
         FONT_SEVEN_DOT,
         ge_CLEAR_SCREEN);                              // clear screen before write of lines
    do{
        do{
            vFn_TempSensor(pu_ts_lm75);                 // values are in struct
            pu_ts_lm75->s_ts.s_ts_temp.u16_value = (pu_ts_lm75->s_ts.s_ts_temp.u16_value) >> 5;      // shift rt 5

            itoa
                (&mssg1[2][0],
                 pu_ts_lm75->s_ts.s_ts_temp.u16_value, 0x0A);
            strncat( (char *)&mssg1[2][0], " Counts", sizeof(" Counts") );

            vFn_DispLineSet
                ( &(mssg1[0][0]),                               // where mssg starts
                 sizeof(mssg1[0]),                              // size of line
                 FONT_SEVEN_DOT,
                 ge_CLEAR_SCREEN);                              // clear screen before write of lines

            WaitOnAnySwitch();
        }while ((swStatusFlagsPtr->Lft.Status & ge_SW_FLAG_SET));   // cont to read, if lft sw
    }while (!(swStatusFlagsPtr->Rt.Status & ge_SW_FLAG_SET));       // exit if rt sw

//------------------------------------ Display the i2c Accel Sensor reading -------------------------------------
    memset(&mssg1[0][0], 0x20, sizeof(mssg1));              // clear the input string array

    strncpy( (char *)&mssg1[0][0], "AC/Lft read,Rt ex", sizeof("AC/Lft read,Rt ex") );

    vFn_InitAccel(ge_MMA7455_GFORCE_2G);
//rwl      u16Fn_CalibAccel(ge_MMA7455_GFORCE_2G);
    do{
        do{
            vFn_ReadAccelData
                    (ge_MMA7455_XOUTL,
                     ge_ACCEL_BLOCK_SIZE,
                     &u_accel_block.au8_accel_block_buffer[0]);

            itoa
                (&mssg1[1][0],
                 pu_accel_block->s_accel_block.u8_xout_8, 0x0A);
            strncat( (char *)&mssg1[1][0], "  X Out", sizeof("  X Out") );

            itoa
                (&mssg1[2][0],
                 pu_accel_block->s_accel_block.u8_yout_8, 0x0A);
            strncat( (char *)&mssg1[2][0], "  Y Out", sizeof("  Y Out") );

            itoa
                (&mssg1[3][0],
                 pu_accel_block->s_accel_block.u8_zout_8, 0x0A);
            strncat( (char *)&mssg1[3][0], "  Z Out", sizeof("  Z Out") );

            vFn_DispLineSet
                ( &(mssg1[0][0]),                               // where mssg starts
                 sizeof(mssg1[0]),                              // size of line
                 FONT_SEVEN_DOT,
                 ge_CLEAR_SCREEN);                              // clear screen before write of lines

            WaitOnAnySwitch();
        }while ((swStatusFlagsPtr->Lft.Status & ge_SW_FLAG_SET)); // cont if lft sw
    }while (!(swStatusFlagsPtr->Rt.Status & ge_SW_FLAG_SET));   // exit if rt sw

//----------------------------------------------------------------------------------------------------------------------
 #endif
//----------------------------------------------------------------------------------------------------------------------

#ifdef SPI_SIGNAL_TEST
    for(;;){
        sdCS_SetVal();
        sdCS_ClrVal();
        u8Fn_RxTx_spi(0xAA);                                // transmit byte for signal checks

        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef GRPH_LINES_TEST
    for(;;){
        grphRESET_ClrVal();                                 // reset the LCD controller chip, set the reset line low
        grphRS_ClrVal();                                    // set the CD line low as output
        grphSCL_ClrVal();                                   // set SCLK line low
        grphSI_ClrVal();                                    // set SDAT line low
        grphRESET_SetVal();                                 // set RST signal high off output
        grphRS_SetVal();                                    // set the CD line high as output
        grphSCL_SetVal();                                   // set SCLK line high
        grphSI_SetVal();                                    // set SDAT line high
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }

//rwl         break;
#endif

#ifdef LED_TEST
    for(;;){
        ledIndicator                                        // explicitly set LED's off at start up
            (ge_LED_ON,
             ge_LED_RED,
             NULL_CHAR);

        ledIndicator
            (ge_LED_ON,
             ge_LED_GREEN,
             NULL_CHAR);

        TimeDelay(ge_T_1_SEC);                                 // wait so a spin loop is not incountered

        ledIndicator                                        // explicitly set LED's off at start up
            (ge_LED_OFF,
             ge_LED_RED,
             NULL_CHAR);

        ledIndicator
            (ge_LED_OFF,
             ge_LED_GREEN,
             NULL_CHAR);

        TimeDelay(ge_T_1_SEC);                              // wait so a spin loop is not incountered

        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef DISP_BKLITE
    for(;;){
        vFn_lcdBkLite(ge_LED_ON);
        TimeDelay(ge_T_1_SEC);
        vFn_lcdBkLite(ge_LED_OFF);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef DISP_TEST
    for(;;){
            vFn_lcdBkLite(ge_LED_OFF);
            vFn_lcdBkLite(ge_LED_ON);
            vFn_grphClrView(FontSizeCurr);                  // clean draw outline
            TimeDelay(ge_T_1_SEC);
            vFn_DispLineSet
                ( &(debug_mssg[0][0]),                      // where mssg starts
                 sizeof(debug_mssg[0]),                     // size of line
                 FontSizeCurr,
                 ge_CLEAR_SCREEN);                      // clear screen before write of lines
            TimeDelay(ge_T_1_SEC);
            vFn_lcdBkLite(ge_LED_OFF);
            TimeDelay(ge_T_1_SEC);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FLASH_ID_TEST
    for(;;){
        u32_flshID = u32Fn_flshGetID();
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FLASH_STATUS_TEST
    for(;;){
        u8_flshStatus = u8Fn_flshGetStatusReg();
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FLASH_ERASE_TEST
    for(;;){
        vFn_flshChipErase();
        u32_addr = 0x00;
        u32_number_blocks = 60;
        u16_status = u16Fn_flshFileEraseVerify
                        (u32_addr,
                         u32_number_blocks);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FLASH_RW_TEST
    for(;;){
        vFn_flshTestWrite();
        vFn_flshTestRead();
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FLASH_SD_WRITE_TEST
    for(;;){
        u16_status = u16Fn_flshWriteToFlash
                         ("fwM822.wav",
                           ge_SD_FILE_STREAM,
                           0x00);
         if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FLASH_SD_VERIFY_TEST
    for(;;){
        u16_status = u16Fn_flshSdFileVerify
                         ("fwM822.wav",
                           ge_SD_FILE_STREAM,
                           0x00);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef SD_WAVE_TEST
    for(;;){
        u16Fn_WaveOut
           ("fwM822.wav",
              ge_SD_FILE_STREAM,
              0x00);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef SCRIPT_TEST
    for(;;){
        vFn_ScriptMaster
            ("script04.cmd",
             ge_SD_FILE_STREAM,
             NULL_CHAR,
             FontSizeCurr);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef DEF_DISPINFORM
    for(;;){
        DispInform(FONT_NINE_DOT);                          // query, display block of information
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef LOGO_ON
    for(;;){
        DispLogo();
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef RTC_TEST
    for(;;){
        vFn_rtcOscillCntrl(ge_RTC_ENABLE_OSCILL);

        rtcStartClock(SystemTimeTablePtr);                  // start rtc, read date time, validate

        vFn_rtcAlarmOper(ge_RTC_ALARM_ENABLE_0);       // enable alarm int
        vFn_rtcAlarmTimeSet
            (ps_rtc_alarm,
             SystemTimeTablePtr,
             ge_RTC_ALARM_SINGLE);

        rtcClockDisp(FONT_NINE_DOT);                        // runs until switch pushed
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef RTC_ALARM_TEST
    vFn_rtcOscillCntrl(ge_RTC_ENABLE_OSCILL);
    for(;;){
        ps_rtc_alarm          = &s_rtc_alarm;

        SystemTimeTablePtr->u8_year   = 0x08;
        SystemTimeTablePtr->u8_month  = 0x06;
        SystemTimeTablePtr->u8_date   = 0x05;
        SystemTimeTablePtr->u8_day    = 0x04;
        SystemTimeTablePtr->u8_hours  = 0x03;
        SystemTimeTablePtr->u8_mins   = 0x02;
        SystemTimeTablePtr->u8_secs   = 0x01 ;    // BCD time values, system clock is always set by RTC

        rtcWriteTime(SystemTimeTablePtr);               // set the system time
        rtcWriteDate(SystemTimeTablePtr);               // it will have been set in TimeMenu

        vFn_rtcReadAlarm(ps_rtc_alarm);              // disable rtc alarms, if any, if alarms desired add logic
        vFn_rtcAlarmOper(ge_RTC_ALARM_DISABLE_0);       // enable alarm int
        vFn_rtcAlarmTimeSet
            (ps_rtc_alarm,
             SystemTimeTablePtr,
             ge_RTC_ALARM_SINGLE);

        vFn_ScriptMaster
            (ps_rtc_alarm->c_alarm_clk_file,
             ge_SD_FILE_STREAM,
             0,
             FONT_NINE_DOT);
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#ifdef FILE_SPEED_TEST
    for(;;){
        vFn_FileSpeedTest();
        if(DEBUG_LOOP_CNTRL != ge_INFINITE_LOOP){
            break;
        }
    }
#endif

#endif
}
/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_accel_test )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_accel_test
    Function:       test read of accelerometer
    
    Input:          void

    Return:         void
----------------------------------------------------------------------------------------------------*/
void    vFn_accel_test (void)
{
uint8  static au8_data_1[2], u8_read_byte;

//rwl            pu_mssg_cmnd        = &u_mssg_cmnd;    // assign pointers for mssg block
            pu16_data_bytes_snt = &u16_data_bytes_snt;

            au8_data_1[0] = ge_MMA7455_MCTL;            // set control register address
            au8_data_1[1] = ge_MMA7455_GFORCE_4G;       // select 4g mode

            pu8_data_block = &au8_data_1[0];
            u16_block_size = sizeof(au8_data_1);

            vFn_i2cDeviceSetup(ge_MMA7455L_ADDR);       // setup bean and processor

            u8Fn_WriteBlock_i2c
                    (pu8_data_block,
                     u16_block_size,
                     pu16_data_bytes_snt);              // write control register

//rwl              for(;;){
                 u8_read_byte = vFn_ReadReg_i2c( (uint8)ge_MMA7455_YOUT8);   // get device address
//rwl              }
    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_InitAccel )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_InitAccel
    Function:       init the accelerometer
    
    Input:          uint16 accel scale

    Return:         void
----------------------------------------------------------------------------------------------------*/
void    vFn_InitAccel (uint16   u16_accel_scale)
{
uint8  static au8_data[2], u8_read_byte;

            pu16_data_bytes_snt = &u16_data_bytes_snt;

            au8_data[0] = ge_MMA7455_MCTL;            // set control register address
            au8_data[1] = u16_accel_scale;            // select acclel scale

            pu8_data_block = &au8_data[0];
            u16_block_size = sizeof(au8_data);

            vFn_i2cDeviceSetup(ge_MMA7455L_ADDR);       // setup bean and processor

            u8Fn_WriteBlock_i2c
                    (pu8_data_block,
                     u16_block_size,
                     pu16_data_bytes_snt);              // write control register

    return;
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_TestCalibAccel )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_TestCalibAccel
    Function:       calibrate the accelerometer
    
    Input:          none

    Return:         void
----------------------------------------------------------------------------------------------------*/
void    vFn_TestCalibAccel (void)
{
uint16  static  u16_axis_cal;

    u16_axis_cal = u16Fn_CalibAccel
                        (ge_MMA7455_GFORCE_2G,
                        ge_MMA7455L_X_AXIS);

    u16_axis_cal = u16Fn_CalibAccel
                        (ge_MMA7455_GFORCE_2G,
                        ge_MMA7455L_Y_AXIS);

    u16_axis_cal = u16Fn_CalibAccel
                        (ge_MMA7455_GFORCE_2G,
                        ge_MMA7455L_Z_AXIS);

    return;
}
/*--------------------------------------------------------------------------------------------------
((((((((((((((((((((((((((((((((( u16Fn_CalibAccel ))))))))))))))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------

u16Fn_CalibAccel
    Function:   u16Fn_CalibAccel
    
    Input:      u16_accel_gforce
                u16_accel_axis              which axis to calibrate
    
    Return:     u16_axis_cal                cal factor for given axis
    
    Notes:      zero calibrate the accelerometer
                When the device is placed horizontally 
                so that X-axis is at 0g, 
                Y-axis is at 0g and 
                Z-axis is at +1g, 
                then the Z-axis output should be: 
                    0x40 (64) in 2g mode, 
                    0x20 (32) in 4g and 
                    0x01 (16) in 8g (8-bit mode).
    
                Zdata=IIC_ByteRead(0x08);
                Zcal+=-2*(Zdata-64);	//64 is for 2g, 32 for 4g and 16 for 2g mode
                Zcalp=Zcal;
                if(Zcalp<0) Zcalp+=2048;
                Zcal_L= (byte)(Zcalp&0x00FF); 
                Zcal_H= (byte)((Zcalp&0xFF00)>>8);
	                //64 is for 2g, 32 for 4g and 16 for 2g mode

--------------------------------------------------------------------------------------------------*/
uint16 u16Fn_CalibAccel (uint16     u16_accel_gforce,
                         uint16     u16_accel_axis)
{
sint8   static  u8_axis_data;
sint16  static  u16_axis_cal,u16_calp;

uint8   static  u8_comp, u8_cal_low, u8_cal_high;

    accel_block_union           u_accel_block;
    accel_block_union_ptr       pu_accel_block;

    pu_accel_block  = &u_accel_block;

    if(u16_accel_gforce == ge_MMA7455_GFORCE_2G){
        u8_comp = 64;                                   //64 is for 2g, 32 for 4g and 16 for 2g mode for Z axis
    }else {
    if(u16_accel_gforce == ge_MMA7455_GFORCE_4G){ 
        u8_comp = 32;
    }else {
    if(u16_accel_gforce == ge_MMA7455_GFORCE_8G){ 
        u8_comp = 16;
    }}}

    vFn_i2cDeviceSetup(ge_MMA7455L_ADDR);       // setup bean and processor

    vFn_WriteReg_i2c
        (ge_MMA7455_MCTL,
         u16_accel_gforce);

    u16_axis_cal = 0;

    switch(u16_accel_axis){
        case    ge_MMA7455L_X_AXIS:
            u8_axis_data = vFn_ReadReg_i2c(ge_MMA7455_XOUT8);
            while(u8_axis_data != 0x00){
                u16_axis_cal += -2 * u8_axis_data;                  //Calc calibration based on current Data, (-)*(-) = +
                u16_calp = u16_axis_cal;                            //If calp is "positive" it is assigned to cal variable
                if(u16_calp < 0) u16_calp += 2048;                  //If Negative value then convert for 2's complement
                u8_cal_low = (byte)(u16_calp & 0x00FF);             //shift low byte to lower 8 bits
                u8_cal_high = (byte)((u16_calp & 0xFF00) >> 8);     //shift high byte to upper 8 bits
                vFn_WriteReg_i2c
                    (ge_MMA7455L_X_CAL_LOW,
                     u8_cal_low);                                   //Write low calibration value to the low calibration register
                vFn_WriteReg_i2c
                    (ge_MMA7455L_X_CAL_HIGH,
                     u8_cal_high);                                  //Write high calibration value to the high calibration register
                u8_axis_data = vFn_ReadReg_i2c(ge_MMA7455_XOUT8);   // read the data again and see if calibrated to 0
            }
        break;

        case    ge_MMA7455L_Y_AXIS:
            u8_axis_data = vFn_ReadReg_i2c(ge_MMA7455_YOUT8);
            while(u8_axis_data != 0x00){
                u16_axis_cal += -2 * u8_axis_data;
                u16_calp = u16_axis_cal;
                if(u16_calp < 0) u16_calp += 2048;                  //If Negative value then convert for 2's complement
                u8_cal_low = (byte)(u16_calp & 0x00FF);             //shift low byte to lower 8 bits
                u8_cal_high = (byte)((u16_calp & 0xFF00) >> 8);     //shift high byte to upper 8 bits
                vFn_WriteReg_i2c
                    (ge_MMA7455L_Y_CAL_LOW,
                     u8_cal_low);                                   //Write low calibration value to the low calibration register
                vFn_WriteReg_i2c
                    (ge_MMA7455L_Y_CAL_HIGH,
                     u8_cal_high);                                  //Write high calibration value to the high calibration register
                u8_axis_data = vFn_ReadReg_i2c(ge_MMA7455_YOUT8);   // read the data again and see if calibrated to 0
            }
        break;

        case    ge_MMA7455L_Z_AXIS:
            u8_axis_data = vFn_ReadReg_i2c(ge_MMA7455_ZOUT8);
            while(u8_axis_data != u8_comp){                        // have to choose value based on gforce
                u16_axis_cal += -2 *(u8_axis_data - u8_comp);
                u16_calp = u16_axis_cal;
                if(u16_calp < 0) u16_calp += 2048;
                u8_cal_low = (byte)(u16_calp & 0x00FF);             //shift low byte to lower 8 bits
                u8_cal_high = (byte)((u16_calp & 0xFF00) >> 8);     //shift high byte to upper 8 bits
                vFn_WriteReg_i2c
                    (ge_MMA7455L_Z_CAL_LOW,
                     u8_cal_low);                                   //Write low calibration value to the low calibration register
                vFn_WriteReg_i2c
                    (ge_MMA7455L_Z_CAL_HIGH,
                     u8_cal_high);                                  //Write high calibration value to the high calibration register
                u8_axis_data = vFn_ReadReg_i2c(ge_MMA7455_ZOUT8);
            }
        break;

        TimeDelay(ge_T_10_MS);
    }

     u16_axis_cal = u16_calp;
     vFn_ReadAccelData
             (ge_MMA7455_XOUTL,
              ge_ACCEL_BLOCK_SIZE,
              (uint8 *)&u_accel_block.au8_accel_block_buffer[0]);

    return(u16_axis_cal);
}

/*--------------------------------------------------------------------------------------------------
(((((((((((((((((((((((((((((((((( vFn_ReadAccelData )))))))))))))))))))))))))))))))))))
----------------------------------------------------------------------------------------------------
vFn_ReadAccelData
    Function:       test read of accelerometer
    
    Input:          pointer to block struct
                    size of block to transfer   

    Return:         this can transfer the entire block or any portion if the pointer and block size is set
----------------------------------------------------------------------------------------------------*/
void    vFn_ReadAccelData (uint8                 u8_reg_addr,
                           uint16                u16_block_size,
                           uint8                 *pu8_accel_block)
{
uint8  static au8_data[6], u8_read_byte;

            vFn_i2cDeviceSetup(ge_MMA7455L_ADDR);       // setup bean and processor

            vFn_ReadBlock_i2c
                    (u8_reg_addr,                       // start of read address
                     u16_block_size,
                     pu8_accel_block);                   // read buffer

    return;
}


